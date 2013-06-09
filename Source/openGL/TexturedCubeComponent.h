#ifndef __TexturedCubeComponent_H__
#define __TexturedCubeComponent_H__

#include "JuceHeader.h"

class TexturedCubeComponent : public Component
                            , public OpenGLRenderer
{
public:
    TexturedCubeComponent();
    ~TexturedCubeComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    GLuint createTextureForImageFile( char *fileName );

    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> sourceColorAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> textureCoordinatesAttribute;
    ScopedPointer<OpenGLShaderProgram::Uniform> projectionUniform;
    ScopedPointer<OpenGLShaderProgram::Uniform> modelViewUniform;
    ScopedPointer<OpenGLShaderProgram::Uniform> textureUniform;;
    GLuint gridTextureId;
    GLuint juceLogoTextureId;
    GLuint cubesVerticesId;
    GLuint cubesIndicesId;
    GLuint juceLogosVerticesId;
    GLuint juceLogosIndicesId;
};

#endif