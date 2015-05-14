#ifndef VertexBuffer_H
#define VertexBuffer_H

#include "JuceHeader.h"

#include "BoundingBox.h"

#include <memory>

class VertexBuffer
{
public:
    typedef std::shared_ptr<VertexBuffer> Ptr;

    struct Vertex
    {
        float position[3];
        float normal[3];
        float colour[4];
        float texCoord[2];
    };

    VertexBuffer(OpenGLContext& context, 
                 Array<Vertex>& vertices,
                 Array<juce::uint32>& indices,
                 GLuint drawMode = GL_TRIANGLES);

    VertexBuffer(OpenGLContext& context, 
                 Array<Vertex>& vertices,
                 Array<juce::uint32>& indices,
                 const BoundingBox& boundingBox,
                 GLuint drawMode = GL_TRIANGLES);

    ~VertexBuffer();

    const BoundingBox& getBoundingBox();

    void bind();
    void unbind();

    void draw();

private:
    BoundingBox calculateBoundingBox(Array<Vertex>& vertices);

    OpenGLContext& openGLContext;
    BoundingBox boundingBox;
    GLuint vertexBuffer, indexBuffer;
    size_t numberOfIndices;
    GLuint drawMode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VertexBuffer)
};

#endif

