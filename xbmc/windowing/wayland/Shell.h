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
class Shell
{
public:

  explicit Shell(struct wl_shell *shell) :
    m_shell(shell)
  {
  }

  ~Shell() {
    wl_shell_destroy(m_shell);
  }

  Shell(const Shell &) = delete;
  Shell &operator=(const Shell &) = delete;

  struct wl_shell * GetWlShell() {
    return m_shell;
  }

  struct wl_shell_surface * CreateShellSurface(struct wl_surface *surface) {
    return wl_shell_get_shell_surface(m_shell, surface);
  }

private:

  struct wl_shell *m_shell;
};
}
}
