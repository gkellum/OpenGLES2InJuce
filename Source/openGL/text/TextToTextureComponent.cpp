#include "TextToTextureComponent.h"

#include "ResourceManager.h"
#include "CustomFonts.h"
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

#define SCALEX(x, width) 2*((x)/width)-1.0f
#define SCALEY(y, height) -2*((y)/height)+1.0f

TextToTextureComponent::TextToTextureComponent()
  : openGLContext()
  , explanationTextTextureId(-1)
  , numberTextTextureId(-1)
  , iteration(0)
{
    openGLContext.setRenderer(this);
    openGLContext.attachTo(*this);
}

TextToTextureComponent::~TextToTextureComponent()
{
    openGLContext.detach();
}

GLuint TextToTextureComponent::createTextureForTextUsingJuceTextRenderer(const String& textString, Rectangle<float>& bounds)
{
    Font font("Helvetica Neue", 22.0f, Font::bold);
    GlyphArrangement glyphArrangement;
    int maximumLinesToUse = 1000;
    float minimumHorizontalScale = 1.0f;
    glyphArrangement.addFittedText(font, textString, 0, 0, getWidth(), getHeight(), Justification::topLeft, maximumLinesToUse, minimumHorizontalScale);

    bool includeWhitespace = true;
    bounds = glyphArrangement.getBoundingBox(0, glyphArrangement.getNumGlyphs(), includeWhitespace);
    int imageWidth = ceil(bounds.getWidth());
    int imageHeight = ceil(bounds.getHeight());
    bool clearImage = true;
    Image imageForText( Image::ARGB, imageWidth, imageHeight, clearImage );

    Graphics g(imageForText);
    g.setOrigin(0, 0);
    glyphArrangement.draw(g);

    GLuint textureId;

    glGenTextures ( 1, &textureId );
    glBindTexture ( GL_TEXTURE_2D, textureId );

    Image::BitmapData imageForTextData(imageForText, 0, 0, imageWidth, imageHeight, Image::BitmapData::readOnly);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageForTextData.data );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    return textureId;
}

void TextToTextureComponent::newOpenGLContextCreated()
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

    const char* textString = "This text was drawn using Juce's text renderer. "
                             "Below is a series of numbers cycling from 0 - 9:";
    explanationTextTextureId = createTextureForTextUsingJuceTextRenderer(textString, explanationTextTextureBounds);
}

void TextToTextureComponent::openGLContextClosing()
{
}

String TextToTextureComponent::getNextNumberString()
{
    switch (iteration)
    {
        case 0:
            return "0123456789";
        case 1:
            return "1234567890";
        case 2:
            return "2345678901";
        case 3:
            return "3456789012";
        case 4:
            return "4567890123";
        case 5:
            return "5678901234";
        case 6:
            return "6789012345";
        case 7:
            return "7890123456";
        case 8:
            return "8901234567";
        case 9:
            return "9012345678";
        default:
            iteration %= 10;
            return getNextNumberString();
    }
}

void TextToTextureComponent::renderOpenGL()
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

    // Draw explanatory text
    {
        float x = 5;
        float y = 5;
        GLfloat vVertices[] = { SCALEX(x, getWidth()), SCALEY(y, getHeight()), 0.0f,  // Position 0
                                0.0f,  0.0f,        // TexCoord 0 
                                SCALEX(x, getWidth()), SCALEY(y+explanationTextTextureBounds.getHeight(), getHeight()), 0.0f,  // Position 1
                                0.0f,  1.0f,        // TexCoord 1
                                SCALEX(x+explanationTextTextureBounds.getWidth(), getWidth()), SCALEY(y+explanationTextTextureBounds.getHeight(), getHeight()), 0.0f,  // Position 2
                                1.0f,  1.0f,        // TexCoord 2
                                SCALEX(x+explanationTextTextureBounds.getWidth(), getWidth()), SCALEY(y, getHeight()), 0.0f,  // Position 3
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

        // Bind the texture
        glActiveTexture ( GL_TEXTURE0 );
        glBindTexture ( GL_TEXTURE_2D, explanationTextTextureId );

        // Set the sampler texture unit to 0
        glUniform1i ( samplerLocationUniform->uniformID, 0 );

        glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
    }

    // Draw number text
    {
        iteration++;
        GLuint numberTextTextureId = createTextureForTextUsingJuceTextRenderer(getNextNumberString(), numberTextTextureBounds);

        float x = 5;
        float y = 5 + explanationTextTextureBounds.getHeight();
        GLfloat vVertices[] = { SCALEX(x, getWidth()), SCALEY(y, getHeight()), 0.0f,  // Position 0
                                0.0f,  0.0f,        // TexCoord 0 
                                SCALEX(x, getWidth()), SCALEY(y+numberTextTextureBounds.getHeight(), getHeight()), 0.0f,  // Position 1
                                0.0f,  1.0f,        // TexCoord 1
                                SCALEX(x+numberTextTextureBounds.getWidth(), getWidth()), SCALEY(y+numberTextTextureBounds.getHeight(), getHeight()), 0.0f,  // Position 2
                                1.0f,  1.0f,        // TexCoord 2
                                SCALEX(x+numberTextTextureBounds.getWidth(), getWidth()), SCALEY(y, getHeight()), 0.0f,  // Position 3
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

        // Bind the texture
        glActiveTexture ( GL_TEXTURE0 );
        glBindTexture ( GL_TEXTURE_2D, numberTextTextureId );

        // Set the sampler texture unit to 0
        glUniform1i ( samplerLocationUniform->uniformID, 0 );

        glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
    }
}


