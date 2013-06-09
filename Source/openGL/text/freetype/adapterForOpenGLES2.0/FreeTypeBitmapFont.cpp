#include "FreeTypeBitmapFont.h"

#include "FontAtlas.h"
#include "FreeTypeBitmapChar.h"
#include "FreeTypeAmalgam.h"
#include "OpenGLEnvironment.h"
#include "GLCallBatcher.h"

using namespace std;

FreeTypeBitmapFont::FreeTypeBitmapFont(FontAtlasWeakPtr pFontAtlas) 
  : m_pFontAtlas(pFontAtlas)
  , m_pFreeTypeFace(NULL)
{
}


FreeTypeBitmapFont::~FreeTypeBitmapFont()
{
    ReleaseFace();
}

void FreeTypeBitmapFont::ReleaseFace()
{
    if (m_pFreeTypeFace) 
    {
        FT_Done_Face(m_pFreeTypeFace);
        m_pFreeTypeFace = NULL;
    }
}

int FreeTypeBitmapFont::GetWidth(const char* text) 
{
    int currX = 0;
    int n = 0;
    unsigned char c;
    std::map<int, FreeTypeBitmapCharPtr>::const_iterator iter;
    FT_Vector kerning;
    FT_UInt ixGlyph;
    FT_UInt ixGlyphPrev = 0;
    bool hasKerning = false;
    if (m_pFreeTypeFace)
    {
        hasKerning = FT_HAS_KERNING(m_pFreeTypeFace); 
    }
    while (text[n] != 0)
    {
        c = text[n];
        iter = m_mapBitmapChar.find(c);
        if (iter != m_mapBitmapChar.end()) 
        {
            if (hasKerning)
            {
                // get kerning
                ixGlyph = FT_Get_Char_Index(m_pFreeTypeFace, c); 
                if (hasKerning && ixGlyphPrev && ixGlyph) 
                { 
                    FT_Get_Kerning(m_pFreeTypeFace, ixGlyphPrev, ixGlyph, FT_KERNING_DEFAULT, &kerning); 
                    currX += kerning.x >> 6; 
                } 
                ixGlyphPrev = ixGlyph;
            }
            currX += iter->second->GetXAdvance();
        }
        n++;
    } 
    return currX;
}   

int FreeTypeBitmapFont::GetCharWidth(const string& text, int index) 
{
    //if (index < (int)text.size())
    //  return m_hSpacing + m_chars[text[index]].GetXAdvance(&m_chars[text[index+1]]);
    //return m_hSpacing + m_chars[text[index]].GetXAdvance();
    return 0;
}   

int FreeTypeBitmapFont::drawString(OpenGLEnvironment& environment, int x, int y, const char* text, int color, float alpha) 
{
    int textureId = -1;
    FontAtlasPtr pFontAtlas = m_pFontAtlas.lock();
    if (pFontAtlas != NULL)
        textureId = pFontAtlas->getTextureID();
    environment.getCallBatcher().setAttributes(environment, textureId, color, alpha, true, false);
    unsigned char c;
    int currX = x;
    int n = 0;
    std::map<int, FreeTypeBitmapCharPtr>::const_iterator iter;
    FT_Vector kerning;
    FT_UInt ixGlyph;
    FT_UInt ixGlyphPrev = 0;
    bool hasKerning = false;
    if (m_pFreeTypeFace)
    {
        hasKerning = FT_HAS_KERNING(m_pFreeTypeFace); 
    }
    while (text[n] != 0)
    {
        c = text[n];
        iter = m_mapBitmapChar.find(c);
        if (iter != m_mapBitmapChar.end()) 
        {
            if (hasKerning)
            {
                // get kerning
                ixGlyph = FT_Get_Char_Index(m_pFreeTypeFace, c); 
                if (hasKerning && ixGlyphPrev && ixGlyph) 
                { 
                    FT_Get_Kerning(m_pFreeTypeFace, ixGlyphPrev, ixGlyph, FT_KERNING_DEFAULT, &kerning); 
                    currX += kerning.x >> 6; 
                } 
                ixGlyphPrev = ixGlyph;
            }
            iter->second->render(environment, currX, y);
            currX += iter->second->GetXAdvance();
        }
        n++;
    } 
    return currX;
}   

int FreeTypeBitmapFont::DrawStringShadow(int x, int y, const char* text, int color, int backColor) 
{
    DrawString(x+2, y+2, text, backColor, 1.0f); 
    return DrawString(x, y, text, color, 1.0f); 
}   

void FreeTypeBitmapFont::FinishCreating()
{
    bool hasKerning = false;
    if (m_pFreeTypeFace)
    {
        hasKerning = FT_HAS_KERNING(m_pFreeTypeFace); 
    }
    FontAtlasPtr pFontAtlas = m_pFontAtlas.lock();
    if (!hasKerning || (pFontAtlas != NULL && !pFontAtlas->getUseKerning()))
        ReleaseFace();
}


