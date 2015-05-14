#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(OpenGLContext& context, 
                           Array<Vertex>& vertices,
                           Array<juce::uint32>& indices,
                           GLuint drawMode)
  : VertexBuffer(context, vertices, indices, calculateBoundingBox(vertices), drawMode)
{
}

VertexBuffer::VertexBuffer(OpenGLContext& context, 
                           Array<Vertex>& vertices,
                           Array<juce::uint32>& indices,
                           const BoundingBox& boundingBox,
                           GLuint drawMode)
  : openGLContext(context)
  , boundingBox(boundingBox)
  , vertexBuffer(0)
  , indexBuffer(0)
  , numberOfIndices(indices.size())
  , drawMode(drawMode)
{
    openGLContext.extensions.glGenBuffers(1, &vertexBuffer);
    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, 
                                          vertices.size() * sizeof(Vertex),
                                          vertices.getRawDataPointer(), 
                                          GL_STATIC_DRAW);

    openGLContext.extensions.glGenBuffers(1, &indexBuffer);
    openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                                          indices.size() * sizeof(juce::uint32),
                                          indices.getRawDataPointer(), 
                                          GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    openGLContext.extensions.glDeleteBuffers(1, &vertexBuffer);
    openGLContext.extensions.glDeleteBuffers(1, &indexBuffer);
}

BoundingBox VertexBuffer::calculateBoundingBox(Array<Vertex>& vertices)
{
    if (vertices.size() == 0)
        return BoundingBox();

    BoundingBox box(vertices.getReference(0).position[0],
                    vertices.getReference(0).position[0],
                    vertices.getReference(0).position[1],
                    vertices.getReference(0).position[1],
                    vertices.getReference(0).position[2],
                    vertices.getReference(0).position[2]);

    int numberOfVertices = vertices.size();
    for (int indexOfVertex = 0; indexOfVertex < numberOfVertices; indexOfVertex++)
    {
        if (box.minimumPoint.x > vertices.getReference(indexOfVertex).position[0])
        {
            box.minimumPoint.x = vertices.getReference(indexOfVertex).position[0];
        }

        if (box.maximumPoint.x < vertices.getReference(indexOfVertex).position[0])
        {
            box.maximumPoint.x = vertices.getReference(indexOfVertex).position[0];
        }

        if (box.minimumPoint.y > vertices.getReference(indexOfVertex).position[1])
        {
            box.minimumPoint.y = vertices.getReference(indexOfVertex).position[1];
        }

        if (box.maximumPoint.y < vertices.getReference(indexOfVertex).position[1])
        {
            box.maximumPoint.y = vertices.getReference(indexOfVertex).position[1];
        }

        if (box.minimumPoint.z > vertices.getReference(indexOfVertex).position[2])
        {
            box.minimumPoint.z = vertices.getReference(indexOfVertex).position[2];
        }

        if (box.maximumPoint.z < vertices.getReference(indexOfVertex).position[2])
        {
            box.maximumPoint.z = vertices.getReference(indexOfVertex).position[2];
        }
    }

    return box;
}

const BoundingBox& VertexBuffer::getBoundingBox()
{
    return boundingBox;
}

void VertexBuffer::bind()
{
    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

void VertexBuffer::draw()
{
    glDrawElements(drawMode, numberOfIndices, GL_UNSIGNED_INT, 0);
}

void VertexBuffer::unbind()
{
    // Reset the element buffers so child Components draw correctly
    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
