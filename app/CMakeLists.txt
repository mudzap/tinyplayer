include(FindPkgConfig)

add_executable(tinyplayer ${SOURCES})

find_package(ALSA REQUIRED)
if (ALSA_FOUND)
    include_directories(${ALSA_INCLUDE_DIRS})
    target_link_libraries (tinyplayer ${ALSA_LIBRARIES})
endif(ALSA_FOUND)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GSTREAMER_AUDIO REQUIRED gstreamer-audio-1.0)
pkg_check_modules(GSTREAMER_BASE REQUIRED gstreamer-base-1.0)
pkg_check_modules(GSTREAMER_TAG REQUIRED gstreamer-tag-1.0)
pkg_check_modules(GSTREAMER_PBUTILS REQUIRED gstreamer-pbutils-1.0)

link_directories(${GSTREAMER_LIBRARY_DIRS})

find_library(BS2B NAMES libbs2b.a)
message(${BS2B})

target_include_directories(tinyplayer PRIVATE
        ${GSTREAMER_INCLUDE_DIRS}
        ${GSTREAMER_APP_INCLUDE_DIRS}
        ${GSTREAMER_AUDIO_INCLUDE_DIRS}
        ${GSTREAMER_BASE_INCLUDE_DIRS}
        ${GSTREAMER_TAG_INCLUDE_DIRS}
        ${GSTREAMER_PBUTILS_INCLUDE_DIRS}
        ${BS2B}
        )

target_link_libraries (tinyplayer
        ${GSTREAMER_BASE_LIBRARIES}
        ${GSTREAMER_LIBRARIES}
        ${GSTREAMER_APP_LIBRARIES}
        ${GSTREAMER_TAG_LIBRARIES}
        ${GSTREAMER_PBUTILS_LIBRARIES}
        ${BS2B}
        )