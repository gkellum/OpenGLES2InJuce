#ifndef __LineComponent_H__
#define __LineComponent_H__

#include "JuceHeader.h"

#include "VertexBuffer.h"
#include "VertexRenderer.h"

#include <memory>

class GraphicObject : public Component
{
public:
    typedef std::shared_ptr<GraphicObject> Ptr;

    GraphicObject(VertexBuffer::Ptr vertexBuffer,
                  VertexRenderer::Ptr renderer,
                  const c4::Matrix4D& modelMatrix);
    ~GraphicObject();

    const BoundingBox& getBoundingBox();

    const c4::Matrix4D& getModelTransformation() const;
    void setModelTransformation(const c4::Matrix4D& modelMatrix);

    void render(OpenGLContext& openGLContext,
                const c4::Matrix4D& viewMatrix,
                const c4::Matrix4D& projectionMatrix);

    void shutdown();

private:
    bool initialized;

    VertexBuffer::Ptr vertexBuffer;
    VertexRenderer::Ptr renderer;

    c4::Matrix4D modelMatrix;
};

#endif