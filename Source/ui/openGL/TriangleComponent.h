#ifndef __TriangleComponent_H__
#define __TriangleComponent_H__

#include "JuceHeader.h"

#include "RAIIOpenGLRenderer.h"

class TriangleComponent : public Component
                        , public OpenGLRenderer
{
public:
    TriangleComponent();
    ~TriangleComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    RAIIOpenGLRenderer raiiOpenGLRenderer;
    bool initialized;

    ScopedPointer<OpenGLShaderProgram> shaderProgram;
};

#endif