#ifndef FITALGORIGHM_H_
#define FITALGORIGHM_H_

#include "armadillo"

#include <iostream>
#include <armadillo>
#include<cmath>

#include "hifunavigation_global.h"

using namespace arma;
#define INVEX -100

struct point3d
{
  double x;
  double y;
  double z;
};

class HIFUNAVIGATIONSHARED_EXPORT CurveSurface
{
public:
    double A, B, C, X0, Y0, Z0;
    CurveSurface();
    ~CurveSurface();
    bool calcCurveSurface(arma::mat, arma::mat, arma::mat);

private:
    int cols;// =M ;
    int rows;// = N;
    double average1(double* x0, int Num);
    double average2(double* x0, double* y0, int Num);
    double average3(double* x0, double* y0, double* z0, int Num);
    double average4(double* x0, double* y0, double* z0, double* e0, int Num);

};


#endif // FITALGORIGHM_H_

