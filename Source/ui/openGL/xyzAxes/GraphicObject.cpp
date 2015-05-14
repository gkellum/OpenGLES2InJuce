#include "GraphicObject.h"

GraphicObject::GraphicObject(VertexBuffer::Ptr vertexBuffer,
                             VertexRenderer::Ptr renderer,
                             const c4::Matrix4D& modelMatrix)
  : vertexBuffer(vertexBuffer)
  , renderer(renderer)
  , modelMatrix(modelMatrix)
{
}

GraphicObject::~GraphicObject()
{
}

const BoundingBox& GraphicObject::getBoundingBox()
{
    return vertexBuffer->getBoundingBox();
}

const c4::Matrix4D& GraphicObject::getModelTransformation() const
{
    return modelMatrix;
}

void GraphicObject::setModelTransformation(const c4::Matrix4D& modelMatrix)
{
    this->modelMatrix = modelMatrix;
}

void GraphicObject::render(OpenGLContext& openGLContext,
                           const c4::Matrix4D& viewMatrix,
                           const c4::Matrix4D& projectionMatrix)
{
    renderer->render(openGLContext, vertexBuffer, modelMatrix, viewMatrix, projectionMatrix);
}

void GraphicObject::shutdown()
{
    renderer->shutdown();
}
