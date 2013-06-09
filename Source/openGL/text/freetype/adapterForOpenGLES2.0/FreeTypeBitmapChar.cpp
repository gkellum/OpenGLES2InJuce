#include "FreeTypeBitmapChar.h"

#include "GLCallBatcher.h"
#include "FreeTypeAmalgam.h"
#include "OpenGLEnvironment.h"

#include <cassert>

FreeTypeBitmapChar::FreeTypeBitmapChar()
  : m_x(0)
  , m_y(0)
  , m_width(0)
  , m_height(0)
  , m_xOffset(0)
  , m_yOffset(0)
  , m_xAdvance(0)
  , m_charCode(0)
  , m_pGlyph(NULL)
{
}

FreeTypeBitmapChar::FreeTypeBitmapChar(int charCode)
  : m_x(0)
  , m_y(0)
  , m_width(0)
  , m_height(0)
  , m_xOffset(0)
  , m_yOffset(0)
  , m_xAdvance(0)
  , m_charCode(charCode) 
  , m_pGlyph(NULL)
{

}

FreeTypeBitmapChar::~FreeTypeBitmapChar()
{
}

void FreeTypeBitmapChar::InitTexCoords(int texWidth, int texHeight)
{
    float x1 = (float)m_x/(float)texWidth;
    float y1 = (float)m_y/(float)texHeight;
    float x2 = (float)(m_x+m_width)/(float)texWidth;
    float y2 = (float)(m_y+m_height)/(float)texHeight;

    m_texCoords[0] = x1;
    m_texCoords[1] = y1;

    m_texCoords[2] = x2;
    m_texCoords[3] = y1;

    m_texCoords[4] = x1;
    m_texCoords[5] = y2;

    m_texCoords[6] = x2;
    m_texCoords[7] = y2;
}

// setup quad and draw as triangle groups
void FreeTypeBitmapChar::render(OpenGLEnvironment& environment, int x, int y)
{
    if (IsEmpty()) return;

    x += m_xOffset; 
    y += m_yOffset;
    float vertices[verticesPerQuad*compVertPos];
    vertices[0] = (float)x;
    vertices[1] = (float)y;

    vertices[2] = (float)(x+m_width);
    vertices[3] = (float)y;

    vertices[4] = (float)x;
    vertices[5] = (float)(y+m_height);

    vertices[6] = (float)(x+m_width);
    vertices[7] = (float)(y+m_height);

    environment.getCallBatcher().addQuad(environment, m_texCoords, vertices);
} // end of draw()


void FreeTypeBitmapChar::DrawToBitmap(unsigned char* pData, int texWidth, int texHeight) 
{
    if (IsEmpty()) return;
    InitTexCoords(texWidth, texHeight);
    // Convert The Glyph To A Bitmap.
    FT_Glyph_To_Bitmap(&m_pGlyph, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)m_pGlyph;

    // This Reference Will Make Accessing The Bitmap Easier.
    FT_Bitmap& bitmap = bitmap_glyph->bitmap;

    assert(bitmap.width == m_width);
    assert(bitmap.rows == m_height);
    int x, y = 0;
    int index;
    for (y = 0; y < bitmap.rows; y++)
    {
        for (x = 0; x < bitmap.width; x++)
        {
            index = (m_y+y)* texWidth + m_x + x;
            pData[index] = bitmap.buffer[y*bitmap.width + x];
        }
    }
}

void FreeTypeBitmapChar::ReleaseGlyph() 
{
    if (m_pGlyph) FT_Done_Glyph(m_pGlyph);
}
