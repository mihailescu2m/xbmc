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
#include <string>
#include <memory>
#include <functional>

#include <wayland-client.h>

namespace xbmc
{
namespace wayland
{
/* This is effectively just a seam for testing purposes so that
 * we can listen for extra objects that the core implementation might
 * not necessarily be interested in */
class ExtraWaylandGlobals
{
public:

  typedef std::function<void(struct wl_registry *,
                             uint32_t,
                             const char *,
                             uint32_t)> GlobalHandler;
  
  void SetHandler(const GlobalHandler &handler) {
    m_handler = handler;
  }

  void NewGlobal(struct wl_registry *,
                 uint32_t,
                 const char *,
                 uint32_t);

  static ExtraWaylandGlobals & GetInstance();
private:

  GlobalHandler m_handler;
  
  static std::unique_ptr<ExtraWaylandGlobals> m_instance;
};

class IWaylandRegistration
{
public:

  virtual ~IWaylandRegistration() {};

  virtual bool OnGlobalInterfaceAvailable(uint32_t,
                                          const char *,
                                          uint32_t) = 0;
};

class Registry
{
public:

  Registry(struct wl_display   *display,
           IWaylandRegistration &registration);
  ~Registry() {
    wl_registry_destroy(m_registry);
  }

  Registry(const Registry &) = delete;
  Registry &operator=(const Registry &) = delete;

  struct wl_registry * GetWlRegistry();
  
  template<typename Create>
  Create Bind(uint32_t name,
              const struct wl_interface *interface,
              uint32_t version)
  {
    void *object = BindInternal(name,
                                interface,
                                version);
    return reinterpret_cast<Create>(object);
  }

private:

  static const struct wl_registry_listener m_listener;

  static void HandleGlobalCallback(void *, struct wl_registry *,
                                   uint32_t, const char *, uint32_t);
  static void HandleRemoveGlobalCallback(void *, struct wl_registry *,
                                         uint32_t name);

  /* Once a global becomes available, we immediately bind to it here
   * and then notify the injected listener interface that the global
   * is available on a named object. This allows that interface to
   * respond to the arrival of the new global how it wishes */
  void *BindInternal(uint32_t name,
                     const struct wl_interface *interface,
                     uint32_t version) {
    return wl_registry_bind(m_registry, name, interface, version);
  }

  struct wl_registry *m_registry;
  IWaylandRegistration &m_registration;

  void HandleGlobal(uint32_t, const char *, uint32_t);
  void HandleRemoveGlobal(uint32_t);
};
}
}
