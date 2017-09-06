#ifndef __VECTORS_H_
#define __VECTORS_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <math>

typedef struct
{
    double x;
    double y;
}TVector;

TVector sub(TVector a, TVector b);
TVector add(TVector a, TVector b);
double mod(TVector in);
TVector divScalar(TVector in, double scalar);
TVector prodScalar(TVector in, double scalar);
double dotProd(TVector a, TVector b);
TVector neg(TVector in);
double projectPointOnAxis(TVector point, TVector ax1, 
                          TVector ax2, TVector * projectedPoint);

#ifdef __cplusplus
 }
#endif

#endif
