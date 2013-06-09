#ifndef __FontInfo_H__
#define __FontInfo_H__

#include <string>

#include <memory>
#if OS_MACOSX || OS_MACIOS || OS_ANDROID
  #include <tr1/memory>
#endif

struct FontInfo
{
    typedef std::tr1::shared_ptr<FontInfo> Ptr;

    std::string pathToFont;
    int fontSize;
};

#endif