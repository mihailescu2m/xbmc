#pragma once

#include <gbm.h>

#include "threads/CriticalSection.h"
#include "windowing/WinSystem.h"
#include "GBMUtils.h"

class IDispResource;

class CWinSystemGbm : public CWinSystemBase
{
public:
  CWinSystemGbm();
  virtual ~CWinSystemGbm() = default;

  bool InitWindowSystem() override;
  bool DestroyWindowSystem() override;

  bool CreateNewWindow(const std::string& name,
                       bool fullScreen,
                       RESOLUTION_INFO& res,
                       PHANDLE_EVENT_FUNC userFunction) override;

  bool DestroyWindow() override;

  bool ResizeWindow(int newWidth, int newHeight, int newLeft, int newTop) override;
  bool SetFullScreen(bool fullScreen, RESOLUTION_INFO& res, bool blankOtherDisplays) override;

  void UpdateResolutions() override;

  bool Hide() override;
  bool Show(bool raise = true) override;
  virtual void Register(IDispResource *resource);
  virtual void Unregister(IDispResource *resource);

protected:
  drm* m_drm;
};
