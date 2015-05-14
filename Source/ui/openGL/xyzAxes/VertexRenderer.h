#ifndef VertexRenderer_H
#define VertexRenderer_H

#include "JuceHeader.h"

#include <memory>

class VertexBuffer;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;

class VertexRenderer
{
public:
    typedef std::shared_ptr<VertexRenderer> Ptr;

    virtual ~VertexRenderer() {}

    virtual void initialise(OpenGLContext& openGLContext) {}

    virtual void render(OpenGLContext& openGLContext,
                        VertexBufferPtr vertexBuffer,
                        const c4::Matrix4D& modelMatrix,
                        const c4::Matrix4D& viewMatrix,
                        const c4::Matrix4D& projectionMatrix) =0;

    virtual void shutdown() {}
};

#endif
