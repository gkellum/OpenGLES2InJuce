#include "TextWithFreetypeFontAtlasComponent.h"

#include "ResourceManager.h"
#include "FontAtlasFactory.h"
#include "RacException.h"
#include "FreeTypeBitmapFont.h"
#include "FontAtlas.h"

#include <cmath>

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#endif

#define PI 3.14159265

enum Fonts
{
    ARIAL_12PT
    ,TIMES_14PT
};

struct FontNameAndSize
{
    const char* fontName;
    int fontSize;
};

FontNameAndSize availableFonts[] = 
{
    { "arial.ttf", 12 }
    ,{ "times.ttf", 14 }
};

TextWithFreetypeFontAtlasComponent::TextWithFreetypeFontAtlasComponent()
  : openGLContext()
{
    openGLContext.setRenderer(this);
    openGLContext.attachTo(*this);
}

TextWithFreetypeFontAtlasComponent::~TextWithFreetypeFontAtlasComponent()
{
    openGLContext.detach();
}

void TextWithFreetypeFontAtlasComponent::newOpenGLContextCreated()
{
    shaderProgram = new OpenGLShaderProgram(openGLContext);

    // The source files for the shaders are in the directory Resources/Shaders
    String vertexShader = ResourceManager::getShaderFromFile("texture.vsh");
#ifdef OS_MACIOS
    String fragmentShader = ResourceManager::getShaderFromFile("texture.fsh");
#else
    String fragmentShader = ResourceManager::getShaderFromFile("texture_desktop.fsh");
#endif  

   // Load the vertex/fragment shaders
    bool succeeded = shaderProgram->addShader((const char*) vertexShader.toUTF8(), GL_VERTEX_SHADER);
    if (!succeeded)
    {
        jassertfalse
        return;
    }

    succeeded = shaderProgram->addShader((const char*) fragmentShader.toUTF8(), GL_FRAGMENT_SHADER);
    if (!succeeded)
    {
        jassertfalse
        return;
    }

    succeeded = shaderProgram->link();
    if (!succeeded)
    {
        jassertfalse
        return;
    }

    positionAttribute = new OpenGLShaderProgram::Attribute(*shaderProgram, "a_position");
    textureCoordinateAttribute = new OpenGLShaderProgram::Attribute(*shaderProgram, "a_texCoord");

    samplerLocationUniform = new OpenGLShaderProgram::Uniform(*shaderProgram, "s_texture");

    createFreeTypeFontAtlas();
}

void TextWithFreetypeFontAtlasComponent::createFreeTypeFontAtlas()
{
    // get the current error to clear it...
    glGetError();

    int numberOfFonts = sizeof(availableFonts) / sizeof(availableFonts[0]);
    std::vector<FontInfo> fontInfoVector;
    for (int counter = 0; counter < numberOfFonts; counter++)
    {
        std::string pathToFont = (const char*) ResourceManager::getFontFromFile(availableFonts[counter].fontName).getFullPathName().toUTF8();
        FontInfo fontInfo = {pathToFont, availableFonts[counter].fontSize};
        fontInfoVector.push_back( fontInfo );
    }

    try
    {
        environment = new OpenGLEnvironmentImpl(FontAtlasFactory::createFontAtlas(fontInfoVector, getWidth(), getHeight()),
                                                                                                  getWidth(), getHeight());
    }
    catch(RacException ex)
    {
        cout << ex.GetStrErr() << endl;
        return;
    }
}

void TextWithFreetypeFontAtlasComponent::openGLContextClosing()
{
}

void TextWithFreetypeFontAtlasComponent::renderOpenGL()
{
    OpenGLHelpers::clear(Colours::white);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_TEXTURE_2D);

    // Draw the gradient for the background fill
    glViewport(0, 0, getWidth(), getHeight());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    shaderProgram->use();

    GLfloat vVertices[] = { -0.5f,  0.25f, 0.0f,  // Position 0
                            0.0f,  0.0f,        // TexCoord 0 
                           -0.5f, -0.25f, 0.0f,  // Position 1
                            0.0f,  1.0f,        // TexCoord 1
                            0.5f, -0.25f, 0.0f,  // Position 2
                            1.0f,  1.0f,        // TexCoord 2
                            0.5f,  0.25f, 0.0f,  // Position 3
                            1.0f,  0.0f         // TexCoord 3
                         };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
      
    // Load the vertex position
    glVertexAttribPointer ( positionAttribute->attributeID, 3, GL_FLOAT,
                           GL_FALSE, 5 * sizeof(GLfloat), vVertices );
    // Load the texture coordinate
    glVertexAttribPointer ( textureCoordinateAttribute->attributeID, 2, GL_FLOAT,
                           GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );

    glEnableVertexAttribArray ( positionAttribute->attributeID );
    glEnableVertexAttribArray ( textureCoordinateAttribute->attributeID );

    if (environment != NULL)
    {
        FreeTypeBitmapFontPtr arialFont = environment->getFreeTypeFont(ARIAL_12PT);
        int black = 0;
        arialFont->drawString(*environment.get(), 5, 0, "This was drawn with a freetype text atlas renderer.", black);
        environment->getCallBatcher().render( *environment.get() );
        environment->getFontAtlas()->render( *environment.get(), 5, 300);
    }
}


