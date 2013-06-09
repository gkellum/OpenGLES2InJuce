#ifndef FreeTypeBITMAPFONT_H_
#define FreeTypeBITMAPFONT_H_

#include <map>
#include <string>
#include <memory>
#if OS_MACOSX || OS_MACIOS || OS_ANDROID
  #include <tr1/memory>
#endif

class FreeTypeBitmapChar;
typedef std::tr1::shared_ptr<FreeTypeBitmapChar> FreeTypeBitmapCharPtr;
class FontAtlas;
typedef std::tr1::shared_ptr<FontAtlas> FontAtlasPtr;
typedef std::tr1::weak_ptr<FontAtlas> FontAtlasWeakPtr;
class OpenGLEnvironment;

class FreeTypeBitmapFont
{
public:
    FreeTypeBitmapFont(FontAtlasWeakPtr pFontAtlas);
    ~FreeTypeBitmapFont();

    typedef std::tr1::shared_ptr<FreeTypeBitmapFont> Ptr;

    void Init();
    void Release();

    void DrawVertical(int x, int y, const std::string& text, int color, float alpha = 1.0f);

    // draws the string at position x, y (given in screen coordinates)
    int DrawString(int x, int y, const std::string& text, int color, float alpha = 1.0f)
    {
        return DrawString(x, y, text.c_str(), color, alpha);
    }   

    int DrawStringShadow(int x, int y, const std::string& text, int color, int backColor)
    {
        return DrawStringShadow(x, y, text.c_str(), color, backColor);
    }   

    int drawString(OpenGLEnvironment& environment,
                   int x, int y, const char* text, int color, float alpha = 1.0f);

    int DrawStringShadow(int x, int y, const char* text, int color, int backColor);

    int GetWidth(const char* text);

    int GetLineHeight() const
    {
        return m_lineHeight;
    }

    void SetLineHeight(int lineHeight)
    {
        m_lineHeight = lineHeight;
    }

    int GetCharWidth(const std::string& text, int index);
    void AddChar(int charCode, FreeTypeBitmapCharPtr pFreeTypeBitmapChar)
    {
        m_mapBitmapChar[charCode] = pFreeTypeBitmapChar;
    }
    void SetFreeTypeFace(struct FT_FaceRec_* pFreeTypeFace)
    {
        m_pFreeTypeFace = pFreeTypeFace;
    }
    void FinishCreating();

    FreeTypeBitmapCharPtr GetChar(int charCode)
    {
        std::map<int, FreeTypeBitmapCharPtr>::const_iterator iter;
        iter = m_mapBitmapChar.find(charCode);
        if (iter != m_mapBitmapChar.end()) 
            return iter->second;
        return FreeTypeBitmapCharPtr();
    }
private:
    void ReleaseFace();
    int m_lineHeight;
    std::map<int, FreeTypeBitmapCharPtr> m_mapBitmapChar;
    FontAtlasWeakPtr m_pFontAtlas;
    struct FT_FaceRec_* m_pFreeTypeFace;
};

#endif /*FTBITMAPFONT_H_*/
