#include "SquareComponent.h"

#include "Settings.h"

#include "OpenGLIncludes.h"

#include <cmath>

SquareComponent::SquareComponent()
  : raiiOpenGLRenderer(this)
  , initialized(false)
{
}

SquareComponent::~SquareComponent()
{
}

void SquareComponent::newOpenGLContextCreated()
{
    initialized = true;

    shaderProgram = new OpenGLShaderProgram(Settings::getInstance().getOpenGLContext());

    // The source files for the shaders are in the directory Resources/Shaders
    String vertexShader =   "attribute vec4 Position; \r\n"
                            "attribute vec4 SourceColor; \r\n"
                            "varying vec4 DestinationColor;  \r\n"
                            "void main(void) {  \r\n"
                            "   DestinationColor = SourceColor;  \r\n"
                            "   gl_Position = Position; \r\n"
                            "} \r\n";

    String fragmentShader =     
                                #if defined(OS_MACIOS) || defined(OS_ANDROID)
                                "varying lowp vec4 DestinationColor;  \r\n"
                                #else
                                "varying vec4 DestinationColor;   \r\n"
                                #endif
                                "void main(void)  \r\n"
                                "   {  \r\n"
                                "   gl_FragColor = DestinationColor; \r\n"
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

    /*
    std::cerr << "Position attribute id <" << positionAttribute->attributeID << ">"
              << " Source Color attribute id <" << sourceColorAttribute->attributeID << ">"
              << std::endl;
    */
}

void SquareComponent::openGLContextClosing()
{
}


void SquareComponent::renderOpenGL()
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
        
    // draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


