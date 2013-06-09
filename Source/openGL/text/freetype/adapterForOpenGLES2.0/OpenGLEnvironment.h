#ifndef __OpenGLEnvironment_H__
#define __OpenGLEnvironment_H__

#include <memory>
#if OS_MACOSX || OS_MACIOS || OS_ANDROID
#include <tr1/memory>
#endif

class GLCallBatcher;
class FontAtlas;
typedef std::tr1::shared_ptr<FontAtlas> FontAtlasPtr;
class FreeTypeBitmapFont;
typedef std::tr1::shared_ptr<FreeTypeBitmapFont> FreeTypeBitmapFontPtr;

class OpenGLEnvironment
{
public:
    virtual ~OpenGLEnvironment() {}

    virtual int getCanvasWidth() const = 0;
    virtual int getCanvasHeight() const =0;

    virtual GLCallBatcher& getCallBatcher() =0;

    virtual FontAtlasPtr getFontAtlas() const =0;

    virtual FreeTypeBitmapFontPtr getFreeTypeFont(int index) =0;
};

#endif