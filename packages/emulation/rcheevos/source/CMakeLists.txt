cmake_minimum_required(VERSION 3.0...4.0)
project(rcheevos)

find_path(LIBRETRO_COMMON_INCLUDE_DIR
  NAMES libretro.h
  PATH_SUFFIXES libretro-common
)

if(NOT LIBRETRO_COMMON_INCLUDE_DIR)
  message(FATAL_ERROR "libretro-common headers not found")
endif()

# Compiling statically
add_definitions(-DRC_STATIC)

# Disable Lua for now
add_definitions(-DRC_DISABLE_LUA)

# RC_CLIENT_SUPPORTS_HASH gates rc_client_begin_identify_and_load_game(), which
# the add-on uses to identify a game without knowing its console up front. It
# has to be defined for the library too, not just for our own translation
# units, or the function is compiled out and the reference goes unresolved.
add_definitions(-DRC_CLIENT_SUPPORTS_HASH)

include_directories(
  include
  src
  ${LIBRETRO_COMMON_INCLUDE_DIR}
)

add_library(${PROJECT_NAME}lib
  src/rapi/rc_api_common.c
  src/rapi/rc_api_editor.c
  src/rapi/rc_api_info.c
  src/rapi/rc_api_runtime.c
  src/rapi/rc_api_user.c
  src/rc_client_external.c
  src/rc_client_raintegration.c
  src/rc_client.c
  src/rc_compat.c
  src/rc_libretro.c
  src/rc_util.c
  src/rc_version.c
  src/rcheevos/alloc.c
  src/rcheevos/condition.c
  src/rcheevos/condset.c
  src/rcheevos/consoleinfo.c
  src/rcheevos/format.c
  src/rcheevos/lboard.c
  src/rcheevos/memref.c
  src/rcheevos/operand.c
  src/rcheevos/rc_validate.c
  src/rcheevos/richpresence.c
  src/rcheevos/runtime_progress.c
  src/rcheevos/runtime.c
  src/rcheevos/trigger.c
  src/rcheevos/value.c
  src/rhash/aes.c
  src/rhash/cdreader.c
  src/rhash/hash_disc.c
  src/rhash/hash_encrypted.c
  src/rhash/hash_rom.c
  src/rhash/hash_zip.c
  src/rhash/hash.c
  src/rhash/md5.c
)

install(TARGETS
  ${PROJECT_NAME}lib
  ARCHIVE DESTINATION
    lib
  DESTINATION
    lib
)

install(
  FILES
    include/rc_api_editor.h
    include/rc_api_info.h
    include/rc_api_request.h
    include/rc_api_runtime.h
    include/rc_api_user.h
    include/rc_client_raintegration.h
    include/rc_client.h
    include/rc_consoles.h
    include/rc_error.h
    include/rc_export.h
    include/rc_hash.h
    include/rc_runtime_types.h
    include/rc_runtime.h
    include/rc_util.h
    include/rcheevos.h
    src/rc_libretro.h
  DESTINATION
    include/${PROJECT_NAME}
)
