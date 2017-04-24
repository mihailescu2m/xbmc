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

#include "Compositor.h"

namespace xw = xbmc::wayland;

xw::Compositor::Compositor(struct wl_compositor *compositor) :
  m_compositor(compositor)
{
}

xw::Compositor::~Compositor()
{
  wl_compositor_destroy(m_compositor);
}

struct wl_compositor *
xw::Compositor::GetWlCompositor()
{
  return m_compositor;
}

struct wl_surface *
xw::Compositor::CreateSurface() const
{
  return wl_compositor_create_surface(m_compositor);
}

struct wl_region *
xw::Compositor::CreateRegion() const
{
  return wl_compositor_create_region(m_compositor);
}
