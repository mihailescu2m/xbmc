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

#include "Registry.h"

namespace xw = xbmc::wayland;

const struct wl_registry_listener xw::Registry::m_listener =
{
  Registry::HandleGlobalCallback,
  Registry::HandleRemoveGlobalCallback
};

void
xw::ExtraWaylandGlobals::NewGlobal(struct wl_registry *registry,
                                   uint32_t name,
                                   const char *interface,
                                   uint32_t version)
{
  if (m_handler)
    m_handler(registry, name, interface, version);
}

xw::ExtraWaylandGlobals &
xw::ExtraWaylandGlobals::GetInstance()
{
  if (!m_instance)
    m_instance.reset(new ExtraWaylandGlobals());

  return *m_instance;
}

std::unique_ptr<xw::ExtraWaylandGlobals> xw::ExtraWaylandGlobals::m_instance;

/* We inject an IWaylandRegistration here which is a virtual
 * class which a callback for the global objects
 * used by xbmc. Once one of those objects becomes
 * available, we call the callback function on that
 * interface. If it returns false, then it means that the main
 * xbmc implementation isn't interested in that object, so we 
 * call out to a listener that can be bound to by any client code
 * (as it is a singleton) to see if that code is interested
 * in the interface and wants to bind to it. This is particularly
 * useful for testing purposes where custom objects on the compositor
 * side are used. */
xw::Registry::Registry(struct wl_display *display,
                       IWaylandRegistration &registration) :
  m_registry(wl_display_get_registry(display)),
  m_registration(registration)
{
  wl_registry_add_listener(m_registry, &m_listener,
                           reinterpret_cast<void *>(this));
}

void
xw::Registry::HandleGlobal(uint32_t name,
                           const char *interface,
                           uint32_t version)
{
  /* Check if our injected listener wants to know about this -
   * otherwise let any external listeners know */
  if (!m_registration.OnGlobalInterfaceAvailable(name,
                                                 interface,
                                                 version))
  {
    ExtraWaylandGlobals::GetInstance().NewGlobal(m_registry,
                                                 name,
                                                 interface,
                                                 version);
  }
}

void
xw::Registry::HandleRemoveGlobal(uint32_t name)
{
}

void
xw::Registry::HandleGlobalCallback(void *data,
                                   struct wl_registry *registry,
                                   uint32_t name,
                                   const char *interface,
                                   uint32_t version)
{
  static_cast<Registry *>(data)->HandleGlobal(name, interface, version);
}

void
xw::Registry::HandleRemoveGlobalCallback(void *data,
                                         struct wl_registry *registry,
                                         uint32_t name)
{
  static_cast<Registry *>(data)->HandleRemoveGlobal(name);
}
