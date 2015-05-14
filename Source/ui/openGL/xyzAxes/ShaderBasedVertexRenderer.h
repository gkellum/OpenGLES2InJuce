#ifndef ShaderBasedVertexRenderer_H
#define ShaderBasedVertexRenderer_H

#include "JuceHeader.h"

#include "VertexRenderer.h"
#include "ShaderProvider.h"
#include "VertexAttributes.h"
#include "ShaderUniforms.h"

#include <vector>

class ShaderBasedVertexRenderer : public VertexRenderer
{
public:
    ShaderBasedVertexRenderer(ShaderProvider::Ptr shaderProvider) 
      : shaderProvider(shaderProvider) {}

    VertexAttributes& getVertexAttributes() { return *vertexAttributes; }

    ShaderUniforms& getShaderUniforms() { return *uniforms; }

    void initialise(OpenGLContext& openGLContext)
    {
        String vertexShader = shaderProvider->createVertexShader();
        String fragmentShader = shaderProvider->createFragmentShader();

        ScopedPointer<OpenGLShaderProgram> newShader(new OpenGLShaderProgram(openGLContext));

        if (newShader->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertexShader))
              && newShader->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragmentShader))
              && newShader->link())
        {
            vertexAttributes = nullptr;
            uniforms = nullptr;

            shaderProgram = newShader;
            shaderProgram->use();

            vertexAttributes = new VertexAttributes(openGLContext, *shaderProgram);

            uniforms = new ShaderUniforms(openGLContext, *shaderProgram);

            shaderProvider->initialise(openGLContext, *shaderProgram);
        }
        else
        {
            String lastError = newShader->getLastError();

            std::cerr << "Last error " << std::endl << lastError << std::endl;
        }
    }

    void render(OpenGLContext& openGLContext,
                VertexBuffer::Ptr vertexBuffer,
                const c4::Matrix4D& modelMatrix,
                const c4::Matrix4D& viewMatrix,
                const c4::Matrix4D& projectionMatrix)
    {
        shaderProgram->use();

        uniforms->setMatrices(modelMatrix, viewMatrix, projectionMatrix);

        shaderProvider->prepareForRender(openGLContext, *uniforms);

        vertexBuffer->bind();

        vertexAttributes->enable(openGLContext);

        vertexBuffer->draw();

        vertexAttributes->disable(openGLContext);

        vertexBuffer->unbind();
    }

    void shutdown()
    {
        shaderProgram = nullptr;
        vertexAttributes = nullptr;
        uniforms = nullptr;
        shaderProvider->shutdown();
    }

private:
    ShaderProvider::Ptr shaderProvider;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    ScopedPointer<VertexAttributes> vertexAttributes;
    ScopedPointer<ShaderUniforms> uniforms;
};

#endif