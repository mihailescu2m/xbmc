#pragma once

/*
 *      Copyright (C) 2011-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#include <vector>

#include <wayland-client.h>

namespace xbmc
{
namespace wayland
{
struct Output
{
public:

  Output(struct wl_output *);
  ~Output() {
    wl_output_destroy(m_output);
  }

  Output(const Output &) = delete;
  Output &operator=(const Output &) = delete;

  struct ModeGeometry
  {
    int32_t width;
    int32_t height;
    int32_t refresh;
  };

  struct PhysicalGeometry
  {
    int32_t x;
    int32_t y;
    int32_t physicalWidth;
    int32_t physicalHeight;
    enum wl_output_subpixel subpixelArrangement;
    enum wl_output_transform outputTransformation;
  };

  struct wl_output * GetWlOutput() {
    return m_output;
  }

  /* It is a precondition violation to use the following four
   * functions when the first modes have not yet been received.
   * 
   * Use a synchronization point after creating this object
   * (eg, WaitForSynchronize() to ensure that the initial modes
   * are available */
  
  /* The "current" mode is the mode that the display is currently
   * using */
  const ModeGeometry & CurrentMode();
  
  /* The "preferred" mode is the mode most optimal to this output.
   * 
   * This is usually the maximum possible mode that this output
   * supports. All fullscreen windows should generally have a buffer
   * of this size in order to avoid scaling. */
  const ModeGeometry & PreferredMode();

  const std::vector <ModeGeometry> & AllModes() const {
    return m_modes;
  }

  /* The geometry represents the physical geometry of this monitor */
  const PhysicalGeometry & Geometry() const {
    return m_geometry;
  }
  
  /* The scale factor of this output is an integer value representing
   * the number of output pixels per hardware pixel. For instance,
   * if UI elements were scaled up to 1680x1050 and the monitor was
   * displaying at a native resolution of 3360x2100 when this would be
   * "2". This is useful for supporting HiDPI display modes where,
   * for instance we allocate a 3360x2100 buffer but display our UI
   * elements at 1680x1050 */
  uint32_t ScaleFactor() const {
    return m_scaleFactor;
  }

  static void GeometryCallback(void *,
                               struct wl_output *,
                               int32_t,
                               int32_t,
                               int32_t,
                               int32_t,
                               int32_t,
                               const char *,
                               const char *,
                               int32_t);
  static void ModeCallback(void *,
                           struct wl_output *,
                           uint32_t,
                           int32_t,
                           int32_t,
                           int32_t);
  static void ScaleCallback(void *,
                            struct wl_output *,
                            int32_t);
  static void DoneCallback(void *,
                           struct wl_output *);

private:

  static const wl_output_listener m_listener;

  void Geometry(int32_t x,
                int32_t y,
                int32_t physicalWidth,
                int32_t physicalHeight,
                int32_t subpixel,
                const char *make,
                const char *model,
                int32_t transform);
  void Mode(uint32_t flags,
            int32_t width,
            int32_t height,
            int32_t refresh);

  /* This function is called whenever the scaling factor for this
   * output changes. It there for clients to support HiDPI displays,
   * although unused as of present */
  void Scale(int32_t factor) {
    m_scaleFactor = factor;
  }

  void Done() {
  }

  struct wl_output *m_output;

  PhysicalGeometry m_geometry;
  std::vector<ModeGeometry> m_modes;

  uint32_t m_scaleFactor;

  /* Only one mode at a time can have the current or preferred
   * flags set, so only one pointer is set here */
  ModeGeometry m_current;
  ModeGeometry m_preferred;
  bool m_currentValid;
  bool m_preferredValid;
};
}
}
