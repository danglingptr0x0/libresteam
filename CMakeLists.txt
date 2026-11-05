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

set(LIBRESTEAM_PLATFORM_DIR "src/${LIBRESTEAM_ARCH}/${LIBRESTEAM_PLATFORM}")

set(LIBRESTEAM_INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include/libresteam")
set(LIBRESTEAM_INCLUDE_DIRS
    "${LIBRESTEAM_INCLUDE_ROOT}/none/none"
)
foreach(libresteam_inc_sub "${LIBRESTEAM_ARCH}/none" "none/${LIBRESTEAM_PLATFORM}" "${LIBRESTEAM_ARCH}/${LIBRESTEAM_PLATFORM}")
    if(EXISTS "${LIBRESTEAM_INCLUDE_ROOT}/${libresteam_inc_sub}")
        list(APPEND LIBRESTEAM_INCLUDE_DIRS "${LIBRESTEAM_INCLUDE_ROOT}/${libresteam_inc_sub}")
    endif()
endforeach()

set(LIBRESTEAM_SOURCES
    src/none/none/steam_id/steam_id.c
    src/none/none/game_id/game_id.c
    src/none/none/backend/backend.c
    src/none/none/user_stats/user_stats.c
    src/none/none/friends/friends.c
    src/none/none/cloud/cloud.c
    src/none/none/lobby/lobby.c
    src/none/none/srv_browser/srv_browser.c
    src/none/none/cb/cb.c
    src/none/none/backend/steam/steam.c
    src/none/none/backend/steam/user_stats.c
    src/none/none/user_stats/ext.c
    src/none/none/backend/steam/user_stats_ext.c
    src/none/none/backend/steam/friends.c
    src/none/none/backend/steam/cloud.c
    src/none/none/backend/steam/user.c
    src/none/none/user/user.c
    src/none/none/apps/apps.c
    src/none/none/backend/steam/apps.c
    src/none/none/utils/utils.c
    src/none/none/backend/steam/utils.c
    src/none/none/music/music.c
    src/none/none/backend/steam/music.c
    src/none/none/timeline/timeline.c
    src/none/none/backend/steam/timeline.c
    src/none/none/friends/ext.c
    src/none/none/backend/steam/friends_ext.c
    src/none/none/backend/steam/lobby.c
    src/none/none/backend/steam/srv_browser.c
    src/none/none/net/net.c
    src/none/none/net_utils/net_utils.c
    src/none/none/backend/steam/net_utils.c
    src/none/none/net_socks/net_socks.c
    src/none/none/backend/steam/net_socks.c
    src/none/none/net_msgs/net_msgs.c
    src/none/none/backend/steam/net_msgs.c
    src/none/none/wshop/wshop.c
    src/none/none/backend/steam/wshop.c
    src/none/none/inventory/inventory.c
    src/none/none/backend/steam/inventory.c
    src/none/none/remote_play/remote_play.c
    src/none/none/backend/steam/remote_play.c
    src/none/none/app_id/app_id.c
    src/none/none/wire/wire.c
    src/none/none/backend/srv/srv.c
    src/none/none/backend/srv/user_stats.c
    src/none/none/backend/srv/friends.c
    src/none/none/backend/srv/cloud.c
    src/none/none/backend/srv/user.c
)

if(LIBRESTEAM_PLATFORM STREQUAL "linux" OR LIBRESTEAM_PLATFORM STREQUAL "windows")
    list(APPEND LIBRESTEAM_SOURCES
        ${LIBRESTEAM_PLATFORM_DIR}/backend/steam/path.c
    )
endif()

add_library(libresteam SHARED ${LIBRESTEAM_SOURCES})
set_target_properties(libresteam PROPERTIES
    PREFIX ""
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    C_VISIBILITY_PRESET hidden
    STD_YESREALLYDONOTSTRIPTHISWINDOWSBINARY ON
)
foreach(libresteam_inc_dir ${LIBRESTEAM_INCLUDE_DIRS})
    target_include_directories(libresteam PUBLIC $<BUILD_INTERFACE:${libresteam_inc_dir}>)
endforeach()
target_include_directories(libresteam PUBLIC $<INSTALL_INTERFACE:include>)
target_include_directories(libresteam SYSTEM PUBLIC ${DANGLING_INCLUDE_DIRS})
target_link_libraries(libresteam PUBLIC ${LIBRESTEAM_DANGLING_SHARED_LIBRARIES})
if(LIBRESTEAM_PLATFORM STREQUAL "windows")
    set_target_properties(libresteam PROPERTIES IMPORT_PREFIX "" IMPORT_SUFFIX ".dll.a")
    target_link_options(libresteam PRIVATE "LINKER:--out-implib,${CMAKE_CURRENT_BINARY_DIR}/libresteam.dll.a")
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/extras.cmake" OPTIONAL)

install(TARGETS libresteam
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
foreach(libresteam_inc_dir ${LIBRESTEAM_INCLUDE_DIRS})
    install(DIRECTORY "${libresteam_inc_dir}/libresteam" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endforeach()
