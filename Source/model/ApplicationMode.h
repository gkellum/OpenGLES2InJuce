#ifndef __ApplicationMode_H___
#define __ApplicationMode_H___

#include "JuceHeader.h"

class ApplicationMode
{
public:
    enum ID
    {
        Triangle
        ,Square
        ,Points
        ,Texture
        ,TextRenderedToImage
        ,TextRenderedFromFontAtlas
        ,NumberOfApplicationModes
    };

    static String getName(ID applicationModeID)
    {
        switch (applicationModeID)
        {
        case Triangle: return "Triangle";
        case Square: return "Square";
        case Points: return "Points";
        case Texture: return "Texture";
        case TextRenderedToImage: return "Text Var#1";
        case TextRenderedFromFontAtlas: return "Text Var#2";
        default:
            jassertfalse;
            return String::empty;
        }
    }
};

#endif
