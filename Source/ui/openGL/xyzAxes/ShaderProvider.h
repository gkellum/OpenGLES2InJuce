#ifndef ShaderProvider_H
#define ShaderProvider_H

#include "JuceHeader.h"

#include "ShaderUniforms.h"

#include <memory>

#if JUCE_OPENGL_ES
    #define LOWP " lowp "
#else
    #define LOWP ""
#endif

class ShaderProvider
{
public:
#if !_LIBCPP_VERSION
    typedef std::tr1::shared_ptr<ShaderProvider> Ptr;
#else
    typedef std::shared_ptr<ShaderProvider> Ptr;
#endif

    virtual ~ShaderProvider() {}

    virtual String createVertexShader() =0;
    virtual String createFragmentShader() =0;

    virtual void initialise(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram) {}
    virtual void shutdown() {}

    virtual void prepareForRender(OpenGLContext& openGLContext, ShaderUniforms& uniforms) {}
};

#endif
