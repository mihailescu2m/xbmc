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

#include "utils/ScopeExit.hxx"

#include <xkbcommon/xkbcommon.h>

#include "input/linux/XKBCommonKeymap.h"
#include "Keyboard.h"

namespace xw = xbmc::wayland;

const struct wl_keyboard_listener xw::Keyboard::m_listener =
{
  Keyboard::HandleKeymapCallback,
  Keyboard::HandleEnterCallback,
  Keyboard::HandleLeaveCallback,
  Keyboard::HandleKeyCallback,
  Keyboard::HandleModifiersCallback
};

namespace
{
void DestroyXKBCommonContext(struct xkb_context *context)
{
  xkb_context_unref(context);
}
}

void
xw::Keyboard::XkbContextDeleter::operator()(struct xkb_context *c)
{
  DestroyXKBCommonContext(c);
}

xw::Keyboard::Keyboard(struct wl_keyboard *keyboard,
                       IKeyboardReceiver &receiver) :
  m_xkbCommonContext(CXKBKeymap::CreateXKBContext(),
                     XkbContextDeleter()),
  m_keyboard(keyboard),
  m_reciever(receiver)
{
  wl_keyboard_add_listener(m_keyboard, &m_listener, this);
}

void xw::Keyboard::HandleKeymapCallback(void *data,
                                        struct wl_keyboard *keyboard,
                                        uint32_t format,
                                        int fd,
                                        uint32_t size)
{
  static_cast <Keyboard *>(data)->HandleKeymap(format,
                                               fd,
                                               size);
}

void xw::Keyboard::HandleEnterCallback(void *data,
                                       struct wl_keyboard *keyboard,
                                       uint32_t serial,
                                       struct wl_surface *surface,
                                       struct wl_array *keys)
{
  static_cast<Keyboard *>(data)->HandleEnter(serial,
                                             surface,
                                             keys);
}

void xw::Keyboard::HandleLeaveCallback(void *data,
                                       struct wl_keyboard *keyboard,
                                       uint32_t serial,
                                       struct wl_surface *surface)
{
  static_cast<Keyboard *>(data)->HandleLeave(serial,
                                             surface);
}

void xw::Keyboard::HandleKeyCallback(void *data,
                                     struct wl_keyboard *keyboard,
                                     uint32_t serial,
                                     uint32_t time,
                                     uint32_t key,
                                     uint32_t state)
{
  static_cast<Keyboard *>(data)->HandleKey(serial,
                                           time,
                                           key,
                                           state);
}

void xw::Keyboard::HandleModifiersCallback(void *data,
                                           struct wl_keyboard *keyboard,
                                           uint32_t serial,
                                           uint32_t mods_depressed,
                                           uint32_t mods_latched,
                                           uint32_t mods_locked,
                                           uint32_t group)
{
  static_cast<Keyboard *>(data)->HandleModifiers(serial,
                                                 mods_depressed,
                                                 mods_latched,
                                                 mods_locked,
                                                 group);
}

/* Creates a new internal keymap representation for a serialized
 * keymap as represented in shared memory as referred to by fd.
 * 
 * Since the fd is sent to us via sendmsg(), the currently running
 * process has ownership over it. As such, it MUST close the file
 * descriptor after it has decided what to do with it in order to
 * avoid a leak.
 */
void xw::Keyboard::HandleKeymap(uint32_t format,
                                int fd,
                                uint32_t size)
{
  /* The file descriptor must always be closed */
  AtScopeExit(fd) {
    close(fd);
  };

  /* We don't understand anything other than xkbv1. If we get some
   * other keyboard, then we can't process keyboard events reliably
   * and that's a runtime error. */
  if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1)
    throw std::runtime_error("Server gave us a keymap we don't understand");

  bool successfullyCreatedKeyboard = false;
  
  /* Either throws or returns a valid xkb_keymap * */
  struct xkb_keymap *keymap =
    CXKBKeymap::ReceiveXKBKeymapFromSharedMemory(m_xkbCommonContext.get(),
                                                 fd,
                                                 size);

  AtScopeExit(&successfullyCreatedKeyboard, keymap)
  {
    if (!successfullyCreatedKeyboard)
      xkb_keymap_unref(keymap);
  };

  m_keymap.reset(new CXKBKeymap(keymap));
  
  successfullyCreatedKeyboard = true;

  m_reciever.UpdateKeymap(m_keymap.get());
}

void xw::Keyboard::HandleEnter(uint32_t serial,
                               struct wl_surface *surface,
                               struct wl_array *keys)
{
  m_reciever.Enter(serial, surface, keys);
}

void xw::Keyboard::HandleLeave(uint32_t serial,
                               struct wl_surface *surface)
{
  m_reciever.Leave(serial, surface);
}

void xw::Keyboard::HandleKey(uint32_t serial,
                             uint32_t time,
                             uint32_t key,
                             uint32_t state)
{
  m_reciever.Key(serial,
                 time,
                 key,
                 static_cast<enum wl_keyboard_key_state>(state));
}

void xw::Keyboard::HandleModifiers(uint32_t serial,
                                   uint32_t mods_depressed,
                                   uint32_t mods_latched,
                                   uint32_t mods_locked,
                                   uint32_t group)
{
  m_reciever.Modifier(serial,
                      mods_depressed,
                      mods_latched,
                      mods_locked,
                      group);
}
