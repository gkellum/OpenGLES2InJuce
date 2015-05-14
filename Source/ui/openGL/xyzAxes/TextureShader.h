#ifndef TextureShader_H
#define TextureShader_H

#include "JuceHeader.h"

#include "ShaderProvider.h"

class TextureShader : public ShaderProvider
{
public:
    TextureShader(Image image) 
      : image(image) 
    {
    }

    ~TextureShader()
    {
        shutdown();
    }

    String createVertexShader()
    {
        String vertexShader =   
                                "uniform mat4 modelViewProjectionMatrix; \n"
                                "attribute vec3 position;   \n"
                                "attribute vec2 texureCoordinates;   \n"
                                "varying vec2 varying_texureCoordinates;     \n"
                                "void main()                  \n"
                                "{                            \n"
                                "    varying_texureCoordinates = texureCoordinates;  \n"
                                "    gl_Position = modelViewProjectionMatrix * vec4(position, 1); \n"
                                "}\n";
        return vertexShader;
    }

    String createFragmentShader()
    {
        String fragmentShader = 
                                #ifdef OS_MACIOS
                                "precision mediump float;                            \n"
                                #endif
                                "varying vec2 varying_texureCoordinates;             \n"
                                "uniform sampler2D textureID;                        \n"
                                "void main()                                         \n"
                                "{                                                   \n"
                                "    gl_FragColor = texture2D( textureID, varying_texureCoordinates ); \n"
                                "}                                                      \n";
        return fragmentShader;
    }

    GLuint createTextureForImageFile( Image image )
    {
        if ( image == Image::null )
        {
            jassertfalse;
            return 0;
        }

        openGLTexture = new OpenGLTexture();
        openGLTexture->loadImage (image);

        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        return openGLTexture->getTextureID();
    }

    void initialise(OpenGLContext& openGLContext,
                    OpenGLShaderProgram& shaderProgram)
    {
        textureId = createTextureForImageFile(image);
    }

    void shutdown()
    {
        openGLTexture = nullptr;
    }

    void prepareForRender(OpenGLContext& openGLContext,
                          ShaderUniforms& uniforms)
    {
        // Bind the texture
        glActiveTexture ( GL_TEXTURE0 );
        glBindTexture ( GL_TEXTURE_2D, textureId );

        uniforms.setTextureID(0);
    }

private:
    Image image;
    ScopedPointer<OpenGLTexture> openGLTexture;
    GLuint textureId;
};

#endif
