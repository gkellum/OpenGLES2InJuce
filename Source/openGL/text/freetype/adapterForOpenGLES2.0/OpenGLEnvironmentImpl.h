#ifndef _OpenGLEnvironmentImpl_H_INCLUDED_
#define _OpenGLEnvironmentImpl_H_INCLUDED_

#include "GLCallBatcher.h"
#include "OpenGLEnvironment.h"

#include <memory>
#if OS_MACOSX || OS_MACIOS || OS_ANDROID
#include <tr1/memory>
#endif

class FontAtlas;
typedef std::tr1::shared_ptr<FontAtlas> FontAtlasPtr;
class FreeTypeBitmapFont;
typedef std::tr1::shared_ptr<FreeTypeBitmapFont> FreeTypeBitmapFontPtr;

class OpenGLEnvironmentImpl : public OpenGLEnvironment
{
public:
    OpenGLEnvironmentImpl(FontAtlasPtr m_pFontAtlas, int canvasWidth, int canvasHeight);
    ~OpenGLEnvironmentImpl();

    int getCanvasWidth() const { return m_screenWidth; }
    int getCanvasHeight() const { return m_screenHeight; }

    FreeTypeBitmapFontPtr getFreeTypeFont(int index);

    GLCallBatcher& getCallBatcher() { return m_batcher; }

    FontAtlasPtr getFontAtlas() const { return m_pFontAtlas; }

private:
    FontAtlasPtr m_pFontAtlas;
    int m_screenWidth;
    int m_screenHeight;
    GLCallBatcher m_batcher;
};

#endif // _OpenGLEnvironmentImpl_H_INCLUDED_
