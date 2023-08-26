cmake_minimum_required(VERSION 3.16.3...3.19.7 FATAL_ERROR)

project(SlicerMixedReality)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "https://www.slicer.org/wiki/Documentation/Nightly/Extensions/SlicerMixedReality")
set(EXTENSION_CATEGORY "MixedReality")
set(EXTENSION_CONTRIBUTORS "Sankhesh Jhaveri (Kitware), Lucas Gandel (Kitware ), Jean-Christophe Fillion-Robin (Kitware), Adam Rankin (Robarts Research Institute)")
set(EXTENSION_DESCRIPTION "Allows user to interact with a Slicer scene using a mixed reality headset.")
set(EXTENSION_ICONURL "https://www.example.com/Slicer/Extensions/SlicerMixedReality.png")
set(EXTENSION_SCREENSHOTURLS "https://www.example.com/Slicer/Extensions/SlicerMixedReality/Screenshots/1.png")
set(EXTENSION_DEPENDS "NA") # Specified as a list or "NA" if no dependencies
set(EXTENSION_BUILD_SUBDIRECTORY inner-build)

set(SUPERBUILD_TOPLEVEL_PROJECT inner)

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})
mark_as_superbuild(Slicer_DIR)

find_package(Git REQUIRED)
mark_as_superbuild(GIT_EXECUTABLE)

#-----------------------------------------------------------------------------
# SuperBuild setup
option(${EXTENSION_NAME}_SUPERBUILD "Build ${EXTENSION_NAME} and the projects it depends on." ON)
mark_as_advanced(${EXTENSION_NAME}_SUPERBUILD)
if(${EXTENSION_NAME}_SUPERBUILD)
  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuildPrerequisites.cmake")
  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild.cmake")
  return()
endif()

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(MixedReality)
## NEXT_MODULE

#-----------------------------------------------------------------------------
# Install OpenXRRemoting
#
# RemotingXR.json and companion files are copied or installed along side the
# vtkRenderingRemotingOpenXR library so that "vtkOpenXRManagerRemoteConnection::Initialize()"
# can locate the files and set the XR_RUNTIME_JSON env. variable
set(OpenXRRemoting_FILES )
if(WIN32)
  set(OpenXRRemoting_FILES
    Microsoft.Holographic.AppRemoting.OpenXr.dll
    Microsoft.Holographic.AppRemoting.OpenXr.SU.dll
    PerceptionDevice.dll
    RemotingXR.json
    )
endif()
set(_dest ${CMAKE_BINARY_DIR}/${Slicer_THIRDPARTY_BIN_DIR})
foreach(file IN LISTS OpenXRRemoting_FILES)
  # Copy
  message(STATUS "Copying ${file} to ${_dest}")
  file(COPY ${OpenXRRemoting_BIN_DIR}/${file}
    DESTINATION ${_dest}
    USE_SOURCE_PERMISSIONS
    )
  # Install
  install(FILES ${OpenXRRemoting_BIN_DIR}/${file}
    DESTINATION ${Slicer_INSTALL_THIRDPARTY_LIB_DIR}
    COMPONENT RuntimeLibraries
    )
endforeach()

#-----------------------------------------------------------------------------
set(EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS)

# vtkRenderingVR, vtkRenderingOpenXR
list(APPEND EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS "${vtkRenderingVR_DIR};vtkRenderingVR;runtime;/")
list(APPEND EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS "${vtkRenderingOpenXR_DIR};vtkRenderingOpenXR;runtime;/")
if(Slicer_USE_PYTHONQT)
  list(APPEND EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS "${vtkRenderingVR_DIR};vtkRenderingVR;python;/")
  list(APPEND EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS "${vtkRenderingOpenXR_DIR};vtkRenderingOpenXR;python;/")
endif()

# vtkRenderingOpenXRRemoting
if(WIN32)
  list(APPEND EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS "${vtkRenderingOpenXRRemoting_DIR};vtkRenderingOpenXRRemoting;runtime;/")
  if(Slicer_USE_PYTHONQT)
    list(APPEND EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS "${vtkRenderingOpenXRRemoting_DIR};vtkRenderingOpenXRRemoting;python;/")
  endif()
endif()
set(${EXTENSION_NAME}_CPACK_INSTALL_CMAKE_PROJECTS "${EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS}" CACHE STRING "List of external projects to install" FORCE)

#-----------------------------------------------------------------------------
list(APPEND CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
list(APPEND CPACK_INSTALL_CMAKE_PROJECTS "${${EXTENSION_NAME}_CPACK_INSTALL_CMAKE_PROJECTS}")
include(${Slicer_EXTENSION_GENERATE_CONFIG})
include(${Slicer_EXTENSION_CPACK})
