#ifndef __RAIIOpenGLRenderer_H__
#define __RAIIOpenGLRenderer_H__

#include "Settings.h"

class RAIIOpenGLRenderer
{
public:
    RAIIOpenGLRenderer(OpenGLRenderer* renderer)
      : renderer(renderer)
    {
        Settings::getInstance().addOpenGLRenderer(renderer);
    }

    ~RAIIOpenGLRenderer()
    {
        Settings::getInstance().removeOpenGLRenderer(renderer);
    }

    OpenGLRenderer* renderer;
};

#endif
