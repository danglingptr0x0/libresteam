cmake_minimum_required(VERSION 3.20)
project(libresteam VERSION 0.0.0 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "$ENV{HOME}/.config/cmake")
include(std OPTIONAL RESULT_VARIABLE LIBRESTEAM_STD_FILE)
set(STD_PROJ_PREFIX "libresteam")

if(NOT LIBRESTEAM_STD_FILE)
    message(STATUS "std: not found; building with CMake defaults")
endif()

include(GNUInstallDirs)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LIBRESTEAM_DANGLING_PREFIX "$ENV{HOME}/.local/x86_64-w64-mingw32" CACHE PATH "libdangling Windows install prefix")
    set(DANGLING_INCLUDE_DIRS "${LIBRESTEAM_DANGLING_PREFIX}/include")
    set(LIBRESTEAM_DANGLING_LIB "${LIBRESTEAM_DANGLING_PREFIX}/lib/libdangling.a")
    if(NOT EXISTS "${LIBRESTEAM_DANGLING_LIB}")
        message(FATAL_ERROR "libresteam: libdangling static library not found at ${LIBRESTEAM_DANGLING_LIB}")
    endif()
    set(LIBRESTEAM_DANGLING_IMPLIB "${LIBRESTEAM_DANGLING_PREFIX}/lib/libdangling.dll.a")
    if(NOT EXISTS "${LIBRESTEAM_DANGLING_IMPLIB}")
        message(FATAL_ERROR "libresteam: libdangling import library not found at ${LIBRESTEAM_DANGLING_IMPLIB}")
    endif()
    set(LIBRESTEAM_WIN_SYS_LIBRARIES kernel32 bcrypt ws2_32 advapi32 shell32 user32 runtimeobject ole32 setupapi winmm)
    set(DANGLING_LIBRARIES "${LIBRESTEAM_DANGLING_LIB}" ${LIBRESTEAM_WIN_SYS_LIBRARIES})
    set(LIBRESTEAM_DANGLING_SHARED_LIBRARIES "${LIBRESTEAM_DANGLING_IMPLIB}" ${LIBRESTEAM_WIN_SYS_LIBRARIES})
    add_link_options(-static)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DANGLING REQUIRED dangling)
    set(LIBRESTEAM_DANGLING_SHARED_LIBRARIES ${DANGLING_LIBRARIES})
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    set(LIBRESTEAM_ARCH "amd64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i386|i686)$")
    set(LIBRESTEAM_ARCH "i386")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "dangcsa64")
    set(LIBRESTEAM_ARCH "dangcsa")
else()
    message(FATAL_ERROR "libresteam: unsupported processor '${CMAKE_SYSTEM_PROCESSOR}'")
endif()

if(LIBRESTEAM_ARCH STREQUAL "i386" OR LIBRESTEAM_ARCH STREQUAL "dangcsa")
    set(LIBRESTEAM_PLATFORM "DANGLING")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LIBRESTEAM_PLATFORM "linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LIBRESTEAM_PLATFORM "windows")
else()
    message(FATAL_ERROR "libresteam: unsupported system '${CMAKE_SYSTEM_NAME}'")
endif()

set(LIBRESTEAM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(LIBRESTEAM_SOURCES
    src/steam_id/steam_id.c
    src/game_id/game_id.c
    src/backend/backend.c
    src/user/stats/stats.c
    src/friends/friends.c
    src/cloud/cloud.c
    src/lobby/lobby.c
    src/srv_browser/srv_browser.c
    src/cb/cb.c
    src/steam/steam.c
    src/steam/path.c
    src/steam/user/stats/stats.c
    src/user/stats/ext.c
    src/steam/user/stats/ext.c
    src/steam/friends/friends.c
    src/steam/cloud.c
    src/steam/user/user.c
    src/user/user.c
    src/apps/apps.c
    src/steam/apps.c
    src/utils/utils.c
    src/steam/utils.c
    src/music/music.c
    src/steam/music.c
    src/timeline/timeline.c
    src/steam/timeline.c
    src/friends/ext.c
    src/steam/friends/ext.c
    src/steam/lobby.c
    src/steam/srv_browser.c
    src/net/net.c
    src/net/utils/utils.c
    src/steam/net/utils.c
    src/net/socks/socks.c
    src/steam/net/socks.c
    src/net/msgs/msgs.c
    src/steam/net/msgs.c
    src/wshop/wshop.c
    src/steam/wshop.c
    src/inventory/inventory.c
    src/steam/inventory.c
    src/remote_play/remote_play.c
    src/steam/remote_play.c
    src/app_id/app_id.c
    src/wire/wire.c
    src/backend/srv/srv.c
    src/backend/srv/user/stats.c
    src/backend/srv/friends.c
    src/backend/srv/cloud.c
    src/backend/srv/user/user.c
    src/mod/mod.c
)

add_library(libresteam SHARED ${LIBRESTEAM_SOURCES})
set_target_properties(libresteam PROPERTIES
    PREFIX ""
    C_VISIBILITY_PRESET hidden
    STD_YESREALLYDONOTSTRIPTHISWINDOWSBINARY ON
)
target_include_directories(libresteam PUBLIC $<BUILD_INTERFACE:${LIBRESTEAM_INCLUDE_DIRS}> $<INSTALL_INTERFACE:include>)
target_include_directories(libresteam SYSTEM PUBLIC ${DANGLING_INCLUDE_DIRS})
target_link_options(libresteam PRIVATE "LINKER:--as-needed")
target_link_libraries(libresteam PRIVATE ${LIBRESTEAM_DANGLING_SHARED_LIBRARIES})
set(LIBRESTEAM_PC_LINK_NAME "libresteam${CMAKE_SHARED_LIBRARY_SUFFIX}")
if(LIBRESTEAM_PLATFORM STREQUAL "windows")
    set_target_properties(libresteam PROPERTIES IMPORT_PREFIX "" IMPORT_SUFFIX ".dll.a")
    target_link_options(libresteam PRIVATE "LINKER:--out-implib,${CMAKE_CURRENT_BINARY_DIR}/libresteam.dll.a")
    set(LIBRESTEAM_PC_LINK_NAME "libresteam.dll.a")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/extras.cmake" OPTIONAL)

if(TARGET libresteam-repl AND LIBRESTEAM_PLATFORM STREQUAL "windows")
    target_link_libraries(libresteam-repl PRIVATE ws2_32)
endif()

install(TARGETS libresteam
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libresteam.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/libresteam.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libresteam.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/libresteam" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
