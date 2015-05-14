//============================================================================
//
// 3D/4D Matrix Classes
//
// Mathematics for 3D Game Programming and Computer Graphics, 3rd ed.
// By Eric Lengyel
//
// The code in this file may be freely used in any software. It is provided
// as-is, with no warranty of any kind.
//
//============================================================================

#ifndef _MatrixClasses_H_
#define _MatrixClasses_H_

#include "JuceHeader.h"

#include "VectorClasses.h"

namespace c4
{

class Matrix3D
{
public:
    float n[3][3];

    Matrix3D() {}
    
    Matrix3D(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3);
    Matrix3D(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22);
    
    Matrix3D& set(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3);
    Matrix3D& set(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22);
    
    float& operator ()(long i, long j)
    {
        return (n[j][i]);
    }
    
    const float& operator ()(long i, long j) const
    {
        return (n[j][i]);
    }
    
    Vector3D& operator [](long j)
    {
        return (*reinterpret_cast<Vector3D *>(n[j]));
    }
    
    const Vector3D& operator [](long j) const
    {
        return (*reinterpret_cast<const Vector3D *>(n[j]));
    }
    
    Vector3D getRow(long i) const
    {
        return (Vector3D(n[0][i], n[1][i], n[2][i]));
    }
    
    Matrix3D& setRow(long i, const Vector3D& row)
    {
        n[0][i] = row.x;
        n[1][i] = row.y;
        n[2][i] = row.z;
        return (*this);
    }
    
    Matrix3D& operator *=(const Matrix3D& m);
    Matrix3D& operator *=(float t);
    Matrix3D& operator /=(float t);
    
    Matrix3D& setIdentity(void);
    
    friend Matrix3D operator *(const Matrix3D& m1, const Matrix3D& m2);
    friend Matrix3D operator *(const Matrix3D& m, float t);
    friend Matrix3D operator /(const Matrix3D& m, float t);
    friend Vector3D operator *(const Matrix3D& m, const Vector3D& v);
    friend Vector3D operator *(const Matrix3D& m, const Point3D& p);
    friend Vector3D operator *(const Vector3D& v, const Matrix3D& m);
    friend Vector3D operator *(const Point3D& p, const Matrix3D& m);
    friend bool operator ==(const Matrix3D& m1, const Matrix3D& m2);
    friend bool operator !=(const Matrix3D& m1, const Matrix3D& m2);
};

inline Matrix3D operator *(float t, const Matrix3D& m)
{
    return (m * t);
}

float Determinant(const Matrix3D& m);
Matrix3D Inverse(const Matrix3D& m);
Matrix3D Adjugate(const Matrix3D& m);
Matrix3D Transpose(const Matrix3D& m);

class Matrix4D
{
public:
    float n[4][4];

    Matrix4D() {}
    
    Matrix4D(const Vector4D& c1, const Vector4D& c2, const Vector4D& c3, const Vector4D& c4);
    Matrix4D(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33);
    Matrix4D(const Vector3D& translationVector);
    Matrix4D(const Vector4D& translationVector);
    Matrix4D(float frustumLeft,
             float frustumRight,
             float frustumBottom,
             float frustumTop,
             float frustumNearDistance,
             float frustumFarDistance);

    Matrix4D& set(const Vector4D& c1, const Vector4D& c2, const Vector4D& c3, const Vector4D& c4);
    Matrix4D& set(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33);
    
    float& operator ()(long i, long j)
    {
        return (n[j][i]);
    }
    
    const float& operator ()(long i, long j) const
    {
        return (n[j][i]);
    }
    
    Vector4D& operator [](long j)
    {
        return (*reinterpret_cast<Vector4D *>(n[j]));
    }
    
    const Vector4D& operator [](long j) const
    {
        return (*reinterpret_cast<const Vector4D *>(n[j]));
    }
    
    Vector4D getRow(long i) const
    {
        return (Vector4D(n[0][i], n[1][i], n[2][i], n[3][i]));
    }
    
    Matrix4D& setRow(long i, const Vector3D& row)
    {
        n[0][i] = row.x;
        n[1][i] = row.y;
        n[2][i] = row.z;
        return (*this);
    }
    
    Matrix4D& setRow(long i, const Vector4D& row)
    {
        n[0][i] = row.x;
        n[1][i] = row.y;
        n[2][i] = row.z;
        n[3][i] = row.w;
        return (*this);
    }
    
    Matrix4D& operator =(const Matrix3D& m);
    Matrix4D& operator *=(const Matrix4D& m);
    Matrix4D& operator *=(const Matrix3D& m);
    
    Matrix4D& setIdentity(void);
    
    friend Matrix4D operator *(const Matrix4D& m1, const Matrix4D& m2);
    friend Matrix4D operator *(const Matrix4D& m1, const Matrix3D& m2);
    friend Vector4D operator *(const Matrix4D& m, const Vector4D& v);
    friend Vector4D operator *(const Vector4D& v, const Matrix4D& m);
    friend Vector4D operator *(const Matrix4D& m, const Vector3D& v);
    friend Vector4D operator *(const Vector3D& v, const Matrix4D& m);
    friend Vector4D operator *(const Matrix4D& m, const Point3D& p);
    friend Vector4D operator *(const Point3D& p, const Matrix4D& m);
    friend Vector4D operator *(const Matrix4D& m, const Vector2D& v);
    friend Vector4D operator *(const Vector2D& v, const Matrix4D& m);
    friend Vector4D operator *(const Matrix4D& m, const Point2D& p);
    friend Vector4D operator *(const Point2D& p, const Matrix4D& m);
    friend bool operator ==(const Matrix4D& m1, const Matrix4D& m2);
    friend bool operator !=(const Matrix4D& m1, const Matrix4D& m2);

    void fromJuceMatrix(const juce::Matrix3D<GLfloat>& juceMatrix);
    juce::Matrix3D<GLfloat> toJuceMatrix() const;
};

float Determinant(const Matrix4D& m);
Matrix4D Inverse(const Matrix4D& m);
Matrix4D Adjugate(const Matrix4D& m);
Matrix4D Transpose(const Matrix4D& m);

} // end namespace c4

#endif

