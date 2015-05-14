#ifndef VertexAttributes_H
#define VertexAttributes_H

#include "JuceHeader.h"

#include "VertexBuffer.h"

class VertexAttributes
{
public:
    VertexAttributes(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
    {
        position      = createAttribute(openGLContext, shader, "position");
        normal        = createAttribute(openGLContext, shader, "normal");
        vertexColor  = createAttribute(openGLContext, shader, "vertexColor");
        texureCoordinates = createAttribute (openGLContext, shader, "texureCoordinates");
    }

    void enable(OpenGLContext& openGLContext)
    {
        if (position != nullptr)
        {
            openGLContext.extensions.glVertexAttribPointer(position->attributeID, 
                                                           3, 
                                                           GL_FLOAT, 
                                                           GL_FALSE, 
                                                           sizeof(VertexBuffer::Vertex),
                                                           0);
            openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);
        }

        if (normal != nullptr)
        {
            openGLContext.extensions.glVertexAttribPointer(normal->attributeID, 
                                                           3, 
                                                           GL_FLOAT, 
                                                           GL_FALSE, 
                                                           sizeof(VertexBuffer::Vertex),
                                                           (GLvoid*)(sizeof(float) * 3));
            openGLContext.extensions.glEnableVertexAttribArray(normal->attributeID);
        }

        if (vertexColor != nullptr)
        {
            openGLContext.extensions.glVertexAttribPointer(vertexColor->attributeID, 
                                                           4, 
                                                           GL_FLOAT, 
                                                           GL_FALSE, 
                                                           sizeof(VertexBuffer::Vertex),(GLvoid*)(sizeof(float) * 6));
            openGLContext.extensions.glEnableVertexAttribArray(vertexColor->attributeID);
        }

        if (texureCoordinates != nullptr)
        {
            openGLContext.extensions.glVertexAttribPointer(texureCoordinates->attributeID, 
                                                           2, 
                                                           GL_FLOAT, 
                                                           GL_FALSE, 
                                                           sizeof(VertexBuffer::Vertex),(GLvoid*)(sizeof(float) * 10));
            openGLContext.extensions.glEnableVertexAttribArray(texureCoordinates->attributeID);
        }
    }

    void disable(OpenGLContext& openGLContext)
    {
        if (position != nullptr)       openGLContext.extensions.glDisableVertexAttribArray(position->attributeID);
        if (normal != nullptr)         openGLContext.extensions.glDisableVertexAttribArray(normal->attributeID);
        if (vertexColor != nullptr)   openGLContext.extensions.glDisableVertexAttribArray(vertexColor->attributeID);
        if (texureCoordinates != nullptr)   openGLContext.extensions.glDisableVertexAttribArray(texureCoordinates->attributeID);
    }

    ScopedPointer<OpenGLShaderProgram::Attribute> position;
    ScopedPointer<OpenGLShaderProgram::Attribute> vertexColor;
    ScopedPointer<OpenGLShaderProgram::Attribute> normal;
    ScopedPointer<OpenGLShaderProgram::Attribute> texureCoordinates;

private:
    static OpenGLShaderProgram::Attribute* createAttribute(OpenGLContext& openGLContext,
                                                            OpenGLShaderProgram& shader,
                                                            const char* attributeName)
    {
        if (openGLContext.extensions.glGetAttribLocation(shader.getProgramID(), attributeName) < 0)
            return nullptr;

        return new OpenGLShaderProgram::Attribute(shader, attributeName);
    }
};

#endif
