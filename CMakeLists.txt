cmake_minimum_required(VERSION 3.16)
project(Pinball VERSION 0.7.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(Pinball
    src/main.cpp
    src/collisionMap.cpp
    src/paddle.cpp
    src/target.cpp
    src/targetHandler.cpp
)

target_include_directories(Pinball PRIVATE include)
target_compile_options(Pinball PRIVATE -O2 -W -Wall -Wextra -Wno-unused-parameter)

# --- Find Allegro 5 ---
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(ALLEGRO5
        allegro-5
        allegro_image-5
        allegro_primitives-5
    )
endif()

if(NOT ALLEGRO5_FOUND)
    # Fallback for Windows (vcpkg) or systems without pkg-config
    find_path(ALLEGRO5_INCLUDE_DIRS allegro5/allegro.h)
    find_library(ALLEGRO5_CORE_LIB      NAMES allegro allegro-5 allegro_monolith-debug allegro_monolith)
    find_library(ALLEGRO5_IMAGE_LIB     NAMES allegro_image allegro_image-5)
    find_library(ALLEGRO5_PRIM_LIB      NAMES allegro_primitives allegro_primitives-5)
    set(ALLEGRO5_LIBRARIES ${ALLEGRO5_CORE_LIB} ${ALLEGRO5_IMAGE_LIB} ${ALLEGRO5_PRIM_LIB})
    if(ALLEGRO5_INCLUDE_DIRS AND ALLEGRO5_CORE_LIB)
        set(ALLEGRO5_FOUND TRUE)
    endif()
endif()

if(NOT ALLEGRO5_FOUND)
    message(FATAL_ERROR
        "Allegro 5 not found.\n"
        "  Linux:  sudo apt install liballegro5-dev\n"
        "  macOS:  brew install allegro\n"
        "  Windows: vcpkg install allegro5:x64-windows")
endif()

target_include_directories(Pinball PRIVATE ${ALLEGRO5_INCLUDE_DIRS})
target_link_libraries(Pinball PRIVATE ${ALLEGRO5_LIBRARIES})
if(ALLEGRO5_LIBRARY_DIRS)
    target_link_directories(Pinball PRIVATE ${ALLEGRO5_LIBRARY_DIRS})
endif()
if(ALLEGRO5_CFLAGS_OTHER)
    target_compile_options(Pinball PRIVATE ${ALLEGRO5_CFLAGS_OTHER})
endif()

# --- Data directory ---
# Linux installed builds: binary in bin/, images in share/pinball/
# Windows / macOS: images placed next to the binary; ALLEGRO_RESOURCES_PATH resolves them at runtime.
if(UNIX AND NOT APPLE)
    set(PINBALL_DATA_INSTALL_DIR "share/pinball")
    target_compile_definitions(Pinball PRIVATE
        PINBALL_DATA_DIR="${CMAKE_INSTALL_PREFIX}/${PINBALL_DATA_INSTALL_DIR}")
    install(TARGETS Pinball DESTINATION bin)
    install(DIRECTORY images/ DESTINATION ${PINBALL_DATA_INSTALL_DIR})
else()
    # Windows / macOS: everything in one directory
    install(TARGETS Pinball DESTINATION .)
    install(DIRECTORY images/ DESTINATION .)
endif()

# --- CPack packaging ---
set(CPACK_PACKAGE_NAME            "Pinball")
set(CPACK_PACKAGE_VERSION         "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR          "Raymon Lange")
set(CPACK_PACKAGE_CONTACT         "raymon.lange@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Pinball arcade game")
set(CPACK_RESOURCE_FILE_LICENSE   "${CMAKE_CURRENT_SOURCE_DIR}/version.txt")

# DEB (Debian / Ubuntu)
set(CPACK_DEBIAN_PACKAGE_SECTION  "games")
set(CPACK_DEBIAN_PACKAGE_DEPENDS  "liballegro5.2 | liballegro5-2, liballegro-image5.2, liballegro-primitives5.2")

# RPM (Fedora / RHEL)
set(CPACK_RPM_PACKAGE_GROUP       "Amusements/Games")
set(CPACK_RPM_PACKAGE_REQUIRES    "allegro5")

# NSIS (Windows installer wizard)
set(CPACK_NSIS_DISPLAY_NAME       "Pinball")
set(CPACK_NSIS_PACKAGE_NAME       "Pinball")
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "Pinball.exe")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

# DragNDrop (macOS DMG)
set(CPACK_DMG_VOLUME_NAME         "Pinball")

include(CPack)
