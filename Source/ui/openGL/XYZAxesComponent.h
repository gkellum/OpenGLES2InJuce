#ifndef __XYZAxesComponent_H__
#define __XYZAxesComponent_H__

#include "JuceHeader.h"

#include "RAIIOpenGLRenderer.h"
#include "GraphicObject.h"
#include "RenderingProperties.h"

class XYZAxesComponent : public Component
                       , public OpenGLRenderer
{
public:
    XYZAxesComponent();
    ~XYZAxesComponent();

    void mouseDown(const MouseEvent& event) override;
    void mouseDrag(const MouseEvent& event) override;
    void mouseMagnify(const MouseEvent &event, float scaleFactor) override;
    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    void createAxesAndPositionLabelsForCameraPosition();

    RAIIOpenGLRenderer raiiOpenGLRenderer;
    OpenGLContext& openGLContext;
    bool initialized;

    RenderingProperties::Ptr renderingProperties;

    GraphicObject::Ptr xAxis;
    GraphicObject::Ptr yAxis;
    GraphicObject::Ptr zAxis;

    std::vector<GraphicObject::Ptr> positionLabels;

    bool cameraMoved;
    c4::Matrix4D viewMatrixAtMouseDown;
};

#endif