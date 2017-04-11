if(OPENGLES_FOUND OR AML_FOUND OR MMAL_FOUND)
  set(SOURCES EGLWrapper.cpp
              EGLNativeTypeHybris.cpp)
  set(HEADERS EGLNativeType.h
              EGLQuirks.h
              EGLWrapper.h
              EGLNativeTypeHybris.h)
endif()

if(OPENGLES_FOUND)
    list(APPEND SOURCES WinSystemEGL.cpp)
    list(APPEND HEADERS WinSystemEGL.h)
endif()

if(CORE_SYSTEM_NAME STREQUAL android)
  list(APPEND SOURCES EGLNativeTypeAndroid.cpp)
  list(APPEND HEADERS EGLNativeTypeAndroid.h)
  list(APPEND SOURCES VideoSyncAndroid.h)
  list(APPEND SOURCES VideoSyncAndroid.cpp)
endif()

if(AML_FOUND)
  list(APPEND SOURCES EGLNativeTypeAmlogic.cpp)
  list(APPEND HEADERS EGLNativeTypeAmlogic.h)
  list(APPEND SOURCES VideoSyncAML.h)
  list(APPEND SOURCES VideoSyncAML.cpp)
endif()

if(MMAL_FOUND)
  list(APPEND SOURCES EGLNativeTypeRaspberryPI.cpp)
  list(APPEND HEADERS EGLNativeTypeRaspberryPI.h)
  list(APPEND SOURCES VideoSyncPi.cpp)
  list(APPEND HEADERS VideoSyncPi.h)
endif()

if(IMX_FOUND)
  list(APPEND SOURCES EGLNativeTypeIMX.cpp)
  list(APPEND HEADERS EGLNativeTypeIMX.h)
endif()

if(SOURCES)
  core_add_library(windowing_egl)
endif()
