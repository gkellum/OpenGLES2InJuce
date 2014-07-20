#ifndef __SquareComponent_H__
#define __SquareComponent_H__

#include "JuceHeader.h"

#include "RAIIOpenGLRenderer.h"

class SquareComponent : public Component
                      , public OpenGLRenderer
{
public:
    SquareComponent();
    ~SquareComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    RAIIOpenGLRenderer raiiOpenGLRenderer;
    bool initialized;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> sourceColorAttribute;
};

#endif