#ifndef __TextToTextureComponent_H__
#define __TextToTextureComponent_H__

#include "JuceHeader.h"

#include <map>

class TextToTextureComponent : public Component
                             , public OpenGLRenderer
{
public:
    TextToTextureComponent();
    ~TextToTextureComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    GLuint createTextureForTextUsingJuceTextRenderer(const String& textString, Rectangle<float>& outBounds);
    String getNextNumberString();

    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> textureCoordinateAttribute;
    ScopedPointer<OpenGLShaderProgram::Uniform> samplerLocationUniform;
    GLuint explanationTextTextureId;
    Rectangle<float> explanationTextTextureBounds;
    GLuint numberTextTextureId;
    Rectangle<float> numberTextTextureBounds;
    int iteration;
};

#endif