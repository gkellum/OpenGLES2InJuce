#include "GLCallBatcher.h"

#include "FreeTypeBitmapChar.h"
#include "OpenGLEnvironment.h"

#include <cassert>
#include <iostream>

#if !defined(OS_MACIOS)
  #ifndef glOrthof
    #define glOrthof glOrtho
  #endif
  #ifndef glTexParameterx
    #define glTexParameterx glTexParameteri
  #endif
#endif

using namespace std;

const int dataPerVextex = 5;
GLCallBatcher::GLCallBatcher()
{
    m_numQuads = 0;
    m_pVertexData = NULL;
    m_pIndices = NULL;
    m_textureID = 0;
    m_sizeCache = 0;
    m_drawCallCount = 0;
    // assume that we will have at least 150 quads strung together
    Reallocate(150);
}

GLCallBatcher::~GLCallBatcher()
{
    Release();
}

// deletes memory for arrays
void GLCallBatcher::Release()
{
    if (m_pVertexData != NULL)
    {
        delete[] m_pVertexData;
        m_pVertexData = NULL;
    }
    if (m_pIndices != NULL)
    {
        delete[] m_pIndices;
        m_pIndices = NULL;
    }
    m_sizeCache = 0;
}

// adds a quad of a give colour, alpha, etc
void GLCallBatcher::addQuad(OpenGLEnvironment& environment,
                            GLuint textureID, int color, float alpha, bool transparent, bool smooth,
                            const float* pTexCoords, const float* pVertices) 
{
    setAttributes(environment, textureID, color, alpha, transparent, smooth);
    addQuad(environment, pTexCoords, pVertices);
}   // Render

// adds a quad and assumes that the colour, alpha is same as previous
void GLCallBatcher::addQuad(OpenGLEnvironment& environment, const float* pTexCoords, const float* pVertices) 
{
    if (m_numQuads >= m_sizeCache)
    {
        Reallocate(20);
    }
    
    int currIndex = m_numQuads*verticesPerQuad*dataPerVextex;
    for (int n = 0; n < verticesPerQuad; n++)
    {
        // x,y,z: note: do not need to copy 3rd coordinate, just zero it
        m_pVertexData[currIndex+n*dataPerVextex] = pVertices[n*compVertPos];
        m_pVertexData[currIndex+n*dataPerVextex+1] = pVertices[n*compVertPos+1];
        // u,v
        m_pVertexData[currIndex+n*dataPerVextex+3] = pTexCoords[n*compVertTex];
        m_pVertexData[currIndex+n*dataPerVextex+4] = pTexCoords[n*compVertTex+1];
    }
    m_numQuads++;
}   // Render

// sets the current colour, alpha, etc,
// if this is different than previous then the current quads have to be rendered
// before adding any new ones
void GLCallBatcher::setAttributes(OpenGLEnvironment& environment, 
                                  GLuint textureID, int color, float alpha, bool transparent, bool smooth) 
{
    if (alpha > 1.0f) alpha = 1.0f;
    if (m_textureID != textureID || m_color != color || m_alpha != alpha ||
        m_transparent != transparent || m_smooth != smooth)
    {
        render(environment);
        m_textureID = textureID;
        m_color = color;
        m_alpha = alpha;
        m_transparent = transparent;
        m_smooth = smooth;
    }
}   // Render

// render the currently stored quads
void GLCallBatcher::render(OpenGLEnvironment& environment)
{
    if (m_numQuads == 0) return;

    glDisable(GL_LIGHTING);
    glFrontFace(GL_CW);

    glEnable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // set orthographic, so that we are drawing billboard style using screen dimensions 
    glOrthof(0, (float) environment.getCanvasWidth(), (float) environment.getCanvasHeight(), 0, -50.0f, 50.0f);
    glActiveTexture(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glClientActiveTexture(GL_TEXTURE1);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_DEPTH_TEST);   
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    if (m_transparent || m_alpha < 0.99f)
    {
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
        glDisable(GL_BLEND);    
    glBindTexture(GL_TEXTURE_2D, m_textureID); 
    if (m_smooth)
    {
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);
    }
    else
    {
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
    }
    if (m_alpha > 0.99f && m_color == WHITE)
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }
    else
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//      glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB);
        const float inv256 = 1.0f/256.0f;
        float red = ((m_color>>16)&0xff)*inv256;
        float green = ((m_color>>8)&0xff)*inv256;
        float blue = (m_color&0xff)*inv256;
        if (m_alpha > 1.0f) m_alpha = 1.0f;
        glColor4f(red, green, blue, m_alpha);
    }
    /*
     Set the vertex pointer.
     param 1: Number of coordinates per vertex; must be 2, 3, or 4.
     param 2: GL_FIXED for CommonLite and GL_FLOAT for Common profile.
     param 3: Specifies the byte offset between consecutive vertexes. 
     param 4: Offset to the start of the first vertex into the VBO.
     */
    glVertexPointer(3, GL_FLOAT, sizeof(float)*dataPerVextex, m_pVertexData); 
    glTexCoordPointer(compVertTex, GL_FLOAT, sizeof(float)*dataPerVextex, 
                      &m_pVertexData[3]);
    assert(m_numQuads*indicesPerQuad < 0xffff);
    glDrawElements(GL_TRIANGLES, (GLsizei)(m_numQuads*indicesPerQuad), 
        GL_UNSIGNED_SHORT, m_pIndices);
    m_drawCallCount++;
    m_quadCount += m_numQuads;
    m_numQuads = 0;
    // revert back to previous state
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
#ifdef _DEBUG
    int err = glGetError();
    if (err != GL_NO_ERROR)
    {
        cout << "OpenGL error!!!" << endl;
    }
#endif
}

// increase the memory alloacation
// new memory is allocated and old memory is copied, then old memory is deleted
void GLCallBatcher::Reallocate(int increase)
{
    int newSize = m_numQuads+increase;
    float* pVertices = new float[newSize*verticesPerQuad*dataPerVextex];    // x,y,z
    memset(pVertices, 0, newSize*verticesPerQuad*dataPerVextex*sizeof(float));
    GLushort* pIndices = new GLushort[newSize*indicesPerQuad];   // 2 triangles per quad
    // indices never change
    for (int n = 0; n < newSize; n++)
    {
        pIndices[n*indicesPerQuad] = (GLshort)n*verticesPerQuad;
        pIndices[n*indicesPerQuad+1] = (GLshort)n*verticesPerQuad+1;
        pIndices[n*indicesPerQuad+2] = (GLshort)n*verticesPerQuad+2;
        pIndices[n*indicesPerQuad+3] = (GLshort)n*verticesPerQuad+1;
        pIndices[n*indicesPerQuad+4] = (GLshort)n*verticesPerQuad+3;
        pIndices[n*indicesPerQuad+5] = (GLshort)n*verticesPerQuad+2;
    }
    if (m_numQuads > 0)
    {
        int numVert = m_numQuads*verticesPerQuad;
        memcpy(pVertices, m_pVertexData, numVert*dataPerVextex*sizeof(float));
//      memcpy(pTexCoords, m_pTexCoords, numVert*compVertTex*sizeof(float));
        Release();
    }
    m_pVertexData = pVertices;
//  m_pTexCoords = pTexCoords;
    m_pIndices = pIndices;
    m_sizeCache = newSize;
}

// output the statistics
void GLCallBatcher::OutputData(ostringstream& os) 
{
    os << " draw calls: " << m_drawCallCount << " quads:" << m_quadCount;
}
