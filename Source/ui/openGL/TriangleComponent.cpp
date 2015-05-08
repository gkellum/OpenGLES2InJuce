#include "TriangleComponent.h"

#include "Settings.h"

#include "OpenGLIncludes.h"

#include <cmath>

TriangleComponent::TriangleComponent()
  : raiiOpenGLRenderer(this)
  , initialized(false)
{
}

TriangleComponent::~TriangleComponent()
{
}

void TriangleComponent::newOpenGLContextCreated()
{
    shaderProgram = new OpenGLShaderProgram(Settings::getInstance().getOpenGLContext());

    // The source files for the shaders are in the directory Resources/Shaders
    String vertexShader =   "attribute vec4 vPosition;      \r\n"
                            "void main()                    \r\n"
                            "{                              \r\n"
                            "   gl_Position = vPosition;    \r\n"
                            "}                              \r\n";

    String fragmentShader =     
                                #if defined(OS_MACIOS) || defined(OS_ANDROID)
                                "precision mediump float;                       \r\n"
                                #endif
                                "void main()                                    \r\n"
                                "{                                              \r\n"
                                "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \r\n"
                                "}                                              \r\n";

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

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    initialized = true;
}

void TriangleComponent::openGLContextClosing()
{
}


void TriangleComponent::renderOpenGL()
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

    GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
                            -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f };

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Load the vertex data
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray ( 0 );

    glDrawArrays ( GL_TRIANGLES, 0, 3 );
}


