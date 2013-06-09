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
        ,Texture
        ,SlowText
        ,FastText
        ,NumberOfApplicationModes
        ,Cube
    };

    static String getName(ID applicationModeID)
    {
        switch (applicationModeID)
        {
        case Triangle: return "Triangle";
        case Square: return "Square";
        case Texture: return "Texture";
        case Cube: return "Cube";
        case SlowText: return "Text Var#1";
        case FastText: return "Text Var#2";
        default:
            jassertfalse
            return String::empty;
        }
    }
};

#endif
