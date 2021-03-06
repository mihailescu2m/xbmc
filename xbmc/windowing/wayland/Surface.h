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
#include <functional>
#include <memory>

#include <wayland-client.h>

namespace xbmc
{
namespace wayland
{
class Surface
{
public:

  explicit Surface(struct wl_surface *surface);
  ~Surface() {
    wl_surface_destroy(m_surface);
  }

  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;

  struct wl_surface * GetWlSurface() {
    return m_surface;
  }

  struct wl_callback * CreateFrameCallback() {
    return wl_surface_frame(m_surface);
  }

  void SetOpaqueRegion(struct wl_region *region) {
    wl_surface_set_opaque_region(m_surface, region);
  }

  void Commit() {
    wl_surface_commit(m_surface);
  }

private:

  struct wl_surface *m_surface;
};

/* This is effectively just a seam for testing purposes so that
 * we can listen for extra objects that the core implementation might
 * not necessarily be interested in. It isn't possible to get any
 * notification from within weston that a surface was created so
 * we need to rely on the client side in order to do that */
class WaylandSurfaceListener
{
public:

  typedef std::function<void(Surface &)> Handler;
  
  void SetHandler(const Handler &handler) {
    m_handler = handler;
  }

  void SurfaceCreated(Surface &);

  static WaylandSurfaceListener & GetInstance();
private:

  Handler m_handler;
  
  static std::unique_ptr<WaylandSurfaceListener> m_instance;
};
}
}
