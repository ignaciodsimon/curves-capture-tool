#include "vectors.h"

TVector sub(TVector a, TVector b)
{
    TVector _result;

    _result.x = (a).x - (b).x;
    _result.y = (a).y - (b).y;

    return _result;
}

TVector add(TVector a, TVector b)
{
    TVector _result;

    _result.x = (a).x + (b).x;
    _result.y = (a).y + (b).y;

    return _result;
}

double mod(TVector in)
{
    return sqrt( (((in).x) * ((in).x)) + (((in).y) * ((in).y)) );
}

TVector divScalar(TVector in, double scalar)
{
    TVector _result;
    _result.x = (in).x / scalar;
    _result.y = (in).y / scalar;
    return _result;
}

TVector prodScalar(TVector in, double scalar)
{
    TVector _result;
    _result.x = (in).x * scalar;
    _result.y = (in).y * scalar;
    return _result;
}

double dotProd(TVector a, TVector b)
{
    return (a.x * b.x) + (a.y * b.y);
}

TVector neg(TVector in)
{
    TVector _result;
    _result.x = -in.x;
    _result.y = -in.y;
    return _result;
}

double sign(TVector in)
{
    if ((in.x + in.y) > 0)
        return 1;
    else
        return -1;
}

double projectPointOnAxis(TVector point, TVector ax1, TVector ax2,
                          TVector * projectedPoint)
{
    // Calculate unitary vector of axis
    double _mod;
    TVector _unitary, _projection;
    _mod = mod(sub(ax2, ax1));
    if(_mod == 0)
        return -1;
    _unitary = divScalar(sub(ax2, ax1), _mod);

    // Calculate projection on axis
    _projection = sub(ax1, prodScalar(_unitary, dotProd(sub(ax1, point) , _unitary)));
    _projection = sub(_projection, ax1);

    if (projectedPoint != NULL)
    {
        projectedPoint->x = _projection.x;
        projectedPoint->y = _projection.y;
    }

    return mod(_projection) * sign(_projection);
}
