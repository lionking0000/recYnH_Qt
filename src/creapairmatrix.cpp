#include "creapairmatrix.h"
#include <iostream> // for cout
#include <fstream>  // for ifstream
#include <armadillo>
#include <algorithm>    // std::sort

using namespace arma;

// https://stackoverflow.com/questions/11964552/finding-quartiles?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qac++
template <typename T1, typename T2> typename T1::value_type quant(const T1 &x, T2 q)
{
    //assert(q >= 0.0 && q <= 1.0);

    const auto n  = x.size();
    const auto id = (n - 1) * q;
    const auto lo = floor(id);
    const auto hi = ceil(id);
    const auto qs = x[lo];
    const auto h  = (id - lo);

    return (1.0 - h) * qs + h * x[hi];
}

CReadPairMatrix::CReadPairMatrix()
{
    m_alpha = 1.0;
}

int CReadPairMatrix::ReadTSV(const char* filename)
{
    this->matrixValue.empty();
    this->filterdMatrixValue.empty();
    this->nullMatrixValue.empty();
    this->IS2MatrixValue.empty();

    this->vecColName.empty();
    this->vecRowName.empty();


    ifstream infile(filename);
    if (!infile) {
        cout << "unable to load file" << endl;
    }
    string str;

    vector<vector<string>> vvStr;

    vector<string> vecTitle;
    int pos1, pos2;

    // title

    getline(infile, str);
    pos1 = 0;
    pos2 = str.find('\t',pos1);
    pos1 = ++pos2;
    while((pos2 = str.find('\t',pos1))!= string::npos)
    {
        this->vecColName.push_back(str.substr(pos1, pos2-pos1));
        pos1 = ++pos2;
    }
    this->vecColName.push_back(str.substr(pos1, string::npos));   // the last name

    // values
    //int nRowIndex = 0;
    while (getline(infile, str))
    {
        pos1 = 0;
        vector<float> vFloat;
        pos2 = str.find('\t',pos1);
        this->vecRowName.push_back( str.substr(pos1, pos2-pos1) );
        pos1 = ++pos2;
        while((pos2 = str.find('\t',pos1))!= string::npos)
        {
            vFloat.push_back(stof(str.substr(pos1, pos2-pos1)));
            pos1 = ++pos2;
        }
        vFloat.push_back(stof(str.substr(pos1, string::npos)));     // the last value
        //std::sort(vFloat.begin(),vFloat.end());
        //std::cout << nRowIndex << "\t" << quant(vFloat, 0.75) << std::endl;
        //nRowIndex++;
        this->matrixValue.push_back(vFloat);

    }

    float sum = 0;
    for(vector<float> x : this->matrixValue)
        for(float i : x)
            sum += i;

    cout << "Read pair sum = " << sum << endl;

    return 1;
}

int CReadPairMatrix::Nullmatrix()
{


    float TotalSum = 0;
    int nCols = this->vecColName.size();
    int nRows = this->vecRowName.size();


    // total sums
    for(vector<float> x : this->matrixValue)
        for(float i : x)
            TotalSum += i;

    TotalSum += ( nCols * nRows );


    vector<float> colSums;
    // col sums
    for(int i = 0; i < nCols; i++ ){
        float sum = 0;
        for(vector<float> x : this->matrixValue){
            sum += x[i];
            sum += m_alpha;
        }
        colSums.push_back( sum / TotalSum );
    }


    vector<float> rowSums;
    // row sums
    for(vector<float> x : this->matrixValue){
        float sum = 0;
        for(int i = 0; i < nRows; i++ ){
            sum += x[i];
            sum += m_alpha;
        }
        rowSums.push_back( sum / TotalSum );
    }

    nullMatrixValue.empty();
    for(int i = 0; i < nRows; i++ ){
        vector<float> x;
        for(int j = 0; j < nCols; j++ ){
            float nullValue = rowSums[i] * colSums[j];
            x.push_back( nullValue * TotalSum );   //x.push_back( int(nullValue * TotalSum) );
        }
        nullMatrixValue.push_back(x);
    }
}

int CReadPairMatrix::GMM_Fit()
{
    int N = this->vecColName.size() * this->vecRowName.size();

    // create synthetic data containing
    // 2 clusters with normal distribution
    int d = 1; // dimensionality
    //uword N = 10000; // number of samples (vectors)
    mat data(d, N, fill::zeros);

    float sum = 0;
    int i = 0;
    for(vector<float> values : this->matrixValue)
        for(float value : values)
        {
            data.col(i) = value;
            i++;
        }

    // model the data as a diagonal GMM with 2 Gaussians
    gmm_diag model;
    bool status = model.learn(data, 2, maha_dist, random_subset,10, 5, 1e-10, true);
    if(status == false) { cout << "learning failed" << endl; }

    double overall_likelihood = model.avg_log_p(data);

    rowvec set_likelihood = model.log_p( data.cols(0,9) );
    double scalar_likelihood = model.log_p( data.col(0) );

    uword gaus_id = model.assign( data.col(0), eucl_dist );
    urowvec gaus_ids = model.assign( data.cols(0,9), prob_dist );


    filterdMatrixValue.empty();
    // Add filtered values
    i = 0;
    int filtered_cnt = 0;
    int filtered_non_zero_cnt = 0;
    for(vector<float> x : this->matrixValue){
        vector<float> filteredValues;
        for(float value : x){
            uword gaus_id1 = model.assign( data.col(i), eucl_dist ); //prob_dist );
            if ( gaus_id1 == 0) {
                filteredValues.push_back(value);
                cout << "## pos\t" << value << endl;
            }
            else{
                filteredValues.push_back(0.0);
                cout << "## filtered\t" << value << endl;
                filtered_cnt ++;
                if ( value != 0 ) filtered_non_zero_cnt ++;
            }
            i++;
        }
        filterdMatrixValue.push_back(filteredValues);
    }

    urowvec histogram1 = model.raw_hist (data, prob_dist);
    rowvec histogram2 = model.norm_hist(data, eucl_dist);

    model.means.print("means:");

    cout << "filtered_cnt\t" << filtered_cnt << endl;
    cout << "filtered_non_zero_cnt\t" << filtered_non_zero_cnt << endl;
    cout << "histogram1\t" << histogram1[0] << "\t" << histogram1[1] << endl;
    cout << "histogram2\t" << histogram2[0] << "\t" << histogram2[1] << endl;

    model.save("/Users/jyang/my_model.gmm");

    mat modified_dcovs = 2 * model.dcovs;

    model.set_dcovs(modified_dcovs);

    return 0;
}

int CReadPairMatrix::InteractionScore2(vector<vector<float>>& nullMat, vector<vector<float>>& filterdMat)
{
    IS2MatrixValue.empty();

    int nCols = this->vecColName.size();
    int nRows = this->vecRowName.size();


    for(int i = 0; i < nRows; i++ ){
        vector<float> x;
        vector<float> t;
        for(int j = 0; j < nCols; j++ ){
            float nullValue = nullMat[i][j];
            float filteredValue = filterdMat[i][j];
            float IS2 = filteredValue / nullValue;
            x.push_back( IS2 );
            t.push_back( IS2 );
        }

        // Remove the 75th percentile signal
        std::sort(t.begin(),t.end());
        //std::cout << nRowIndex << "\t" << quant(vFloat, 0.75) << std::endl;
        float quantile = quant(t, 0.75);
        if ( quantile == 0.0 ){
            IS2MatrixValue.push_back( x );
        }
        else{
            for(int j = 0; j < nCols; j++ ){
                x[j] -= quantile;
                if ( x[j] < 0.0 ) x[j] = 0.0;
            }
            IS2MatrixValue.push_back( x );
        }


        //std::cout << quant(x, 0.75) << std::endl;
    }
    return 0;
}
