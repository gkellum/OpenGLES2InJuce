#ifndef __TextWithJuceFontAtlasComponent_H__
#define __TextWithJuceFontAtlasComponent_H__

#include "JuceHeader.h"

#include "RAIIOpenGLRenderer.h"

#include <map>
#include <memory>

class TextWithJuceFontAtlasComponent : public Component
                                     , public OpenGLRenderer
{
public:
    TextWithJuceFontAtlasComponent();
    ~TextWithJuceFontAtlasComponent() noexcept;

    void newOpenGLContextCreated();
    void openGLContextClosing();

    void renderOpenGL();

private:
    struct OpenGLGlyphTextureInfo;
    typedef std::shared_ptr<OpenGLGlyphTextureInfo> OpenGLGlyphTextureInfoPtr;
    std::map<int, OpenGLGlyphTextureInfoPtr> createBitmapFontUsingJuceTextRenderer();
    std::map<int, OpenGLGlyphTextureInfoPtr> asciiCodeToTextureInfoMap;
    int drawTextWithBitmapFont(const String& message, int x, int y);
    String getNextNumberString();

    RAIIOpenGLRenderer raiiOpenGLRenderer;
    bool initialized;

    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<OpenGLShaderProgram::Attribute> positionAttribute;
    ScopedPointer<OpenGLShaderProgram::Attribute> textureCoordinateAttribute;
    ScopedPointer<OpenGLShaderProgram::Uniform> samplerLocationUniform;
    uint64 iteration;
};

#endif