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

namespace xbmc
{
namespace wayland
{
class Compositor
{
public:

  explicit Compositor(struct wl_compositor *compositor) :
    m_compositor(compositor)
  {
  }

  ~Compositor() {
    wl_compositor_destroy(m_compositor);
  }

  Compositor(const Compositor &) = delete;
  Compositor &operator=(const Compositor &) = delete;

  struct wl_compositor * GetWlCompositor() {
    return m_compositor;
  }
  
  /* Creates a "surface" on the compositor. This is not a renderable
   * surface immediately, a renderable "buffer" must be bound to it
   * (usually an EGL Window) */
  struct wl_surface * CreateSurface() const {
    return wl_compositor_create_surface(m_compositor);
  }
  
  /* Creates a "region" on the compositor side. Server side regions
   * are manipulated on the client side and then can be used to
   * affect rendering and input on the server side */
  struct wl_region * CreateRegion() const {
    return wl_compositor_create_region(m_compositor);
  }

private:

  struct wl_compositor *m_compositor;
};
}
}
