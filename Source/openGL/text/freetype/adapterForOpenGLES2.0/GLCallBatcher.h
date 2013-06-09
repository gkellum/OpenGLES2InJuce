#ifndef _GLCALLBATCHER_H_INCLUDED_
#define _GLCALLBATCHER_H_INCLUDED_

#include "JuceHeader.h"

#ifdef OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES1/glext.h>
#endif

#include <sstream>

class OpenGLEnvironment;

// AtlasImage objects send their quads to GLCallBatcher for rendering.
// GLCallBatcher strings the quads together into triangle lists.
// All quads with same alpha, colour, transpancy and texture are rendered together 
// with one call to glDrawElements
class GLCallBatcher
{
public:
    GLCallBatcher();
    ~GLCallBatcher();

    void addQuad(OpenGLEnvironment& environment, const float* pTexCoords, const float* pVertices);

    void render(OpenGLEnvironment& environment);

    void OutputData(std::ostringstream& os);

    void ResetStats()
    {
        m_drawCallCount = 0;
        m_quadCount = 0;
    }
    void setAttributes(OpenGLEnvironment& environment, GLuint textureID, int color, float alpha, bool transparent, bool smooth);

    void Release();

private:
    void addQuad(OpenGLEnvironment& environment,
                 GLuint textureID, int color, float alpha, bool transparent, bool smooth,
                 const float* pTexCoords, const float* pVertices);
    void Reallocate(int increase);
    float* m_pVertexData;       // storage of vertex array
    GLushort* m_pIndices;   // indices of quads
    int m_numQuads;         // current number of quads waiting to be rendered
    int m_sizeCache;        // current amount of quads that can fit in the arrays
    float m_alpha;          // alpha value of current quads
    int m_color;            // colour tint of current quads
    GLuint m_textureID;     // texture of current quads
    bool m_transparent;     // are current quads transparent
    int m_drawCallCount;    // statistical info
    int m_quadCount;
    bool m_smooth;
};





#endif // _GLCALLBATCHER_H_INCLUDED_
