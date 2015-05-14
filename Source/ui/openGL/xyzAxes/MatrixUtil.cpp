#include "MatrixUtil.h"

#include <sstream>

std::string MatrixUtil::toString(const c4::Matrix4D& matrix)
{
    float* data = (float*) matrix.n;

    std::stringstream ss;
    ss << "Matrix [" << std::endl;
    ss << "\t\t" << data[0] << ", "  << data[1] << ", "  << data[2] << ", "  << data[3] << std::endl;
    ss << "\t\t" << data[4] << ", "  << data[5] << ", "  << data[6] << ", "  << data[7] << std::endl;
    ss << "\t\t" << data[8] << ", "  << data[9] << ", "  << data[10] << ", " << data[11] << std::endl;
    ss << "\t\t" << data[12] << ", " << data[13] << ", " << data[14] << ", " << data[15] << std::endl;
    ss << "]";

    return ss.str();
}

std::string MatrixUtil::toString(const Matrix3D<float>& matrix)
{
    float* data = (float*) matrix.mat;

    std::stringstream ss;
    ss << "Matrix [" << std::endl;
    ss << "\t\t" << data[0] << ", "  << data[1] << ", "  << data[2] << ", "  << data[3] << std::endl;
    ss << "\t\t" << data[4] << ", "  << data[5] << ", "  << data[6] << ", "  << data[7] << std::endl;
    ss << "\t\t" << data[8] << ", "  << data[9] << ", "  << data[10] << ", " << data[11] << std::endl;
    ss << "\t\t" << data[12] << ", " << data[13] << ", " << data[14] << ", " << data[15] << std::endl;
    ss << "]";

    return ss.str();
}

std::string MatrixUtil::toString(const c4::Point3D& point)
{
    std::stringstream ss;
    ss << "Point (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
    return ss.str();
}

std::string MatrixUtil::toString(const c4::Vector3D& vector)
{
    std::stringstream ss;
    ss << "Vector (" << vector.x << ", " << vector.y << ", " << vector.z << ")" << std::endl;
    return ss.str();
}

std::string MatrixUtil::toString(const c4::Vector4D& vector)
{
    std::stringstream ss;
    ss << "Vector (" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")" << std::endl;
    return ss.str();
}

bool MatrixUtil::equals(const Matrix3D<float>& juceMatrix, const c4::Matrix4D& c4Matrix)
{
    return equals(c4Matrix, juceMatrix);
}

bool MatrixUtil::equals(const c4::Matrix4D& c4Matrix, const Matrix3D<float>& juceMatrix)
{
    float* c4Data = (float*) c4Matrix.n;
    float* juceData = (float*) juceMatrix.mat;

    return     fabs(c4Data[0]  - juceData[0]) < 0.01f
            && fabs(c4Data[1]  - juceData[1]) < 0.01f
            && fabs(c4Data[2]  - juceData[2]) < 0.01f
            && fabs(c4Data[3]  - juceData[3]) < 0.01f
            && fabs(c4Data[4]  - juceData[4]) < 0.01f
            && fabs(c4Data[5]  - juceData[5]) < 0.01f
            && fabs(c4Data[6]  - juceData[6]) < 0.01f
            && fabs(c4Data[7]  - juceData[7]) < 0.01f
            && fabs(c4Data[8]  - juceData[8]) < 0.01f
            && fabs(c4Data[9]  - juceData[9]) < 0.01f
            && fabs(c4Data[10] - juceData[10]) < 0.01f
            && fabs(c4Data[11] - juceData[11]) < 0.01f
            && fabs(c4Data[12] - juceData[12]) < 0.01f
            && fabs(c4Data[13] - juceData[13]) < 0.01f
            && fabs(c4Data[14] - juceData[14]) < 0.01f
            && fabs(c4Data[15] - juceData[15]) < 0.01f;
}
