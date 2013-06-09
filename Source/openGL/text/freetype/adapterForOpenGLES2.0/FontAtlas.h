#ifndef _FONTATLASMAKER_H_INCLUDED_
#define _FONTATLASMAKER_H_INCLUDED_

#include <string>
#include <vector>
#include <memory>
#if OS_MACOSX || OS_MACIOS || OS_ANDROID
  #include <tr1/memory>
#endif

struct FT_LibraryRec_;
struct FT_FaceRec_;
class FreeTypeBitmapChar;
typedef std::tr1::shared_ptr<FreeTypeBitmapChar> FreeTypeBitmapCharPtr;
class FreeTypeBitmapFont;
typedef std::tr1::shared_ptr<FreeTypeBitmapFont> FreeTypeBitmapFontPtr;
class OpenGLEnvironment;

class FontAtlas : public std::tr1::enable_shared_from_this<FontAtlas>
{
public:
    FontAtlas();
    ~FontAtlas();

    typedef std::tr1::shared_ptr<FontAtlas> Ptr;

    void addFont(const std::string& pathToFont, 
                 int size, 
                 const char* renderableLetters);

    void createAtlas();

    unsigned int getTextureID() const
    {
        return m_textureID;
    }

    FreeTypeBitmapFontPtr getFont(int index)
    {
        return m_listBitmapFont[index];
    }

    void render(OpenGLEnvironment& environment, int x, int y);

    bool getUseKerning() const { return m_useKerning; }
    void setUseKerning(bool useKerning) { m_useKerning = useKerning; }

private:
    bool binPack(int width, int height);
    void releaseTexture();
    void releaseLibrary();

    struct FT_LibraryRec_* m_pLibrary;
    std::vector<FreeTypeBitmapCharPtr> m_listBitmapChar;
    unsigned int m_textureID;
    std::vector<FreeTypeBitmapFontPtr> m_listBitmapFont;
    std::vector<struct FT_FaceRec_*> m_listFace;
    FreeTypeBitmapChar* m_pShowAtlas;
    bool m_useKerning;
};

#endif // _FONTATLASMAKER_H_INCLUDED_
