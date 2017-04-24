#pragma once

#include "GLContextEGL.h"
#include "rendering/gles/RenderSystemGLES.h"
#include "utils/GlobalsHandling.h"
#include "WinSystemGbm.h"

class CWinSystemGbmGLESContext : public CWinSystemGbm, public CRenderSystemGLES
{
public:
  CWinSystemGbmGLESContext() = default;
  virtual ~CWinSystemGbmGLESContext() = default;

  bool InitWindowSystem() override;
  bool CreateNewWindow(const std::string& name,
                       bool fullScreen,
                       RESOLUTION_INFO& res,
                       PHANDLE_EVENT_FUNC userFunction) override;

  bool SetFullScreen(bool fullScreen, RESOLUTION_INFO& res, bool blankOtherDisplays) override;
  EGLDisplay GetEGLDisplay() const;
  EGLSurface GetEGLSurface() const;
  EGLContext GetEGLContext() const;
  EGLConfig  GetEGLConfig() const;
protected:
  void SetVSyncImpl(bool enable) override { return; };
  void PresentRenderImpl(bool rendered) override;

private:
  CGLContextEGL m_pGLContext;

};

XBMC_GLOBAL_REF(CWinSystemGbmGLESContext, g_Windowing);
#define g_Windowing XBMC_GLOBAL_USE(CWinSystemGbmGLESContext)
