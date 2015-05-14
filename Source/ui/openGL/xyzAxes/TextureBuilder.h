#ifndef TextureBuilder_H
#define TextureBuilder_H

#include "JuceHeader.h"

#include "VertexBuffer.h"
#include "ShaderBasedVertexRenderer.h"
#include "TextureShader.h"

class TextureBuilder
{
public:
    static GraphicObject::Ptr createTexture(OpenGLContext& context,
                                            Image image,
                                            float radius,
                                            const c4::Matrix4D& modelMatrix,
                                            const Colour& colourMask = Colours::transparentWhite)
    {
        Array<VertexBuffer::Vertex> vertices;

        {
            VertexBuffer::Vertex vertexA =
            {
                { -radius, -radius, 0.0, },
                { 0, 0, 0, },
                { colourMask.getFloatRed(), colourMask.getFloatGreen(), colourMask.getFloatBlue(), colourMask.getFloatAlpha() },
                { 0, 0, }
            };
            vertices.add(vertexA);
        }

        {
            VertexBuffer::Vertex vertexB =
            {
                { -radius, radius, 0.0, },
                { 0, 0, 0, },
                { colourMask.getFloatRed(), colourMask.getFloatGreen(), colourMask.getFloatBlue(), colourMask.getFloatAlpha() },
                { 0, 1, }
            };
            vertices.add(vertexB);
        }

        {
            VertexBuffer::Vertex vertexC =
            {
                { radius, radius, 0.0, },
                { 0, 0, 0, },
                { colourMask.getFloatRed(), colourMask.getFloatGreen(), colourMask.getFloatBlue(), colourMask.getFloatAlpha() },
                { 1, 1, }
            };
            vertices.add(vertexC);
        }

        {
            VertexBuffer::Vertex vertexD =
            {
                { radius, -radius, 0.0, },
                { 0, 0, 0, },
                { colourMask.getFloatRed(), colourMask.getFloatGreen(), colourMask.getFloatBlue(), colourMask.getFloatAlpha() },
                { 1, 0, }
            };
            vertices.add(vertexD);
        }

        Array<juce::uint32> indices;
        indices.add(0);
        indices.add(1);
        indices.add(2);
        indices.add(0);
        indices.add(2);
        indices.add(3);

        VertexBuffer::Ptr vertexBuffer(new VertexBuffer(context,
                                                        vertices,
                                                        indices,
                                                        GL_TRIANGLES));

        VertexRenderer::Ptr renderer(
            new ShaderBasedVertexRenderer( ShaderProvider::Ptr(new TextureShader(image)) )
        );
        renderer->initialise(context);

        return GraphicObject::Ptr(new GraphicObject(vertexBuffer,
                                                    renderer,
                                                    modelMatrix));
    }

    static Image createTextImage(const String& text, const Font& font)
    {
        int width = font.getStringWidth(text) + 5;
        int height = font.getHeight() + 2;
        int imageSize = nextPowerOfTwo(jmax(width, height));

        Image image(Image::ARGB, imageSize, imageSize, true, OpenGLImageType());

        Graphics g (image);

        g.setColour(Colours::white);
        int cornerRounding = 10;
        g.fillRoundedRectangle(0, 0, image.getWidth(), image.getHeight(), cornerRounding);
        g.setColour(Colours::black);
        g.drawRoundedRectangle(1, 1, image.getWidth()-2, image.getHeight()-2, cornerRounding, 1);
        g.setFont(font);
        int maximumNumberOfLines = 1;
        g.drawFittedText(text, 0, 0, image.getWidth(), image.getHeight(), Justification::centred, maximumNumberOfLines); 

        return image;
    }
};

#endif