#ifndef LineSegmentBuilder_H
#define LineSegmentBuilder_H

#include "JuceHeader.h"

#include "VertexBuffer.h"
#include "ShaderBasedVertexRenderer.h"
#include "VertexAttributes.h"
#include "ShaderUniforms.h"
#include "SimpleColorShader.h"

class LineSegmentBuilder
{
public:
    static GraphicObject::Ptr createLineSegment(OpenGLContext& context,
                                                const std::tuple<float, float, float>& pointA,
                                                const std::tuple<float, float, float>& pointB,
                                                const Colour& colour,
                                                const c4::Matrix4D& modelMatrix)
    {
        Array<VertexBuffer::Vertex> vertices;

        {
            VertexBuffer::Vertex vertexA =
            {
                { std::get<0>(pointA), std::get<1>(pointA), std::get<2>(pointA), },
                { 0, 0, 0, },
                { colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() },
                { 0, 0, }
            };
            vertices.add(vertexA);
        }

        {
            VertexBuffer::Vertex vertexB =
            {
                { std::get<0>(pointB), std::get<1>(pointB), std::get<2>(pointB), },
                { 0, 0, 0, },
                { colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() },
                { 0, 0, }
            };
            vertices.add(vertexB);
        }

        Array<juce::uint32> indices;
        indices.add(0);
        indices.add(1);

        VertexBuffer::Ptr vertexBuffer(new VertexBuffer(context,
                                                        vertices,
                                                        indices,
                                                        GL_LINES));

        VertexRenderer::Ptr renderer(
            new ShaderBasedVertexRenderer( ShaderProvider::Ptr(new SimpleColorShader()) )
        );
        renderer->initialise(context);

        return GraphicObject::Ptr(new GraphicObject(vertexBuffer,
                                                    renderer,
                                                    modelMatrix));
    }
};

#endif