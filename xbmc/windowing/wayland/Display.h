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

#include <wayland-client.h>

#include <functional>
#include <memory>

struct wl_display;
struct wl_callback;

typedef struct wl_display * EGLNativeDisplayType;

namespace xbmc
{
namespace wayland
{
class Display
{
  public:

    Display();
    ~Display();

    Display(const Display &) = delete;
    Display &operator=(const Display &) = delete;

    struct wl_display * GetWlDisplay() {
      return m_display;
    }

    EGLNativeDisplayType* GetEGLNativeDisplay() {
      return &m_display;
    }

    /* Create a sync callback object. This can be wrapped in an
     * xbmc::wayland::Callback object to call an arbitrary function
     * as soon as the display has finished processing all commands.
     *
     * This does not block until a synchronization is complete -
     * consider using a function like WaitForSynchronize to do that
     */
    struct wl_callback * Sync() {
      return wl_display_sync(m_display);
    }

  private:

    struct wl_display *m_display;
};

/* This is effectively just a seam for testing purposes so that
 * we can listen for extra objects that the core implementation might
 * not necessarily be interested in */
class WaylandDisplayListener
{
public:

  typedef std::function<void(Display &)> Handler;
  
  void SetHandler(const Handler &handler) {
    m_handler = handler;
  }

  void DisplayAvailable(Display &);

  static WaylandDisplayListener & GetInstance();
private:

  Handler m_handler;
  
  static std::unique_ptr<WaylandDisplayListener> m_instance;
};
}
}
