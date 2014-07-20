#ifndef __TexturedComponent_H__
#define __TexturedComponent_H__

#include "JuceHeader.h"

#include "RAIIOpenGLRenderer.h"

class TexturedComponent : public Component
                        , public OpenGLRenderer
{
public:
    TexturedComponent();
    ~TexturedComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    GLuint createTextureForImageFile(char *fileName);

    RAIIOpenGLRenderer raiiOpenGLRenderer;
    bool initialized;

    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> textureCoordinateAttribute;
    ScopedPointer<OpenGLShaderProgram::Uniform> samplerLocationUniform;
    GLuint juceLogoTextureId;
};

#endif