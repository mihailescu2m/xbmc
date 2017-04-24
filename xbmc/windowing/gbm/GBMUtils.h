#pragma once

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <vector>

#include "guilib/Resolution.h"

struct gbm {
    struct gbm_device *dev;
    struct gbm_surface *surface;
    int width, height;
};

struct crtc {
    drmModeCrtc *crtc;
    drmModeObjectProperties *props;
    drmModePropertyRes **props_info;
};

struct connector {
    drmModeConnector *connector;
    drmModeObjectProperties *props;
    drmModePropertyRes **props_info;
};

struct drm {
    int fd;

    struct gbm *gbm;

    struct crtc *crtc;
    struct connector *connector;
    int crtc_index;

    drmModeModeInfo *mode;
    uint32_t crtc_id;
    uint32_t connector_id;
};

struct drm_fb {
    struct gbm_bo *bo;
    uint32_t fb_id;
};

class CGBMUtils
{
public:
  static drm * InitDrm();
  static bool InitGbm(RESOLUTION_INFO res);
  static void DestroyGbm();
  static bool SetVideoMode(RESOLUTION_INFO res);
  static void FlipPage();
  static void DestroyDrm();
  static bool GetModes(std::vector<RESOLUTION_INFO> &resolutions);
private:
  static bool GetMode(RESOLUTION_INFO res);
  static bool GetResources();
  static bool GetConnector();
  static bool GetEncoder();
  static bool GetPreferredMode();
  static bool RestoreOriginalMode();
  static bool WaitingForFlip();
  static bool QueueFlip();
  static void PageFlipHandler(int fd, unsigned int frame, unsigned int sec, unsigned int usec, void *data);
  static void DrmFbDestroyCallback(struct gbm_bo *bo, void *data);
  static drm_fb * DrmFbGetFromBo(struct gbm_bo *bo);
};
