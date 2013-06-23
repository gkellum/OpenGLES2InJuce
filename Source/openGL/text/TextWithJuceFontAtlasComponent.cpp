#include "TextWithJuceFontAtlasComponent.h"

#include "ResourceManager.h"

#include <cmath>

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#endif

#define PI 3.14159265

#define SCALEX(x, width) 2*((x)/width)-1.0f
#define SCALEY(y, height) -2*((y)/height)+1.0f

TextWithJuceFontAtlasComponent::TextWithJuceFontAtlasComponent()
  : openGLContext()
  , iteration(0)
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
    float glyphWidth;
    float glyphHeight;
    int textureWidth;
    float textureHeight;
};

std::map<int, TextWithJuceFontAtlasComponent::OpenGLGlyphTextureInfo> TextWithJuceFontAtlasComponent::createBitmapFontUsingJuceTextRenderer()
{
    std::map<int, OpenGLGlyphTextureInfo> asciiCodeToTextureInfoMap;
    std::pair<int, int> firstAndLastAsciiCode(32, 122);
    GlyphArrangement glyphArrangement;
    Font font("Helvetica Neue", 22.0f, Font::bold);
    for (int asciiCode = firstAndLastAsciiCode.first; asciiCode <= firstAndLastAsciiCode.second; asciiCode++)
    {
        glyphArrangement.clear();
        String textString((char*)&asciiCode, 1);
        glyphArrangement.addLineOfText(font, textString, 0.0f, 0.0f );
        Rectangle<float> glyphBounds = glyphArrangement.getGlyph(0).getBounds();
        float glyphX = glyphArrangement.getGlyph(0).getLeft();
        float glyphY = glyphBounds.getY();
        float glyphWidth = glyphBounds.getWidth();
        float glyphHeight = glyphBounds.getHeight();

        //std::cerr << "Creating image for character '" << (char) asciiCode << "' with bounds [" << glyphX << ", " 
        //          << glyphY << ", " << glyphWidth << ", " << glyphHeight << "]" << std::endl;

        bool clearImage = true;
        int imageWidth = ceil(glyphWidth);
        int imageHeight = ceil(1.2f * glyphHeight);
        Image imageForText( Image::ARGB, imageWidth, imageHeight, clearImage );

        Graphics g(imageForText);
        const Rectangle<int> area(0, 0, imageWidth, imageHeight);

        /*
        int maximumNumberOfLines = 1;
        float minimumHorizontalScale = 0.9f;
        g.drawFittedText(textString, area, Justification::centred, maximumNumberOfLines, minimumHorizontalScale);
        */

        g.setOrigin(0, ceil(glyphHeight));
        glyphArrangement.draw(g);

        OpenGLGlyphTextureInfo textureInfo;

        glGenTextures ( 1, &textureInfo.textureId );
        glBindTexture ( GL_TEXTURE_2D, textureInfo.textureId );

        Image::BitmapData imageForTextData(imageForText, 0, 0, imageWidth, imageHeight, Image::BitmapData::readOnly);

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageForTextData.data );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        textureInfo.glyphWidth = glyphWidth;
        textureInfo.glyphHeight = glyphHeight;
        textureInfo.textureWidth = imageWidth;
        textureInfo.textureHeight = imageHeight;

        asciiCodeToTextureInfoMap[asciiCode] = textureInfo;
    }

    return asciiCodeToTextureInfoMap;
}

String TextWithJuceFontAtlasComponent::getNextNumberString()
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

int TextWithJuceFontAtlasComponent::drawTextWithBitmapFont(const String& textMessage, int startX, int startY)
{
    float x = startX;
    float y = startY;
    int stringLength = textMessage.length();
    float charImageHeight = 0;
    std::map<int, OpenGLGlyphTextureInfo>::iterator it;
    for (int indexOfCharacter = 0; indexOfCharacter < stringLength; indexOfCharacter++)
    {
        int asciiCode = (int) textMessage[indexOfCharacter];
        it = asciiCodeToTextureInfoMap.find(asciiCode);

        if (it == asciiCodeToTextureInfoMap.end())
        {
            std::cerr << "Could not find glyph for ascii code <" << asciiCode << ">" << std::endl;
            continue;
        }

        OpenGLGlyphTextureInfo& textureInfo = it->second;

        float charImageWidth = textureInfo.textureWidth;
        charImageHeight = textureInfo.textureHeight;

        float charGlyphWidth = textureInfo.glyphWidth;
        float charGlyphHeight = textureInfo.glyphHeight;

        if ( (x + charGlyphWidth + 5) > getWidth())
        {
            x = 5;
            y += charImageHeight;
        }

        /*
        std::cerr << "Character '" << textMessage[counter] << "' bounds [" << x << ", " << y << ", " << charImageWidth << ", " << charImageHeight << ", " << getWidth() << ", " << getHeight() << "] " << std::endl
                  << "(" << SCALEX(x, getWidth()) << ", " << SCALEY(y, getHeight()) << ") "
                  << "(" << SCALEX(x, getWidth()) << ", " << SCALEY(y+charImageHeight, getHeight()) << ") "
                  << "(" << SCALEX(x+charImageWidth, getWidth()) << ", " << SCALEY(y+charImageHeight, getHeight()) << ") "
                  << "(" << SCALEX(x+charImageWidth, getWidth()) << ", " << SCALEY(y, getHeight()) << ") "
                  << std::endl;
        */

        GLfloat vVertices[] = { SCALEX(x, getWidth()), SCALEY(y, getHeight()), 0.0f,  // Position 0
                                0.0f,  0.0f,        // TexCoord 0 
                                SCALEX(x, getWidth()), SCALEY(y+charImageHeight, getHeight()), 0.0f,  // Position 1
                                0.0f,  1.0f,        // TexCoord 1
                                SCALEX(x+charImageWidth, getWidth()), SCALEY(y+charImageHeight, getHeight()), 0.0f,  // Position 2
                                1.0f,  1.0f,        // TexCoord 2
                                SCALEX(x+charImageWidth, getWidth()), SCALEY(y, getHeight()), 0.0f,  // Position 3
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

        glActiveTexture ( GL_TEXTURE0 );
        glBindTexture ( GL_TEXTURE_2D, textureInfo.textureId );

        // Set the sampler texture unit to 0
        glUniform1i ( samplerLocationUniform->uniformID, 0 );

        glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

        x += charImageWidth;
    }

    return y + charImageHeight;
}

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

    // Bind the texture
    String textMessage("This text was drawn using a single texture for each character where each texture was created at startup using Juce's text renderer. "
                       "Below is a series of numbers cycling from 0 - 9:");
    int endY = drawTextWithBitmapFont(textMessage, 5, 5);

    iteration++;
    drawTextWithBitmapFont(getNextNumberString(), 5, endY);
}


