#include "FontAtlasFactory.h"

#include "FontAtlas.h"
#include "JuceHeader.h"

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#else
  #ifndef glOrthof
    #define glOrthof glOrtho
  #endif
#endif

using namespace std;

bool isError(int errorCode)
{
    if (errorCode != 0)
    {
        std::cerr << "OpenGL error code <" << errorCode << ">" << std::endl;
        return true;
    }
    return false;
}

FontAtlasPtr FontAtlasFactory::createFontAtlas(const std::vector<FontInfo>& fontInfoVector,
                                               int canvasWidth, 
                                               int canvasHeight)
{
    if (isError(glGetError())) { jassertfalse }

    // Disabled for OpenGL ES 2
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    if (isError(glGetError())) { jassertfalse }
    glEnable(GL_CULL_FACE);
    if (isError(glGetError())) { jassertfalse }
    glDisable(GL_DITHER);   // these are enabled by default
    if (isError(glGetError())) { jassertfalse }
    // Disabled for OpenGL ES 2
    glDisable(GL_ALPHA_TEST);
    if (isError(glGetError())) { jassertfalse }

    glViewport(0, 0, canvasWidth, canvasHeight);
    if (isError(glGetError())) { jassertfalse }
    glMatrixMode(GL_MODELVIEW);
    if (isError(glGetError())) { jassertfalse }
    glLoadIdentity();
    if (isError(glGetError())) { jassertfalse }

    glDisable(GL_LIGHTING);
    if (isError(glGetError())) { jassertfalse }
    glFrontFace(GL_CW);
    if (isError(glGetError())) { jassertfalse }
    glMatrixMode(GL_PROJECTION);
    if (isError(glGetError())) { jassertfalse }
    glLoadIdentity();
    if (isError(glGetError())) { jassertfalse }
    // set orthographic, so that we are drawing billboard style using screen dimensions 
    glOrthof(0, (float)canvasWidth, (float)canvasHeight, 0, -50.0f, 50.0f);
    if (isError(glGetError())) { jassertfalse }

    glActiveTexture(GL_TEXTURE1);
    if (isError(glGetError())) { jassertfalse }
    glDisable(GL_TEXTURE_2D);
    if (isError(glGetError())) { jassertfalse }
    glActiveTexture(GL_TEXTURE0);
    if (isError(glGetError())) { jassertfalse }
    glEnable(GL_TEXTURE_2D);
    if (isError(glGetError())) { jassertfalse }
    glClientActiveTexture(GL_TEXTURE1);
    if (isError(glGetError())) { jassertfalse }
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (isError(glGetError())) { jassertfalse }
    glClientActiveTexture(GL_TEXTURE0);
    if (isError(glGetError())) { jassertfalse }
    glEnableClientState(GL_VERTEX_ARRAY);
    if (isError(glGetError())) { jassertfalse }
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if (isError(glGetError())) { jassertfalse }
    glDisableClientState(GL_COLOR_ARRAY);
    if (isError(glGetError())) { jassertfalse }
    glDisable(GL_DEPTH_TEST);   
    if (isError(glGetError())) { jassertfalse }
    // handle tranparancy
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (isError(glGetError())) { jassertfalse }

    //const char* renderableCharacters = " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyzÁáÉéÍíÑñÓóÚú";
    const char* renderableCharacters = " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyz";

    FontAtlasPtr fontAtlas(new FontAtlas());

    std::vector<FontInfo>::const_iterator it;
    for (it = fontInfoVector.begin(); it != fontInfoVector.end(); it++)
    {
        const FontInfo& fontInfo = *it;
        fontAtlas->addFont(fontInfo.pathToFont, fontInfo.fontSize, renderableCharacters);
    }

    fontAtlas->createAtlas();

    return fontAtlas;
}

