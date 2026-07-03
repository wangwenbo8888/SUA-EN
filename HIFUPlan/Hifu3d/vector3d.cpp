
#include "vector3d.h"

#include <math.h>

vector3d vector3d::Orthogonal2DA()
{
    return vector3d(y, -x, 0);
}

vector3d vector3d::Orthogonal2DB()
{
    return vector3d(-y, x, 0);
}

float vector3dCross2D(const vector3d &a, const vector3d &b)
{
    return a.x * b.y - a.y * b.x;
}

extern const vector3d vector3dX(1.0f, 0.0f, 0.0f);
extern const vector3d vector3dY(0.0f, 1.0f, 0.0f);
extern const vector3d vector3dZ(0.0f, 0.0f, 1.0f);

extern const vector3d vector3dZero(0.0f, 0.0f, 0.0f);

void vector3d::SetZero()					{ x = y = z = 0.0f; }
void vector3d::Set(float xv, float yv, float zv)	{ x = xv; y = yv; z = zv; }

float vector3d::Magnitude()	const				{ return sqrt(x * x + y * y + z * z); }
float vector3d::MagnitudeSquared()	const			{ return x * x + y * y + z * z; }

void vector3d::Normalise()
{
    float mag = Magnitude();

    if (mag == 0.0f)
        return;

    float invMag = 1.0f / mag;
    x /= mag;
    y /= mag;
    z /= mag;
}

vector3d	vector3d::Normalised() const
{
    vector3d normalisedvector3d(x, y, z);

    float maxValue = 0.0f;

    if (fabs(normalisedvector3d.x) > maxValue) maxValue = fabs(normalisedvector3d.x);
    if (fabs(normalisedvector3d.y) > maxValue) maxValue = fabs(normalisedvector3d.y);
    if (fabs(normalisedvector3d.z) > maxValue) maxValue = fabs(normalisedvector3d.z);

    if (maxValue == 0.0f)
        return vector3d(x, y, z);

    normalisedvector3d.x = normalisedvector3d.x / maxValue;
    normalisedvector3d.y = normalisedvector3d.y / maxValue;
    normalisedvector3d.z = normalisedvector3d.z / maxValue;

    float mag = normalisedvector3d.Magnitude();

    if (mag == 0.0f)
        return vector3d(x, y, z);

    float invMag = 1.0f / mag;
    normalisedvector3d.x *= invMag;
    normalisedvector3d.y *= invMag;
    normalisedvector3d.z *= invMag;

    return normalisedvector3d;
}

vector3d	vector3d::operator +  (const vector3d &v) const	{ return vector3d(x + v.x, y + v.y, z + v.z); }
vector3d	vector3d::operator -  (const vector3d &v) const	{ return vector3d(x - v.x, y - v.y, z - v.z); }
void		vector3d::operator += (const vector3d &v)		{ x += v.x; y += v.y; z += v.z; }
void		vector3d::operator -= (const vector3d &v)		{ x -= v.x; y -= v.y; z -= v.z; }
void		vector3d::operator *= (float s)			{ x *= s; y *= s; z *= s; }
vector3d	vector3d::operator -	() const			{ return vector3d(-x, -y, -z); }

vector3d vector3d::operator /= (float f)
{
    x /= f;
    y /= f;
    z /= f;

    return vector3d(x,y,z);
}

float		operator * (const vector3d &a, const vector3d &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vector3dDot(const vector3d &a, const vector3d &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector3d vector3d::CROSS(const vector3d& p)
{
    return vector3d(y*p.z-p.y*z,
                    z*p.x-x*p.z,
                    x*p.y-p.x*y);
}

float vector3d::DOT(const vector3d& p)
{
    return x*p.x+y*p.y+z*p.z;
}

vector3d	operator * (const vector3d &v, float s)			{ return vector3d(v.x * s, v.y * s, v.z * s); }
vector3d	operator * (float s, const vector3d &v)			{ return vector3d(v.x * s, v.y * s, v.z * s); }

vector3d	vector3dCross(const vector3d &a, const vector3d &b)
{
    return vector3d(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
        );
}

vector3d vector3dScale(const vector3d &a, const vector3d &b)
{
    return vector3d(
        a.x * b.x,
        a.y * b.y,
        a.z * b.z);
}

vector3d vector3dRotate(vector3d p, const vector3d &p1, const vector3d &p2, float theta)
{
    vector3d q, r;
    float costheta, sintheta;

    r.x = p2.x - p1.x;
    r.y = p2.y - p1.y;
    r.z = p2.z - p1.z;
    p.x -= p1.x;
    p.y -= p1.y;
    p.z -= p1.z;
    r.Normalise();

    costheta = cos(theta);
    sintheta = sin(theta);

    q.x += (costheta + (1 - costheta) * r.x * r.x) * p.x;
    q.x += ((1 - costheta) * r.x * r.y - r.z * sintheta) * p.y;
    q.x += ((1 - costheta) * r.x * r.z + r.y * sintheta) * p.z;

    q.y += ((1 - costheta) * r.x * r.y + r.z * sintheta) * p.x;
    q.y += (costheta + (1 - costheta) * r.y * r.y) * p.y;
    q.y += ((1 - costheta) * r.y * r.z - r.x * sintheta) * p.z;

    q.z += ((1 - costheta) * r.x * r.z - r.y * sintheta) * p.x;
    q.z += ((1 - costheta) * r.y * r.z + r.x * sintheta) * p.y;
    q.z += (costheta + (1 - costheta) * r.z * r.z) * p.z;

    q.x += p1.x;
    q.y += p1.y;
    q.z += p1.z;

    return q;
}

vector3d Interpolate(float t, const vector3d &a, const vector3d &b)
{
    return a * (1.0f - t) + b * t;
}

float mlDistanceBetween(const vector3d & a, const vector3d & b)
{
    vector3d difference = a - b;
    return difference.Magnitude();
}

