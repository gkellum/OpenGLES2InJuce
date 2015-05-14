#include "PointSpriteComponent.h"

#include "Settings.h"
#include "TexturedComponent.h"
#include "OpenGLIncludes.h"

#include <cmath>

PointSpriteComponent::PointSpriteComponent()
  : raiiOpenGLRenderer(this)
  , initialized(false)
{
}

PointSpriteComponent::~PointSpriteComponent()
{
}

void PointSpriteComponent::newOpenGLContextCreated()
{
    initialized = true;

    //GLfloat pointSizeRange[2];
    //glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, pointSizeRange);
    //std::cerr << "Point sprite radius range (" << pointSizeRange[0] << ", " << pointSizeRange[1] << ")" << std::endl;
    // Outputs: Point sprite radius range (1, 511)
    
    //const float pixelToPointMultiplier = 0.59f;

    shaderProgram = new OpenGLShaderProgram(Settings::getInstance().getOpenGLContext());

    // The source files for the shaders are in the directory Resources/Shaders
    String vertexShader =
                            "attribute vec4 Position; \r\n"
                            "attribute vec4 SourceColor; \r\n"
                            "varying vec4 DestinationColor;  \r\n"
                            "void main(void) {  \r\n"
                            "   DestinationColor = SourceColor;  \r\n"
                            "   gl_Position = Position; \r\n"
                            "   gl_PointSize = 100.0; \r\n"
                            "} \r\n";

    String fragmentShader =     
                                #if defined(OS_MACIOS) || defined(OS_ANDROID)
                                "varying lowp vec4 DestinationColor;  \r\n"
                                #else
                                "#version 120 \r\n"
                                "varying vec4 DestinationColor;   \r\n"
                                #endif
                                "uniform sampler2D s_texture;                        \r\n"
                                "void main(void)  \r\n"
                                "{  \r\n"
                                "   //gl_FragColor = DestinationColor; \r\n"
                                "   gl_FragColor = texture2D(s_texture, gl_PointCoord); \r\n"
                                "} \r\n";

   // Load the vertex/fragment shaders
    bool succeeded = shaderProgram->addShader((const char*) vertexShader.toUTF8(), GL_VERTEX_SHADER);
    if (!succeeded)
    {
        jassertfalse;
        return;
    }

    succeeded = shaderProgram->addShader((const char*) fragmentShader.toUTF8(), GL_FRAGMENT_SHADER);
    if (!succeeded)
    {
        jassertfalse;
        return;
    }

    succeeded = shaderProgram->link();
    if (!succeeded)
    {
        jassertfalse;
        return;
    }

    positionAttribute = new OpenGLShaderProgram::Attribute(*shaderProgram, "Position");
    sourceColorAttribute = new OpenGLShaderProgram::Attribute(*shaderProgram, "SourceColor");

    samplerLocationUniform = new OpenGLShaderProgram::Uniform(*shaderProgram, "s_texture");

    juceLogoTextureId = TexturedComponent::createTextureForImageFile("droplet_png");

    /*
    std::cerr << "Position attribute id <" << positionAttribute->attributeID << ">"
              << " Source Color attribute id <" << sourceColorAttribute->attributeID << ">"
              << std::endl;
    */
}

void PointSpriteComponent::openGLContextClosing()
{
}


void PointSpriteComponent::renderOpenGL()
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

    const GLfloat squareVertices[] = {
        -0.5f, -0.5f,
        0.5f,  -0.5f,
        -0.5f,  0.5f,
        0.5f,   0.5f,
    };

    const GLubyte squareColors[] = {
        0xFF, 0x00, 0x00, 255,
        0xFF, 0x00, 0x00, 255,
        0x00, 0x00, 0xFF, 255,
        0x00, 0x00, 0xFF, 255,
    };

    // update attribute values
    glVertexAttribPointer(positionAttribute->attributeID, 2, GL_FLOAT, 0, 0, squareVertices);
    glEnableVertexAttribArray(positionAttribute->attributeID);
    glVertexAttribPointer(sourceColorAttribute->attributeID, 4, GL_UNSIGNED_BYTE, 1, 0, squareColors); //enable the normalized flag
    glEnableVertexAttribArray(sourceColorAttribute->attributeID);
        
    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, juceLogoTextureId );

    // Set the sampler texture unit to 0
    glUniform1i ( samplerLocationUniform->uniformID, 0 );

    // draw
    glDrawArrays(GL_POINTS, 0, 4);
}


