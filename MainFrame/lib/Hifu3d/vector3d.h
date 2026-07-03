

// vector3d 3d定义头文件

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

class vector3d
{
public:

    float x;
    float y;
    float z;

    vector3d();
    vector3d(float xv, float yv, float zv);
    vector3d(const vector3d & v);

    void SetZero();
    void Set(float xv, float yv, float zv);
    void Normalise();

    vector3d Normalised() const;

    vector3d Orthogonal2DA();
    vector3d Orthogonal2DB();

    float Magnitude()	const;
    float MagnitudeSquared() const;

    vector3d operator /=(float f);
    vector3d operator -	() const;
    vector3d operator -  (const vector3d &v) const;
    vector3d operator +  (const vector3d &v) const;

    vector3d CROSS(const vector3d& p);
    float DOT(const vector3d& p);

    void operator += (const vector3d &v);
    void operator -= (const vector3d &v);
    void operator *= (float s);

    bool operator == (const vector3d &other) const;
    bool operator != (const vector3d &other) const;

    float &	operator[](int index);
    const float & operator[](int index) const;

};

inline vector3d::vector3d() : x(0.0f), y(0.0f), z(0.0f)
{
}

inline vector3d::vector3d(float xv, float yv, float zv) : x(xv), y(yv), z(zv)
{
}

inline vector3d::vector3d(const vector3d & v) : x(v.x), y(v.y), z(v.z)
{
}

inline bool	vector3d::operator==(const vector3d &other) const
{
    return (x == other.x) && (y == other.y) && (z == other.z);
}

inline bool	vector3d::operator!=(const vector3d &other) const
{
    return (x != other.x) || (y != other.y) || (z != other.z);
}

inline float&	vector3d::operator[](int index)
{
    return reinterpret_cast<float*>(this)[index];
}

inline const float&	vector3d::operator[](int index) const
{
    return reinterpret_cast<const float*>(this)[index];
}

float		operator * (const vector3d &a, const vector3d &b);
vector3d	operator * (const vector3d &v, float s);
vector3d	operator * (float s, const vector3d &v);

vector3d vector3dScale(const vector3d &a, const vector3d &b);

float vector3dDot(const vector3d &a, const vector3d &b);

vector3d vector3dCross(const vector3d &a, const vector3d &b);

vector3d vector3dRotate(vector3d p, const vector3d &p1, const vector3d &p2, float theta);

vector3d Interpolate(float t, const vector3d &a, const vector3d &b);

float mlDistanceBetween(const vector3d & a, const vector3d & b);

extern const vector3d vector3dX;
extern const vector3d vector3dY;
extern const vector3d vector3dZ;
extern const vector3d vector3dZero;

#endif // VECTOR3D_H_
