#ifndef ShaderUniforms_H
#define ShaderUniforms_H

#include "JuceHeader.h"

#include <memory>

struct ShaderUniforms
{
    typedef std::shared_ptr<ShaderUniforms> Ptr;

    ShaderUniforms(OpenGLContext& openGLContext, 
                   OpenGLShaderProgram& shader)
    {
        projectionMatrixUniform = createUniform(openGLContext, shader, "projectionMatrix");
        modelViewMatrixUniform = createUniform(openGLContext, shader, "modelViewMatrix");
        modelViewProjectionMatrixUniform = createUniform(openGLContext, shader, "modelViewProjectionMatrix");
        lightsModelViewMatrixUniform = createUniform(openGLContext, shader, "lightsModelViewMatrix");
        textureIDUniform = createUniform(openGLContext, shader, "textureID");
        colorTextureMixUniform = createUniform(openGLContext, shader, "colorTextureMix");
    }

    void setMatrices(const c4::Matrix4D& modelMatrix,
                     const c4::Matrix4D& viewMatrix,
                     const c4::Matrix4D& projectionMatrix)
    {
        if (projectionMatrixUniform != nullptr)
        {
            projectionMatrixUniform->setMatrix4( (float*) projectionMatrix.n, 1, true);
        }
        
        c4::Matrix4D modelViewMatrix = modelMatrix * viewMatrix;
        if (modelViewMatrixUniform != nullptr)
        {
            modelViewMatrixUniform->setMatrix4( (float*) modelViewMatrix.n, 1, true);
        }

        if (modelViewProjectionMatrixUniform != nullptr)
        {
            c4::Matrix4D modelViewProjectionMatrix = modelViewMatrix * projectionMatrix;
            modelViewProjectionMatrixUniform->setMatrix4( (float*) modelViewProjectionMatrix.n, 1, true);
        }

        if (lightsModelViewMatrixUniform != nullptr)
        {
            lightsModelViewMatrixUniform->setMatrix4( (float*) modelViewMatrix.n, 1, true);
        }
    }

    void setTextureID(int textureID)
    {
        if (textureIDUniform != nullptr)
        {
            if (textureID >= 0)
            {
                textureIDUniform->set(textureID);
                if (colorTextureMixUniform != nullptr)
                {
                    colorTextureMixUniform->set(1.0f);
                }
            }
            else
            {
                textureIDUniform->set(textureID);
                if (colorTextureMixUniform != nullptr)
                {
                    colorTextureMixUniform->set(0.0f);
                }
            }
        }
    }

    ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrixUniform;
    ScopedPointer<OpenGLShaderProgram::Uniform> modelViewMatrixUniform;
    ScopedPointer<OpenGLShaderProgram::Uniform> modelViewProjectionMatrixUniform;
    ScopedPointer<OpenGLShaderProgram::Uniform> lightsModelViewMatrixUniform;
    ScopedPointer<OpenGLShaderProgram::Uniform> textureIDUniform;
    ScopedPointer<OpenGLShaderProgram::Uniform> colorTextureMixUniform;

private:
    static OpenGLShaderProgram::Uniform* createUniform(OpenGLContext& openGLContext,
                                                       OpenGLShaderProgram& shader,
                                                       const char* uniformName)
    {
        if (openGLContext.extensions.glGetUniformLocation(shader.getProgramID(), uniformName) < 0)
            return nullptr;

        return new OpenGLShaderProgram::Uniform(shader, uniformName);
    }
};

#endif
