#ifndef __PointSpriteComponent_H__
#define __PointSpriteComponent_H__

#include "JuceHeader.h"

#include "RAIIOpenGLRenderer.h"

class PointSpriteComponent : public Component
                           , public OpenGLRenderer
{
public:
    PointSpriteComponent();
    ~PointSpriteComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    RAIIOpenGLRenderer raiiOpenGLRenderer;
    bool initialized;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> sourceColorAttribute;
    ScopedPointer<OpenGLShaderProgram::Uniform> samplerLocationUniform;
    GLuint juceLogoTextureId;
};

#endif