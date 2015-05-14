#include "BoundingBox.h"

#include <iostream>
#include <sstream>

using namespace std;

#define LOGGING 0

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(const BoundingBox& otherBox) 
  : minimumPoint(otherBox.minimumPoint)
  , maximumPoint(otherBox.maximumPoint)
{
}

BoundingBox::BoundingBox(float minX, float maxX, 
                         float minY, float maxY, 
                         float minZ, float maxZ)
  : minimumPoint(minX, minY, minZ)
  , maximumPoint(maxX, maxY, maxZ)
{
}

BoundingBox::BoundingBox(const c4::Vector3D& minimumPoint, const c4::Vector3D& maximumPoint)
  : minimumPoint(minimumPoint)
  , maximumPoint(maximumPoint)
{
}

BoundingBox::BoundingBox(std::pair<float, float> cartesianLocation, 
                         float radius, 
                         std::pair<float, float> zRange)
  : minimumPoint(cartesianLocation.first-0.5f*radius, cartesianLocation.second-0.5f*radius, zRange.first)
  , maximumPoint(cartesianLocation.first+0.5f*radius, cartesianLocation.second+0.5f*radius, zRange.second)
{
}

std::pair<float, float> BoundingBox::getCenterOfBase(Dimension axisOfOrientation) const
{
    switch (axisOfOrientation)
    {
    case XAxis:
        return std::pair<float, float>( 0.5f * (minimumPoint.y + maximumPoint.y),
                                        0.5f * (minimumPoint.z + maximumPoint.z));
    case YAxis:
        return std::pair<float, float>( 0.5f * (minimumPoint.x + maximumPoint.x),
                                        0.5f * (minimumPoint.z + maximumPoint.z));
    case ZAxis:
        return std::pair<float, float>( 0.5f * (minimumPoint.x + maximumPoint.x),
                                        0.5f * (minimumPoint.y + maximumPoint.y));
    default:
        return std::pair<float, float>(0, 0);
    }
}

c4::Vector3D BoundingBox::getCenter() const
{
    return c4::Vector3D( 0.5f * (minimumPoint.x + maximumPoint.x),
                         0.5f * (minimumPoint.y + maximumPoint.y),
                         0.5f * (minimumPoint.z + maximumPoint.z) );
}

float BoundingBox::getRadius(Dimension axisOfOrientation) const
{
    float diameterOfFirstDimensionalBasis = 0;
    float diameterOfSecondDimensionalBasis = 0;
    switch (axisOfOrientation)
    {
    case XAxis:
        diameterOfFirstDimensionalBasis = getDistance(YAxis);
        diameterOfSecondDimensionalBasis = getDistance(ZAxis);
        break;
    case YAxis:
        diameterOfFirstDimensionalBasis = getDistance(XAxis);
        diameterOfSecondDimensionalBasis = getDistance(ZAxis);
        break;
    case ZAxis:
        diameterOfFirstDimensionalBasis = getDistance(XAxis);
        diameterOfSecondDimensionalBasis = getDistance(YAxis);
        break;
    default:
        break;
    }
    return 0.5f * std::min(diameterOfFirstDimensionalBasis, diameterOfSecondDimensionalBasis);
}

float BoundingBox::getMin(Dimension dimension) const
{
    switch (dimension)
    {
    case XAxis:
        return minimumPoint.x;
    case YAxis:
        return minimumPoint.y;
    case ZAxis:
        return minimumPoint.z;
    default:
        return 0;
    }
}

float BoundingBox::getMax(Dimension dimension) const
{
    switch (dimension)
    {
    case XAxis:
        return maximumPoint.x;
    case YAxis:
        return maximumPoint.y;
    case ZAxis:
        return maximumPoint.z;
    default:
        return 0;
    }
}

float BoundingBox::getDistance(Dimension dimension) const
{
    switch (dimension)
    {
    case XAxis:
        return maximumPoint.x - minimumPoint.x;
    case YAxis:
        return maximumPoint.y - minimumPoint.y;
    case ZAxis:
        return maximumPoint.z - minimumPoint.z;
    default:
        return 0;
    }
}

BoundingBox BoundingBox::getSectionOfBoundingBoxAlongAxisOfOrientation(const std::pair<float, float>& beginningAndEndOfSection, 
                                                                       Dimension axisOfOrientation) const
{
    c4::Vector3D sectionsMinimumPoint(minimumPoint);
    c4::Vector3D sectionsMaximumPoint(maximumPoint);

    switch (axisOfOrientation)
    {
    case XAxis:
        sectionsMinimumPoint.x = getMin(axisOfOrientation) + beginningAndEndOfSection.first * getDistance(axisOfOrientation);
        sectionsMaximumPoint.x = getMin(axisOfOrientation) + beginningAndEndOfSection.second * getDistance(axisOfOrientation);
        break;
    case YAxis:
        sectionsMinimumPoint.y = getMin(axisOfOrientation) + beginningAndEndOfSection.first * getDistance(axisOfOrientation);
        sectionsMaximumPoint.y = getMin(axisOfOrientation) + beginningAndEndOfSection.second * getDistance(axisOfOrientation);
        break;
    case ZAxis:
        sectionsMinimumPoint.z = getMin(axisOfOrientation) + beginningAndEndOfSection.first * getDistance(axisOfOrientation);
        sectionsMaximumPoint.z = getMin(axisOfOrientation) + beginningAndEndOfSection.second * getDistance(axisOfOrientation);
        break;
    default:
        break;
    }

    return BoundingBox(sectionsMinimumPoint, sectionsMaximumPoint);
}

c4::Vector3D BoundingBox::denormalize(c4::Vector3D& point)
{
    return c4::Vector3D(
                point.x * getDistance(XAxis) + minimumPoint.x,
                point.y * getDistance(YAxis) + minimumPoint.y,
                point.z * getDistance(ZAxis) + minimumPoint.z
                  );
}

BoundingBox BoundingBox::denormalize(BoundingBox boundingBox)
{
    return BoundingBox(boundingBox.minimumPoint.x * getDistance(XAxis) + minimumPoint.x,
                       boundingBox.maximumPoint.x * getDistance(XAxis) + minimumPoint.x,
                       boundingBox.minimumPoint.y * getDistance(YAxis) + minimumPoint.y, 
                       boundingBox.maximumPoint.y * getDistance(YAxis) + minimumPoint.y,
                       boundingBox.minimumPoint.z * getDistance(ZAxis) + minimumPoint.z, 
                       boundingBox.maximumPoint.z * getDistance(ZAxis) + minimumPoint.z);
}

std::string BoundingBox::toString() const
{
    std::stringstream ss;
    ss << "Bound values "
            << "(" << minimumPoint.x << ", " << minimumPoint.y << ", " << minimumPoint.z << ") "
            << "(" << maximumPoint.x << ", " << maximumPoint.y << ", " << maximumPoint.z << ")";
    return ss.str();
}

