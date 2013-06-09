#include "TextWithJuceFontAtlasComponent.h"

#include "ResourceManager.h"

#include <cmath>

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#endif

#define PI 3.14159265

TextWithJuceFontAtlasComponent::TextWithJuceFontAtlasComponent()
  : openGLContext()
{
    openGLContext.setRenderer(this);
    openGLContext.attachTo(*this);
}

TextWithJuceFontAtlasComponent::~TextWithJuceFontAtlasComponent()
{
    openGLContext.detach();
}

struct TextWithJuceFontAtlasComponent::OpenGLGlyphTextureInfo
{
    GLuint textureId;
    float textureWidth;
    float textureHeight;
};

std::map<int, TextWithJuceFontAtlasComponent::OpenGLGlyphTextureInfo> TextWithJuceFontAtlasComponent::createBitmapFontUsingJuceTextRenderer()
{
    std::map<int, OpenGLGlyphTextureInfo> asciiCodeToTextureInfoMap;
    std::pair<int, int> firstAndLastAsciiCode(32, 122);
    GlyphArrangement glyphArrangement;
    Font font("Helvetica Neue", 14.5f, Font::bold);
    for (int asciiCode = firstAndLastAsciiCode.first; asciiCode <= firstAndLastAsciiCode.second; asciiCode++)
    {
        glyphArrangement.clear();
        glyphArrangement.addLineOfText(font, String((char*)&asciiCode, 1), 0.0f, 0.0f );
        float imageWidth = glyphArrangement.getBoundingBox (0, -1, true).getWidth();
        float imageHeight = glyphArrangement.getBoundingBox (0, -1, true).getHeight();

        std::cerr << "Creating image for character '" << (char) asciiCode << "' with dimensions (" << imageWidth << ", " << imageHeight << ")" << std::endl;

        bool clearImage = true;
        // TODO: fix this memory leak...
        Image* imageForText = new Image( Image::ARGB, ceil(imageWidth), ceil(imageHeight), clearImage );

        Graphics g(*imageForText);
        const Rectangle<int> area(0, 0, imageWidth, imageHeight);
        
        //int maximumNumberOfLines = 1;
        //float minimumHorizontalScale = 0.9f;
        //g.drawFittedText(textString, area, Justification::centred, maximumNumberOfLines, minimumHorizontalScale);

        glyphArrangement.draw(g);

        OpenGLGlyphTextureInfo textureInfo;

        glGenTextures ( 1, &textureInfo.textureId );
        glBindTexture ( GL_TEXTURE_2D, textureInfo.textureId );

        Image::BitmapData imageForTextData(*imageForText, 0, 0, imageWidth, imageHeight, Image::BitmapData::readOnly);

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, ceil(imageWidth), ceil(imageHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageForTextData.data );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        textureInfo.textureWidth = imageWidth;
        textureInfo.textureHeight = imageHeight;

        asciiCodeToTextureInfoMap[asciiCode] = textureInfo;
    }

    return asciiCodeToTextureInfoMap;
}

void TextWithJuceFontAtlasComponent::drawTextWithBitmapFont(const String& message, int startX, int startY)
{
    int lengthOfString = message.length();
    std::map<int, OpenGLGlyphTextureInfo>::iterator it;
    int x = startX;
    int y = startY;
    for (int indexOfCharacter = 0; indexOfCharacter < lengthOfString; indexOfCharacter++)
    {
        int asciiCode = (int) message[indexOfCharacter];
        it = asciiCodeToTextureInfoMap.find(asciiCode);
        if (it == asciiCodeToTextureInfoMap.end())
        {
            std::cerr << "Could not find glyph for ascii code <" << asciiCode << ">" << std::endl;
        }
        else
        {
            //std::cerr << "Drawing glyph for ascii code <" << asciiCode << "> at (" << x << ", " << y << ")" << std::endl;

            OpenGLGlyphTextureInfo& textureInfo = it->second;

            glActiveTexture ( GL_TEXTURE0 );
            glBindTexture ( GL_TEXTURE_2D, textureInfo.textureId );

            // Set the sampler texture unit to 0
            glUniform2i ( samplerLocationUniform->uniformID, x, y );

            GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
            glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

            x += textureInfo.textureWidth;
        }
    }
}

/*
void OpenGLComponent::createBitmapFont()
{
   OpenGLTexture texture[NUM_GLIPHS];
   base = glGenLists(NUM_GLIPHS);                        // Storage For 96 Characters

   Font f;
   f.setHeight(24);

   GlyphArrangement arr;
   for (unsigned char i = 32; i < 32 + NUM_GLIPHS; ++i)
   {
      arr.clear();
      arr.addLineOfText(f, String((char*)&i, 1), 0.0f, 0.0f );
      float w = arr.getBoundingBox (0, -1, true).getWidth(),
           h = arr.getBoundingBox (0, -1, true).getHeight();
      int   w1 = nextPowerOfTwo((int)w), h1 = nextPowerOfTwo((int)h);
      Image image(Image::ARGB, w1, h1, true );
      Graphics g (image);
      g.setOrigin(0, (int)h1);
      arr.draw (g);
      
      texture[i-32].loadImage(image);

      glNewList(base + i - 32, GL_COMPILE);      
      texture[i-32].draw2D( 0.0f    , 0.0f,
                  0.0f + w1, 0.0f,
                  0.0f + w1, 0.0f + h1,
                  0.0f    , 0.0f + h1,
                  Colours::darksalmon );
      glTranslatef(w,0.0f,0.0f);
      glEndList();
   }
}
*/

void TextWithJuceFontAtlasComponent::newOpenGLContextCreated()
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

    asciiCodeToTextureInfoMap = createBitmapFontUsingJuceTextRenderer();
}

void TextWithJuceFontAtlasComponent::openGLContextClosing()
{
}

void TextWithJuceFontAtlasComponent::renderOpenGL()
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

    /*
    GLfloat vVertices[] = { -0.5f,  0.25f, 0.0f,  // Position 0
                            0.0f,  0.0f,        // TexCoord 0 
                           -0.5f, -0.25f, 0.0f,  // Position 1
                            0.0f,  1.0f,        // TexCoord 1
                            0.5f, -0.25f, 0.0f,  // Position 2
                            1.0f,  1.0f,        // TexCoord 2
                            0.5f,  0.25f, 0.0f,  // Position 3
                            1.0f,  0.0f         // TexCoord 3
                         };
    */
    GLfloat vVertices[] = { 0, 0, 0.0f,  // Position 0
                            0.0f,  0.0f,        // TexCoord 0 
                            0, (GLfloat) getHeight(), 0.0f,  // Position 1
                            0.0f,  1.0f,        // TexCoord 1
                            (GLfloat) getWidth(), (GLfloat) getHeight(), 0.0f,  // Position 2
                            1.0f,  1.0f,        // TexCoord 2
                            (GLfloat) getWidth(),  0, 0.0f,  // Position 3
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
    /*
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, juceLogoTextureId );

    // Set the sampler texture unit to 0
    glUniform1i ( samplerLocationUniform->uniformID, 0 );

    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
    */

    String message("This was drawn with a bitmap font");
    drawTextWithBitmapFont(message, 0, 0);
}


