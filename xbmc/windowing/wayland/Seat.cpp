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

#include "Seat.h"

namespace xw = xbmc::wayland;

/* We only support version 1 of this interface, the
 * other struct members are impliedly set to NULL */
const struct wl_seat_listener xw::Seat::m_listener =
{
  Seat::HandleCapabilitiesCallback
};

xw::Seat::Seat(struct wl_seat *seat,
               IInputReceiver &reciever) :
  m_seat(seat),
  m_input(reciever),
  m_currentCapabilities(static_cast<enum wl_seat_capability>(0))
{
  wl_seat_add_listener(m_seat, &m_listener, reinterpret_cast<void *>(this));
}

void xw::Seat::HandleCapabilitiesCallback(void *data,
                                          struct wl_seat *seat,
                                          uint32_t cap)
{
  enum wl_seat_capability capabilities =
    static_cast<enum wl_seat_capability>(cap);
  static_cast<Seat *>(data)->HandleCapabilities(capabilities);
}

/* The capabilities callback is effectively like a mini-registry
 * for all of the child objects of a Seat */
void xw::Seat::HandleCapabilities(enum wl_seat_capability cap)
{
  enum wl_seat_capability newCaps =
    static_cast<enum wl_seat_capability>(~m_currentCapabilities & cap);
  enum wl_seat_capability lostCaps =
    static_cast<enum wl_seat_capability>(m_currentCapabilities & ~cap);

  if (newCaps & WL_SEAT_CAPABILITY_POINTER)
  {
    m_input.InsertPointer(wl_seat_get_pointer(m_seat));
  }

  if (newCaps & WL_SEAT_CAPABILITY_KEYBOARD)
  {
    m_input.InsertKeyboard(wl_seat_get_keyboard(m_seat));
  }

  if (lostCaps & WL_SEAT_CAPABILITY_POINTER)
    m_input.RemovePointer();

  if (lostCaps & WL_SEAT_CAPABILITY_KEYBOARD)
    m_input.RemoveKeyboard();

  m_currentCapabilities = cap;
}
