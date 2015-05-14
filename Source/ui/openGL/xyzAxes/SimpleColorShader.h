#ifndef SimpleColorShader_H
#define SimpleColorShader_H

#include "JuceHeader.h"

#include "ShaderProvider.h"

class SimpleColorShader : public ShaderProvider
{
public:
    SimpleColorShader() {}

    String createVertexShader()
    {
        String vertexShader =
            "uniform mat4 modelViewProjectionMatrix;\n"
            " \n"
            "attribute vec3 position;"
            "attribute vec4 vertexColor;"
            " \n"
            "varying vec4 varying_vertexColor;     \n"
            " \n"
            "void main()\n"
            "{\n"
            "  varying_vertexColor = vertexColor; \n"
            "  gl_Position = modelViewProjectionMatrix * vec4(position, 1);\n"
            "}\n";
        return vertexShader;
    }

    String createFragmentShader()
    {
        String fragmentShader =
            "varying vec4 varying_vertexColor;     \n"
            " \n"
            "void main()\n"
            "{\n"
            "    gl_FragColor = varying_vertexColor;\n"
            "}\n";
        return fragmentShader;
    }

private:
};

#endif
