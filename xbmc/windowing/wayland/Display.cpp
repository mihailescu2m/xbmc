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
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <cstdlib>

#include "Display.h"

namespace xw = xbmc::wayland;

void
xw::WaylandDisplayListener::DisplayAvailable(Display &display)
{
  if (m_handler)
    m_handler(display);
}

xw::WaylandDisplayListener &
xw::WaylandDisplayListener::GetInstance()
{
  if (!m_instance)
    m_instance.reset(new WaylandDisplayListener());

  return *m_instance;
}

std::unique_ptr<xw::WaylandDisplayListener> xw::WaylandDisplayListener::m_instance;

xw::Display::Display() :
  m_display(wl_display_connect(NULL))
{
  /* wl_display_connect won't throw when it fails, but it does
   * return NULL on failure. If this object would be incomplete
   * then that is a fatal error for the backend and we should
   * throw a runtime_error for the main connection manager to handle
   */
  if (!m_display)
  {
    std::stringstream ss;
    ss << "Failed to connect to wayland display";
    throw std::runtime_error(ss.str());
  }
  
  WaylandDisplayListener::GetInstance().DisplayAvailable(*this);
}

xw::Display::~Display()
{
  wl_display_flush(m_display);
  wl_display_disconnect(m_display);
}
