#ifndef OpenGLIncludes_H
#define OpenGLIncludes_H

#if OS_MACIOS
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES2/gl.h>
#elif OS_MACOSX
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
#endif

#endif