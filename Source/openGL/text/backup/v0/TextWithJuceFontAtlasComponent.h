#ifndef __TextWithJuceFontAtlasComponent_H__
#define __TextWithJuceFontAtlasComponent_H__

#include "JuceHeader.h"

#include <map>

class TextWithJuceFontAtlasComponent : public Component
                                     , public OpenGLRenderer
{
public:
    TextWithJuceFontAtlasComponent();
    ~TextWithJuceFontAtlasComponent();

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    struct OpenGLGlyphTextureInfo;
    std::map<int, OpenGLGlyphTextureInfo> createBitmapFontUsingJuceTextRenderer();
    std::map<int, OpenGLGlyphTextureInfo> asciiCodeToTextureInfoMap;
    void drawTextWithBitmapFont(const String& message, int x, int y);

    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> textureCoordinateAttribute;
    ScopedPointer<OpenGLShaderProgram::Uniform> samplerLocationUniform;
};

#endif