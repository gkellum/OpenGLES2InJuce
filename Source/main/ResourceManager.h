#ifndef __ResourceManager_INCLUDE__
#define __ResourceManager_INCLUDE__

#include "JuceHeader.h"

#include <string>

class ResourceManager
{
public:
    static juce::Image getImageFromFile(const String& filename);

    static String getShaderFromFile(const String& filename);

    static File getFontFromFile(const String& filename);
};

#endif
