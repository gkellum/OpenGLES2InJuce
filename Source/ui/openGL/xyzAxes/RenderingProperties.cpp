#include "RenderingProperties.h"

#include <cmath>

RenderingProperties::RenderingProperties(int screenWidthInPixels, int screenHeightInPixels)
  : NearPlane(1.0f)
  , FarPlane(100.0f)
  , ViewAngle(45.0f)
  , ZoomFactor(1.0f)
  , screenWidthInPixels(screenWidthInPixels)
  , screenHeightInPixels(screenHeightInPixels)
  , slicesPerRing(24)
  , numberOfRings(8)
  , labelHeight(0.15f)
  , alpha(1.0f)
{
    double fovy = ViewAngle * ZoomFactor;
    double aspect = double(screenWidthInPixels) / double(screenHeightInPixels);
    const double ymax = NearPlane * tan (fovy * double_Pi / 360.0);
    const double ymin = -ymax;
    leftClippingPlane = ymin * aspect;
    rightClippingPlane = ymax * aspect;
    bottomClippingPlane = ymin;
    topClippingPlane = ymax;

    viewMatrix = c4::Matrix4D( c4::Vector3D(0.0f, 0.0f, -15.0f) );

    projectionMatrix = c4::Matrix4D(leftClippingPlane,
                                    rightClippingPlane,
                                    bottomClippingPlane,
                                    topClippingPlane,
                                    NearPlane,
                                    FarPlane );
}

c4::Matrix4D RenderingProperties::getViewMatrix() const
{
    return viewMatrix;
}

void RenderingProperties::setViewMatrix(const c4::Matrix4D viewMatrix)
{
    this->viewMatrix = viewMatrix;
}

c4::Matrix4D RenderingProperties::getProjectionMatrix() const
{
    return projectionMatrix;
}



