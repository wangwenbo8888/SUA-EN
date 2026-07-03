

#include "fitalgorighm.h"

#include "float.h"
#include "qdebug.h"


CurveSurface::CurveSurface()
{
    A = B = C = X0 = Y0 = Z0 = INVEX;
}

double CurveSurface::average1(double* x0, int Num)
{
    double result = 0.0;
    for (int i = 0; i < Num; ++i)
    {
        result += x0[i];
    }

    return result / Num;
}

double CurveSurface::average2(double* x0, double* y0, int Num)
{
    double result = 0.0;
    for (int i = 0; i < Num; ++i)
    {
        result += x0[i]*y0[i];
    }

    return result / Num;
}

double CurveSurface::average3(double* x0, double* y0, double* z0, int Num)
{
    double result = 0.0;
    for (int i = 0; i < Num; ++i)
    {
        result += x0[i]*y0[i]*z0[i];
    }

    return result / Num;
}

double CurveSurface::average4(double* x0, double* y0, double* z0, double* e0, int Num)
{
    double result = 0.0;
    for (int i = 0; i < Num; ++i)
    {
        result += x0[i]*y0[i]*z0[i]*e0[i];
    }

    return result / Num;
}


bool CurveSurface::calcCurveSurface(arma::mat X, arma::mat Y, arma::mat Z)
{
    qDebug()<<"Enter calc surface !";
    int M = 100;
    int N = 100;
    double *ValidX = new double[M* N];
    double *ValidY = new double[M* N];
    double *ValidZ = new double[M* N];
    int validNum = 0;
    for (int i = 0; i < X.n_cols; ++i)
    {
        for (int j = 0; j < X.n_rows; ++j)
        {
            ValidX[validNum] = X(j, i);
            ValidY[validNum] = Y(j, i);
            ValidZ[validNum] = Z(j, i);
            validNum++;
        }
    }

    qDebug()<<"Step 1 !";

    arma::mat matA(6, 6, arma::fill::zeros);
    arma::vec matB(6, 1, arma::fill::zeros);
    arma::vec result(6, 1, arma::fill::zeros);

    qDebug()<<"Step 2 !";

    matA(0, 0) = average4(ValidY, ValidY, ValidY, ValidY, validNum);
    matA(0, 1) = average4(ValidY, ValidY, ValidZ, ValidZ, validNum);
    matA(0, 2) = average3(ValidX, ValidY, ValidY, validNum);
    matA(0, 3) = average3(ValidY, ValidY, ValidY, validNum);
    matA(0, 4) = average3(ValidY, ValidY, ValidZ, validNum);
    matA(0, 5) = average2(ValidY, ValidY, validNum);

    qDebug()<<"Step 3 !";

    matA(1, 0) = average4(ValidY, ValidY, ValidZ, ValidZ, validNum);
    matA(1, 1) = average4(ValidZ, ValidZ, ValidZ, ValidZ, validNum);
    matA(1, 2) = average3(ValidX, ValidZ, ValidZ, validNum);
    matA(1, 3) = average3(ValidY, ValidZ, ValidZ, validNum);
    matA(1, 4) = average3(ValidZ, ValidZ, ValidZ, validNum);
    matA(1, 5) = average2(ValidZ, ValidZ, validNum);

    qDebug()<<"Step 4 !";

    matA(2, 0) = average3(ValidX, ValidY, ValidY, validNum);
    matA(2, 1) = average3(ValidX, ValidZ, ValidZ, validNum);
    matA(2, 2) = average2(ValidX, ValidX, validNum);
    matA(2, 3) = average2(ValidX, ValidY, validNum);
    matA(2, 4) = average2(ValidX, ValidZ, validNum);
    matA(2, 5) = average1(ValidX, validNum);

    qDebug()<<"Step 5 !";

    matA(3, 0) = average3(ValidY, ValidY, ValidY, validNum);
    matA(3, 1) = average3(ValidY, ValidZ, ValidZ, validNum);
    matA(3, 2) = average2(ValidX, ValidY, validNum);
    matA(3, 3) = average2(ValidY, ValidY, validNum);
    matA(3, 4) = average2(ValidY, ValidZ, validNum);
    matA(3, 5) = average1(ValidY, validNum);

    qDebug()<<"Step 6 !";

    matA(4, 0) = average3(ValidY, ValidY, ValidZ, validNum);
    matA(4, 1) = average3(ValidZ, ValidZ, ValidZ, validNum);
    matA(4, 2) = average2(ValidX, ValidZ, validNum);
    matA(4, 3) = average2(ValidY, ValidZ, validNum);
    matA(4, 4) = average2(ValidZ, ValidZ, validNum);
    matA(4, 5) = average1(ValidZ, validNum);

    qDebug()<<"Step 7 !";

    matA(5, 0) = average2(ValidY, ValidY, validNum);
    matA(5, 1) = average2(ValidZ, ValidZ, validNum);
    matA(5, 2) = average1(ValidX, validNum);
    matA(5, 3) = average1(ValidY, validNum);
    matA(5, 4) = average1(ValidZ, validNum);
    matA(5, 5) = 1;

    qDebug()<<"Step 8 !";

    matB(0) = -average4(ValidX, ValidX, ValidY, ValidY, validNum);
    matB(1) = -average4(ValidX, ValidX, ValidZ, ValidZ, validNum);
    matB(2) = -average3(ValidX, ValidX, ValidX, validNum);
    matB(3) = -average3(ValidX, ValidX, ValidY, validNum);
    matB(4) = -average3(ValidX, ValidX, ValidZ, validNum);
    matB(5) = -average2(ValidX, ValidX, validNum);

    qDebug()<<"Step 9 !";

    result = inv(matA)*matB;
    X0 = -result(2) / 2;
    Y0 = -result(3) / (2 * result(0));
    Z0 = -result(4) / (2 * result(1));
    A = sqrt(X0*X0 + result(0)*Y0*Y0 + result(1)*Z0*Z0 - result(5));
    B = A / sqrt(result(0));
    C = A / sqrt(result(1));

    qDebug()<<"Step 10 !";

    if (std::isnan(A)||std::isnan(B)||std::isnan(C)||std::isnan(X0)||std::isnan(Y0)||std::isnan(Z0))
    {
        return false;
    }
    else
    {
        return true;
    }


    delete []ValidX;
    ValidX = NULL;
    delete []ValidY;
    ValidY = NULL;
    delete []ValidZ;
    ValidZ = NULL;

}

CurveSurface::~CurveSurface()
{
}
