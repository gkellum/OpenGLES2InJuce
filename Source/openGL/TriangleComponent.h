#ifndef __TriangleComponent_H__
#define __TriangleComponent_H__

#include "JuceHeader.h"

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
    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
};

#endif