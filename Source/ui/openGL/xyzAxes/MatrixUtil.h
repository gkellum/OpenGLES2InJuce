#ifndef MatrixUtil_H
#define MatrixUtil_H

#include "JuceHeader.h"

#include "MatrixClasses.h"

#include <string>

class MatrixUtil
{
public:
    static std::string toString(const Matrix3D<float>& matrix);
    static std::string toString(const c4::Matrix4D& matrix);
    static std::string toString(const c4::Point3D& point);
    static std::string toString(const c4::Vector3D& point);
    static std::string toString(const c4::Vector4D& point);

    static bool equals(const c4::Matrix4D& c4Matrix, const Matrix3D<float>& juceMatrix);
    static bool equals(const Matrix3D<float>& juceMatrix, const c4::Matrix4D& c4Matrix);
};

#endif