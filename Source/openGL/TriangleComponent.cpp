#include "TriangleComponent.h"

#include "ResourceManager.h"

#include <cmath>

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#endif

#define PI 3.14159265

TriangleComponent::TriangleComponent()
  : openGLContext()

{
    openGLContext.setRenderer(this);
    openGLContext.attachTo(*this);
}

TriangleComponent::~TriangleComponent()
{
    openGLContext.detach();
}

void TriangleComponent::newOpenGLContextCreated()
{
    shaderProgram = new OpenGLShaderProgram(openGLContext);

    // The source files for the shaders are in the directory Resources/Shaders
    String vertexShader = ResourceManager::getShaderFromFile("triangle.vsh");
#ifdef OS_MACIOS
    String fragmentShader = ResourceManager::getShaderFromFile("triangle.fsh");
#else
    String fragmentShader = ResourceManager::getShaderFromFile("triangle_desktop.fsh");
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

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

void TriangleComponent::openGLContextClosing()
{
}


void TriangleComponent::renderOpenGL()
{
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


