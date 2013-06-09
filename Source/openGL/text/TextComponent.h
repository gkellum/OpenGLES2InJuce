#ifndef __TextComponent_H__
#define __TextComponent_H__

#include "JuceHeader.h"

#include "FreeTypeAmalgam.h"
#include "OpenGLEnvironmentImpl.h"

#include <map>

class TextComponent : public Component
                        , public OpenGLRenderer
{
public:
    TextComponent();
    ~TextComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    GLuint createTextureForTextUsingJuceTextRenderer(const char* textString);
    void createFreeTypeFontAtlas();

    struct OpenGLGlyphTextureInfo;
    std::map<int, OpenGLGlyphTextureInfo> createBitmapFontUsingJuceTextRenderer();
    std::map<int, OpenGLGlyphTextureInfo> asciiCodeToTextureInfoMap;
    void drawTextWithBitmapFont(const String& message, int x, int y);

    FT_Library library;
    FT_Face face;

    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> textureCoordinateAttribute;
    ScopedPointer<OpenGLShaderProgram::Uniform> samplerLocationUniform;
    GLuint juceLogoTextureId;
    ScopedPointer<Image> imageForText;
    ScopedPointer<OpenGLEnvironmentImpl> environment;
};

#endif