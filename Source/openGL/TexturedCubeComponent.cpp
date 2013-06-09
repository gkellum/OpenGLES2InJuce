#include "TexturedCubeComponent.h"

#include "ResourceManager.h"

#include <cmath>

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#endif

#define PI 3.14159265

typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2]; // New
} Vertex;

#define TEX_COORD_MAX   4

const Vertex Vertices[] = {
    // Front
    {{1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{-1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
    // Back
    {{1, 1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{-1, -1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{1, -1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, 1, -2}, {0, 0, 0, 1}, {0, 0}},
    // Left
    {{-1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}}, 
    {{-1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, -1, -2}, {0, 0, 0, 1}, {0, 0}},
    // Right
    {{1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
    // Top
    {{1, 1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
    {{-1, 1, 0}, {0, 0, 0, 1}, {0, 0}},
    // Bottom
    {{1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
    {{1, -1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
    {{-1, -1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}}, 
    {{-1, -1, -2}, {0, 0, 0, 1}, {0, 0}}
};

const GLubyte Indices[] = {
    // Front
    0, 1, 2,
    2, 3, 0,
    // Back
    4, 5, 6,
    6, 7, 4,
    // Left
    8, 9, 10,
    10, 11, 8,
    // Right
    12, 13, 14,
    14, 15, 12,
    // Top
    16, 17, 18,
    18, 19, 16,
    // Bottom
    20, 21, 22,
    22, 23, 20
};

const Vertex Vertices2[] = {
    {{0.5, -0.5, 0.01}, {1, 1, 1, 1}, {1, 1}},
    {{0.5, 0.5, 0.01}, {1, 1, 1, 1}, {1, 0}},
    {{-0.5, 0.5, 0.01}, {1, 1, 1, 1}, {0, 0}},
    {{-0.5, -0.5, 0.01}, {1, 1, 1, 1}, {0, 1}},
};

const GLubyte Indices2[] = {
    1, 0, 2, 3
};

TexturedCubeComponent::TexturedCubeComponent()
  : openGLContext()
  , gridTextureId(-1)
  , juceLogoTextureId(-1)
  , cubesVerticesId(-1)
  , cubesIndicesId(-1)
  , juceLogosVerticesId(-1)
  , juceLogosIndicesId(-1)
{
    openGLContext.setRenderer(this);
    openGLContext.attachTo(*this);
}

TexturedCubeComponent::~TexturedCubeComponent()
{
    openGLContext.detach();
}

GLuint TexturedCubeComponent::createTextureForImageFile( char *fileName )
{
    Image juceLogoImage = ResourceManager::getImageFromFile(fileName);

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

void TexturedCubeComponent::newOpenGLContextCreated()
{
    // Create the program for the vertex and fragment shaders
    shaderProgram = new OpenGLShaderProgram(openGLContext);

    // The source files for the shaders are in the directory Resources/Shaders
    String vertexShader = ResourceManager::getShaderFromFile("cube.vsh");
    String fragmentShader = ResourceManager::getShaderFromFile("cube.fsh");

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

    // We need to pull out the attribute ids and uniform ids from the shaders so that we can set
    // values to them...
    positionAttribute = new OpenGLShaderProgram::Attribute(*shaderProgram, "Position");
    sourceColorAttribute = new OpenGLShaderProgram::Attribute(*shaderProgram, "SourceColor");
    textureCoordinatesAttribute = new OpenGLShaderProgram::Attribute(*shaderProgram, "TexCoordIn");

    projectionUniform = new OpenGLShaderProgram::Uniform(*shaderProgram, "Projection");
    modelViewUniform = new OpenGLShaderProgram::Uniform(*shaderProgram, "Modelview");
    textureUniform = new OpenGLShaderProgram::Uniform(*shaderProgram, "Texture");

    // Load images and create textures for them.
    gridTextureId = createTextureForImageFile("grid.png");
    juceLogoTextureId = createTextureForImageFile("jucelogo96.png");

    // Load the vertices into vertex buffers...
    glGenBuffers(1, &cubesVerticesId);
    glBindBuffer(GL_ARRAY_BUFFER, cubesVerticesId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &cubesIndicesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubesIndicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &juceLogosVerticesId);
    glBindBuffer(GL_ARRAY_BUFFER, juceLogosVerticesId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
    
    glGenBuffers(1, &juceLogosIndicesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, juceLogosIndicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

}

void TexturedCubeComponent::openGLContextClosing()
{
}


void TexturedCubeComponent::renderOpenGL()
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

    Matrix3D<GLfloat> matrix;

    // 1
//    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
        
    glBindBuffer(GL_ARRAY_BUFFER, cubesVerticesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubesIndicesId);
    
    // 2
    glVertexAttribPointer(positionAttribute->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(sourceColorAttribute->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
    
    glVertexAttribPointer(textureCoordinatesAttribute->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 7));    
    
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, gridTextureId);
    glUniform1i(textureUniform->uniformID, 0);
    
    // 3
    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, juceLogosVerticesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, juceLogosIndicesId);

    glActiveTexture(GL_TEXTURE0); // unneccc in practice
    glBindTexture(GL_TEXTURE_2D, juceLogoTextureId);
    glUniform1i(textureUniform->uniformID, 0); // unnecc in practice

    glUniformMatrix4fv(modelViewUniform->uniformID, 1, 0, matrix.mat);

    glVertexAttribPointer(positionAttribute->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(sourceColorAttribute->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
    glVertexAttribPointer(textureCoordinatesAttribute->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 7));

    glDrawElements(GL_TRIANGLE_STRIP, sizeof(Indices2)/sizeof(Indices2[0]), GL_UNSIGNED_BYTE, 0);
    
//    [_context presentRenderbuffer:GL_RENDERBUFFER];
}


