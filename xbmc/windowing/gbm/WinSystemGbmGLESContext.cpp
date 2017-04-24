#include "WinSystemGbmGLESContext.h"
#include "utils/log.h"

bool CWinSystemGbmGLESContext::InitWindowSystem()
{
  if (!CWinSystemGbm::InitWindowSystem())
  {
    return false;
  }

  if (!m_pGLContext.CreateDisplay(m_drm->gbm->dev,
                                  EGL_OPENGL_ES2_BIT,
                                  EGL_OPENGL_ES_API))
  {
    return false;
  }

  return true;
}

bool CWinSystemGbmGLESContext::CreateNewWindow(const std::string& name,
                                               bool fullScreen,
                                               RESOLUTION_INFO& res,
                                               PHANDLE_EVENT_FUNC userFunction)
{
  m_pGLContext.Detach();

  if (!CWinSystemGbm::DestroyWindow())
  {
    return false;
  }

  if (!CWinSystemGbm::CreateNewWindow(name, fullScreen, res, userFunction))
  {
    return false;
  }

  if (!m_pGLContext.CreateSurface(m_drm->gbm->surface))
  {
    return false;
  }

  if (!m_pGLContext.CreateContext())
  {
    return false;
  }

  if (!m_pGLContext.BindContext())
  {
    return false;
  }

  return true;
}

bool CWinSystemGbmGLESContext::SetFullScreen(bool fullScreen, RESOLUTION_INFO& res, bool blankOtherDisplays)
{
  if (res.iWidth != m_drm->mode->hdisplay ||
      res.iHeight != m_drm->mode->vdisplay)
  {
    CLog::Log(LOGDEBUG, "CWinSystemGbmGLESContext::%s - resolution changed, creating a new window", __FUNCTION__);
    CreateNewWindow("", fullScreen, res, nullptr);
  }

  m_pGLContext.SwapBuffers();

  CWinSystemGbm::SetFullScreen(fullScreen, res, blankOtherDisplays);
  CRenderSystemGLES::ResetRenderSystem(res.iWidth, res.iHeight, fullScreen, res.fRefreshRate);

  return true;
}

void CWinSystemGbmGLESContext::PresentRenderImpl(bool rendered)
{
  if (rendered)
  {
    m_pGLContext.SwapBuffers();
    CGBMUtils::FlipPage();
  }
}

EGLDisplay CWinSystemGbmGLESContext::GetEGLDisplay() const
{
  return m_pGLContext.m_eglDisplay;
}

EGLSurface CWinSystemGbmGLESContext::GetEGLSurface() const
{
  return m_pGLContext.m_eglSurface;
}

EGLContext CWinSystemGbmGLESContext::GetEGLContext() const
{
  return m_pGLContext.m_eglContext;
}

EGLConfig  CWinSystemGbmGLESContext::GetEGLConfig() const
{
  return m_pGLContext.m_eglConfig;
}
