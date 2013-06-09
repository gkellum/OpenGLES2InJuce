#ifndef __DirectoryBasedFontLoader_H__
#define __DirectoryBasedFontLoader_H__

#include "FontInfo.h"

#include <string>
#include <vector>
#include <memory>
#if OS_MACOSX || OS_MACIOS || OS_ANDROID
  #include <tr1/memory>
#endif

class FontAtlas;
typedef std::tr1::shared_ptr<FontAtlas> FontAtlasPtr;

class FontAtlasFactory
{
public:
    static FontAtlasPtr createFontAtlas(const std::vector<FontInfo>& fontInfoVector,
                                        int canvasWidth, 
                                        int canvasHeight);
};

#endif