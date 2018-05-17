#ifndef CREADPAIRMATRIX_H
#define CREADPAIRMATRIX_H

#include<string>
#include<vector>

using namespace std;

class CReadPairMatrix
{
public:
    CReadPairMatrix();

    vector<string> vecColName;
    vector<string> vecRowName;
    vector<vector<float>> matrixValue;
    vector<vector<float>> filterdMatrixValue;
    vector<vector<float>> nullMatrixValue;
    vector<vector<float>> IS2MatrixValue;


    int ReadTSV(const char* filename);
    int GMM_Fit();
    int Nullmatrix();
    int InteractionScore2(vector<vector<float>>& nullMat, vector<vector<float>>& filterdMat);

    float m_alpha;
};

#endif // CREADPAIRMATRIX_H
