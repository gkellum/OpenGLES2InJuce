#ifndef __RenderingProperties_H__
#define __RenderingProperties_H__

#include "JuceHeader.h"

#include "MatrixClasses.h"

#include <memory>
#if defined(OS_MACOSX) || defined(OS_MACIOS) || defined(OS_ANDROID)
  #if !_LIBCPP_VERSION
    #include <tr1/memory>
  #endif
#endif

struct RenderingProperties
{
#if !_LIBCPP_VERSION
    typedef std::tr1::shared_ptr<RenderingProperties> Ptr;
#else
    typedef std::shared_ptr<RenderingProperties> Ptr;
#endif

    RenderingProperties(int screenWidthInPixels, int screenHeightInPixels);

    c4::Matrix4D getViewMatrix() const;
    void setViewMatrix(const c4::Matrix4D viewMatrix);

    c4::Matrix4D getProjectionMatrix() const;

    enum ZPosition
    {
        CenterZPosition,
        MinimumZPosition,
        MaximumZPosition
    };

    const float NearPlane;
    const float FarPlane;
    const float ViewAngle;
    const float ZoomFactor;

    float leftClippingPlane;
    float rightClippingPlane;
    float bottomClippingPlane;
    float topClippingPlane;

    c4::Matrix4D viewMatrix;
    c4::Matrix4D projectionMatrix;

    int screenWidthInPixels;
    int screenHeightInPixels;
    int slicesPerRing;
    int numberOfRings;

    float labelHeight;
    float alpha;
};

#endif