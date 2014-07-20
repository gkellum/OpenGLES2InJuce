#include "TexturedComponent.h"

#include "Settings.h"

#include "OpenGLIncludes.h"

#include <cmath>

TexturedComponent::TexturedComponent()
  : raiiOpenGLRenderer(this)
  , initialized(false)
  , juceLogoTextureId(-1)
{
}

TexturedComponent::~TexturedComponent()
{
}

GLuint TexturedComponent::createTextureForImageFile( char *fileName )
{
    Image juceLogoImage = ImageCache::getFromMemory(BinaryData::jucelogo96_png, BinaryData::jucelogo96_pngSize);

    if ( juceLogoImage == Image::null )
    {
        printf( "Error loading (%s) image.\n", fileName );
        return 0;
    }

    GLuint textureId;

    glGenTextures ( 1, &textureId );
    glBindTexture ( GL_TEXTURE_2D, textureId );

    Image::BitmapData juceLogoBitmapData(juceLogoImage, 0, 0, juceLogoImage.getWidth(), juceLogoImage.getHeight(), Image::BitmapData::readOnly);

    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, juceLogoImage.getWidth(), juceLogoImage.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, juceLogoBitmapData.data );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    return textureId;
}

void TexturedComponent::newOpenGLContextCreated()
{
    initialized = true;

    shaderProgram = new OpenGLShaderProgram(Settings::getInstance().getOpenGLContext());

    String vertexShader =   "attribute vec4 a_position;   \r\n"
                            "attribute vec2 a_texCoord;   \r\n"
                            "varying vec2 v_texCoord;     \r\n"
                            "void main()                  \r\n"
                            "{                            \r\n"
                            "    gl_Position = a_position; \r\n"
                            "    v_texCoord = a_texCoord;  \r\n"
                            "}\r\n";

    String fragmentShader = 
                            #ifdef OS_MACIOS
                            "precision mediump float;                            \r\n"
                            #endif
                            "varying vec2 v_texCoord;                            \r\n"
                            "uniform sampler2D s_texture;                        \r\n"
                            "void main()                                         \r\n"
                            "{                                                   \r\n"
                            "    gl_FragColor = texture2D( s_texture, v_texCoord ); \r\n"
                            "}                                                      \r\n";

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

    juceLogoTextureId = createTextureForImageFile("jucelogo96.png");
}

void TexturedComponent::openGLContextClosing()
{
}


void TexturedComponent::renderOpenGL()
{
    if (!initialized)
    {
        newOpenGLContextCreated();
    }

    OpenGLHelpers::clear(Colours::white);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_TEXTURE_2D);

    // Draw the gradient for the background fill
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

    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, juceLogoTextureId );

    // Set the sampler texture unit to 0
    glUniform1i ( samplerLocationUniform->uniformID, 0 );

    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
}


