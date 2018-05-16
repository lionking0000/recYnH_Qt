#include "creapairmatrix.h"
#include <iostream> // for cout
#include <fstream>  // for ifstream
#include <armadillo>

using namespace arma;

CReadPairMatrix::CReadPairMatrix()
{
    m_alpha = 1.0;
}

int CReadPairMatrix::ReadTSV(const char* filename)
{
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
    for(int i = 0; i < nCols; i++ ){
        vector<float> x;
        for(int j = 0; i < nRows; i++ ){
            float nullValue = colSums[i] * rowSums[j];
            x.push_back( nullValue * TotalSum );
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

    model.means.print("means:");

    double overall_likelihood = model.avg_log_p(data);

    rowvec set_likelihood = model.log_p( data.cols(0,9) );
    double scalar_likelihood = model.log_p( data.col(0) );

    uword gaus_id = model.assign( data.col(0), eucl_dist );
    urowvec gaus_ids = model.assign( data.cols(0,9), prob_dist );


    filterdMatrixValue.empty();
    // Add filtered values
    i = 0;
    for(vector<float> x : this->matrixValue){
        vector<float> filteredValues;
        for(float value : x){
            uword gaus_id1 = model.assign( data.col(i), prob_dist );
            if ( gaus_id1 == 0) {
                filteredValues.push_back(value);
            }
            else{
                filteredValues.push_back(0.0);
            }
            i++;
        }
        filterdMatrixValue.push_back(filteredValues);
    }

    urowvec histogram1 = model.raw_hist (data, prob_dist);
    rowvec histogram2 = model.norm_hist(data, eucl_dist);

    cout << "histogram2\t" << histogram2[0] << "\t" << histogram2[1] << endl;

    model.save("/Users/jyang/my_model.gmm");

    mat modified_dcovs = 2 * model.dcovs;

    model.set_dcovs(modified_dcovs);

    return 0;
}
