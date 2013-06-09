#ifndef FreeTypeBITMAPCHAR_H_
#define FreeTypeBITMAPCHAR_H_

const int verticesPerQuad = 4;
const int indicesPerQuad = 6;
const int compVertPos = 2;
const int compVertTex = 2;

const int WHITE = 0xffffff;
const int BLACK = 0;

#include <memory>
#if OS_MACOSX || OS_MACIOS || OS_ANDROID
  #include <tr1/memory>
#endif

struct FT_GlyphRec_;
class OpenGLEnvironment;

// each FTBitmapChar represents a character from the fnt file
// reads paramaters from fnt file and creates textured quad   
class FreeTypeBitmapChar
{
public:
    FreeTypeBitmapChar();
    FreeTypeBitmapChar(int charCode);
    ~FreeTypeBitmapChar();

    typedef std::tr1::shared_ptr<FreeTypeBitmapChar> Ptr;

    void render(OpenGLEnvironment& environment, int x, int y);

    void SetXY(int x, int y) 
    {
        m_x = x;
        m_y = y;
    }
    int GetX2() const
    {
        return m_x + m_width; 
    }
    int GetY2() const
    {
        return m_x + m_width; 
    }
    int GetXAdvance() const
    {
        return m_xAdvance;
    }
    void SetXAdvance(int xAdvance)
    {
        m_xAdvance = xAdvance;
    }
    int GetHeight() const
    {
        return m_height;
    }
    int GetTotalHeight() const
    {
        return m_yOffset + m_height;
    }
    int GetWidth() const
    {
        return m_width;
    }
    void SetSize(int width, int height)
    {
        m_width = width;
        m_height = height;
    }
    void SetOffsets(int xOffset, int yOffset)
    {
        m_xOffset = xOffset;
        m_yOffset = yOffset;
    }
    int GetYOffset() const
    {
        return m_yOffset;
    }
    void ReduceYOffset(int amount)
    {
        m_yOffset -= amount;
    }
    void AddKerning(FreeTypeBitmapChar* pBitmapChar, int amount);
    bool IsLoaded() const
    {
        return m_x >= 0;
    }
    int GetNumPixels() const
    {
        return m_width*m_height;
    }
    int GetCharCode() const
    {
        return m_charCode;
    }
    void SetCharCode(int charCode)
    {
        m_charCode = charCode;
    }
    void SetGlyph(struct FT_GlyphRec_* pGlyph)
    {
        m_pGlyph = pGlyph;
    }
    void DrawToBitmap(unsigned char* pData, 
        int texWidth, int texHeight);
    bool IsEmpty() const
    {
        return m_width == 0 || m_height == 0;
    }
    void ReleaseGlyph();
    void InitTexCoords(int texWidth, int texHeight);
private:
    int m_x, m_y;
    int m_width;
    int m_height;
    int m_xOffset;
    int m_yOffset;
    int m_xAdvance;
    int m_charCode; // for debugging
    float m_texCoords[verticesPerQuad*compVertTex];
    struct FT_GlyphRec_* m_pGlyph;
};

#endif /*FTBITMAPCHAR_H_*/
