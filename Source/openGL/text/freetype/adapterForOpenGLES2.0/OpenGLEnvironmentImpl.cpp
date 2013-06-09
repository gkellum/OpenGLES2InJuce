#include "OpenGLEnvironmentImpl.h"

#include "Util.h"
#include "FontAtlas.h"

OpenGLEnvironmentImpl::OpenGLEnvironmentImpl(FontAtlasPtr pFontAtlas, int canvasWidth, int canvasHeight) 
  : m_pFontAtlas(pFontAtlas)
  , m_screenWidth(canvasWidth)
  , m_screenHeight(canvasHeight)
{
}

OpenGLEnvironmentImpl::~OpenGLEnvironmentImpl()
{
    m_batcher.Release();
}

FreeTypeBitmapFontPtr OpenGLEnvironmentImpl::getFreeTypeFont(int index) 
{ 
    return m_pFontAtlas->getFont(index);
}
