#ifndef __BoundingBox_H__
#define __BoundingBox_H__

#include "Dimension.h"
#include "MatrixClasses.h"
#include "VectorClasses.h"

#include <string>
#include <utility>

struct BoundingBox
{
    BoundingBox();

    BoundingBox(const BoundingBox& otherBox); 

    BoundingBox(float minX, float maxX, 
                float minY, float maxY, 
                float minZ, float maxZ);

    BoundingBox(const c4::Vector3D& minimumPoint, const c4::Vector3D& maximumPoint);

    BoundingBox(std::pair<float, float> cartesianLocation, 
                float radius, 
                std::pair<float, float> zRange);

    std::pair<float, float> getCenterOfBase(Dimension axisOfOrientation) const;
    c4::Vector3D getCenter() const;

    float getRadius(Dimension axisOfOrientation) const;

    float getMin(Dimension dimension) const;
    float getMax(Dimension dimension) const;

    float getDistance(Dimension dimension) const;

    BoundingBox getSectionOfBoundingBoxAlongAxisOfOrientation(const std::pair<float, float>& beginningAndEndOfSection, Dimension axisOfOrientation) const;

    //std::triple<float, float, float> denormalize(const std::triple<float, float, float>& point);
    c4::Vector3D denormalize(c4::Vector3D& point);
    BoundingBox denormalize(BoundingBox boundingBox);

    std::string toString() const;

    BoundingBox& operator -=(const c4::Vector3D& aPoint) 
    {
        minimumPoint -= aPoint;
        maximumPoint -= aPoint;
        return *this;
    }

    c4::Vector3D minimumPoint;
    c4::Vector3D maximumPoint;
};

#endif