#.rst:
# FindWayland
# -----------
# Finds the Wayland library
#
# This will will define the following variables::
#
# WAYLAND_FOUND        - the system has Wayland
# WAYLAND_INCLUDE_DIRS - the Wayland include directory
# WAYLAND_LIBRARIES    - the Wayland libraries
# WAYLAND_DEFINITIONS  - the Wayland definitions


if(PKG_CONFIG_FOUND)
  pkg_check_modules (PC_WAYLAND wayland-client wayland-egl xkbcommon QUIET)
endif()

find_path(WAYLAND_INCLUDE_DIR NAMES wayland-client.h
                          PATHS ${PC_WAYLAND_INCLUDE_DIRS})

find_library(WAYLAND_CLIENT_LIBRARY NAMES wayland-client
                         PATHS ${PC_WAYLAND_LIBRARIES} ${PC_WAYLAND_LIBRARY_DIRS})

find_library(WAYLAND_EGL_LIBRARY NAMES wayland-egl
                         PATHS ${PC_WAYLAND_LIBRARIES} ${PC_WAYLAND_LIBRARY_DIRS})

find_library(XKBCOMMON_LIBRARY NAMES xkbcommon
                         PATHS ${PC_WAYLAND_LIBRARIES} ${PC_WAYLAND_LIBRARY_DIRS})

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (WAYLAND
  REQUIRED_VARS
  WAYLAND_INCLUDE_DIR
  WAYLAND_CLIENT_LIBRARY
  WAYLAND_EGL_LIBRARY
  XKBCOMMON_LIBRARY)

if (WAYLAND_FOUND)
  set(WAYLAND_LIBRARIES ${WAYLAND_CLIENT_LIBRARY} ${WAYLAND_EGL_LIBRARY} ${XKBCOMMON_LIBRARY})
  set(WAYLAND_INCLUDE_DIRS ${PC_WAYLAND_INCLUDE_DIRS})
  set(WAYLAND_DEFINITIONS -DHAVE_WAYLAND=1)
endif()

mark_as_advanced (WAYLAND_LIBRARY WAYLAND_INCLUDE_DIR)
