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

#include "Surface.h"

namespace xw = xbmc::wayland;

std::unique_ptr<xw::WaylandSurfaceListener> xw::WaylandSurfaceListener::m_instance;

xw::WaylandSurfaceListener &
xw::WaylandSurfaceListener::GetInstance()
{
  if (!m_instance)
    m_instance.reset(new WaylandSurfaceListener());

  return *m_instance;
}

void
xw::WaylandSurfaceListener::SetHandler(const Handler &handler)
{
  m_handler = handler;
}

void
xw::WaylandSurfaceListener::SurfaceCreated(xw::Surface &surface)
{
  if (m_handler)
    m_handler(surface);
}

xw::Surface::Surface(struct wl_surface *surface) :
  m_surface(surface)
{
  WaylandSurfaceListener::GetInstance().SurfaceCreated(*this);
}

xw::Surface::~Surface()
{
  wl_surface_destroy(m_surface);
}

struct wl_surface *
xw::Surface::GetWlSurface()
{
  return m_surface;
}

struct wl_callback *
xw::Surface::CreateFrameCallback()
{
  return wl_surface_frame(m_surface);
}

void
xw::Surface::SetOpaqueRegion(struct wl_region *region)
{
  wl_surface_set_opaque_region(m_surface, region);
}

void
xw::Surface::Commit()
{
  wl_surface_commit(m_surface);
}
