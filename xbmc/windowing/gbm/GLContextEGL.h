#pragma once

#include "EGL/egl.h"
#include "gbm.h"

class CGLContextEGL
{
public:
  CGLContextEGL();
  virtual ~CGLContextEGL();

  bool CreateDisplay(gbm_device* connection,
                     EGLint renderable_type,
                     EGLint rendering_api);

  bool CreateSurface(gbm_surface* surface);
  bool CreateContext();
  bool BindContext();
  void Destroy();
  void Detach();
  void SwapBuffers();

  EGLDisplay m_eglDisplay;
  EGLSurface m_eglSurface;
  EGLContext m_eglContext;
  EGLConfig m_eglConfig;
};
