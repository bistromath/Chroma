######################################################################
# Module file extension
######################################################################
if (WIN32)
    set(MODULE_EXT "dll")
elseif (UNIX)
    set(MODULE_EXT "so")
else()
    message(FATAL_ERROR "Must be Win32 or Unix")
endif()

######################################################################
# Local includes
######################################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CHROMA_INCLUDE_DIRS})

######################################################################
# Euler includes
######################################################################
#include_directories(${EULER_INCLUDE_DIRS})
#list(APPEND CHROMA_LIBRARIES ${EULER_LIBRARIES})

list(APPEND CHROMA_SOURCES
    Types/spd.cpp
    Types/XYZ.cpp
    Data/StandardIlluminants.cpp
    #rgbw.cpp
    #cri.cpp
)

add_library(Chroma SHARED ${CHROMA_SOURCES})
target_link_libraries(Chroma ${CHROMA_LIBRARIES})
set_target_properties(Chroma PROPERTIES VERSION ${CHROMA_VERSION})
set_target_properties(Chroma PROPERTIES DEFINE_SYMBOL "CHROMA_DLL_EXPORTS")

install(TARGETS Chroma
    LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT chroma_runtime # .so
    ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT chroma_devel   # .lib
    RUNTIME DESTINATION bin              COMPONENT chroma_runtime # .dll
)

#get_target_property(CHROMA_LIBRARY_LOCATION Chroma LOCATION ${CMAKE_BUILD_TYPE})
#get_filename_component(CHROMA_LIBRARY_NAME ${CHROMA_LIBRARY_LOCATION} NAME)

######################################################################
# Configure and install pkgconfig files
######################################################################
#configure_file(
#    ${CMAKE_CURRENT_SOURCE_DIR}/Chroma.in.pc
#    ${CMAKE_CURRENT_BINARY_DIR}/Chroma.pc
#@ONLY)

#install(
#    FILES ${CMAKE_CURRENT_BINARY_DIR}/Chroma.pc
#    DESTINATION lib${LIB_SUFFIX}/pkgconfig
#    COMPONENT chroma_devel
#)
