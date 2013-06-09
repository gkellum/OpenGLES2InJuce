#ifndef __TextWithFreetypeFontAtlasComponent_H__
#define __TextWithFreetypeFontAtlasComponent_H__

#include "JuceHeader.h"

#include "FreeTypeAmalgam.h"
#include "OpenGLEnvironmentImpl.h"

#include <map>

class TextWithFreetypeFontAtlasComponent : public Component
                                         , public OpenGLRenderer
{
public:
    TextWithFreetypeFontAtlasComponent();
    ~TextWithFreetypeFontAtlasComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    void createFreeTypeFontAtlas();

    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> textureCoordinateAttribute;
    ScopedPointer<OpenGLShaderProgram::Uniform> samplerLocationUniform;
    ScopedPointer<OpenGLEnvironmentImpl> environment;
};

#endif