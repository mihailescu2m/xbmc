#include "WinSystemGbm.h"

#include <string.h>

#include "guilib/GraphicContext.h"
#include "settings/DisplaySettings.h"
#include "utils/log.h"

CWinSystemGbm::CWinSystemGbm() :
  m_drm(nullptr)
{
  m_eWindowSystem = WINDOW_SYSTEM_GBM;
}

bool CWinSystemGbm::InitWindowSystem()
{
  m_drm = CGBMUtils::InitDrm();

  if (!m_drm)
  {
    CLog::Log(LOGERROR, "CWinSystemGbm::%s - failed to initialize DRM", __FUNCTION__);
    return false;
  }

  CLog::Log(LOGDEBUG, "CWinSystemGbm::%s - initialized DRM", __FUNCTION__);
  return CWinSystemBase::InitWindowSystem();
}

bool CWinSystemGbm::DestroyWindowSystem()
{
  CGBMUtils::DestroyDrm();
  m_drm = nullptr;

  CLog::Log(LOGDEBUG, "CWinSystemGbm::%s - deinitialized DRM", __FUNCTION__);
  return true;
}

bool CWinSystemGbm::CreateNewWindow(const std::string& name,
                                    bool fullScreen,
                                    RESOLUTION_INFO& res,
                                    PHANDLE_EVENT_FUNC userFunction)
{
  if (!CGBMUtils::InitGbm(res))
  {
    CLog::Log(LOGERROR, "CWinSystemGbm::%s - failed to initialize GBM", __FUNCTION__);
    return false;
  }

  CLog::Log(LOGDEBUG, "CWinSystemGbm::%s - initialized GBM", __FUNCTION__);
  return true;
}

bool CWinSystemGbm::DestroyWindow()
{
  CGBMUtils::DestroyGbm();

  CLog::Log(LOGDEBUG, "CWinSystemGbm::%s - deinitialized GBM", __FUNCTION__);
  return true;
}

void CWinSystemGbm::UpdateResolutions()
{
  CWinSystemBase::UpdateResolutions();

  UpdateDesktopResolution(CDisplaySettings::GetInstance().GetResolutionInfo(RES_DESKTOP),
			  0,
			  m_drm->mode->hdisplay,
			  m_drm->mode->vdisplay,
			  m_drm->mode->vrefresh);

  std::vector<RESOLUTION_INFO> resolutions;

  if (!CGBMUtils::GetModes(resolutions) || resolutions.empty())
  {
    CLog::Log(LOGWARNING, "CWinSystemGbm::%s - Failed to get resolutions", __FUNCTION__);
  }
  else
  {
    for (auto i = 0; i < resolutions.size(); i++)
    {
      g_graphicsContext.ResetOverscan(resolutions[i]);
      CDisplaySettings::GetInstance().AddResolutionInfo(resolutions[i]);

      CLog::Log(LOGNOTICE, "Found resolution for display %d with %dx%d%s @ %f Hz",
		resolutions[i].iScreen,
		resolutions[i].iScreenWidth,
		resolutions[i].iScreenHeight,
		resolutions[i].dwFlags & D3DPRESENTFLAG_INTERLACED ? "i" : "",
		resolutions[i].fRefreshRate);
    }
  }

  CDisplaySettings::GetInstance().ApplyCalibrations();
}

bool CWinSystemGbm::ResizeWindow(int newWidth, int newHeight, int newLeft, int newTop)
{
  return true;
}

bool CWinSystemGbm::SetFullScreen(bool fullScreen, RESOLUTION_INFO& res, bool blankOtherDisplays)
{
  auto ret = CGBMUtils::SetVideoMode(res);

  if (!ret)
  {
    return false;
  }

  return true;
}

bool CWinSystemGbm::Hide()
{
  return false;
}

bool CWinSystemGbm::Show(bool raise)
{
  return true;
}

void CWinSystemGbm::Register(IDispResource * /*resource*/)
{
}

void CWinSystemGbm::Unregister(IDispResource * /*resource*/)
{
}
