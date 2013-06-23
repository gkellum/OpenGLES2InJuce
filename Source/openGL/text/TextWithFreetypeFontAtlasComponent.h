#ifndef __TextWithFreetypeFontAtlasComponent_H__
#define __TextWithFreetypeFontAtlasComponent_H__

#include "JuceHeader.h"

#include "FreeTypeAmalgam.h"
#include "OpenGLEnvironmentImpl.h"

#include <map>

/**
 * This Freetype component has never worked correctly. I gave up on working out the
 * kinks in it after seeing how fast the Juce text rendering already is.
 */
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