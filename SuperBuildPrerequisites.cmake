if(DEFINED slicersources_SOURCE_DIR AND NOT DEFINED Slicer_SOURCE_DIR)
  # Explicitly setting "Slicer_SOURCE_DIR" when only "slicersources_SOURCE_DIR"
  # is defined is required to successfully complete configuration in an empty
  # build directory
  #
  # Indeed, in that case, Slicer sources have been downloaded by they have not been
  # added using "add_subdirectory()" and the variable "Slicer_SOURCE_DIR" is not yet in
  # in the CACHE.
  set(Slicer_SOURCE_DIR ${slicersources_SOURCE_DIR})
endif()

if(APPLE)
  set(SlicerMixedReality_EXTERNAL_PROJECT_DEPENDENCIES "")
  message(STATUS "SlicerMixedReality_EXTERNAL_PROJECT_DEPENDENCIES:${SlicerMixedReality_EXTERNAL_PROJECT_DEPENDENCIES}")
  return()
endif()

# Set list of dependencies to ensure the custom application bundling this
# extension does NOT automatically collect the project list and attempt to
# build external projects associated with VTK modules enabled below.
if(DEFINED Slicer_SOURCE_DIR)
  # Extension is bundled in a custom application
  set(SlicerMixedReality_EXTERNAL_PROJECT_DEPENDENCIES
    OpenXR-SDK
    )
else()
  # Extension is build standalone against Slicer itself built
  # against VTK without the relevant modules enabled.
  set(SlicerMixedReality_EXTERNAL_PROJECT_DEPENDENCIES
    ""
    )
  if(WIN32)
    list(APPEND SlicerMixedReality_EXTERNAL_PROJECT_DEPENDENCIES
      vtkRenderingOpenXRRemoting
      )
  endif()
endif()
message(STATUS "SlicerMixedReality_EXTERNAL_PROJECT_DEPENDENCIES:${SlicerMixedReality_EXTERNAL_PROJECT_DEPENDENCIES}")

if(NOT DEFINED Slicer_SOURCE_DIR)
  # Extension is built standalone

  # VTKExternalModule is required to configure these external projects:
  # - vtkRenderingVR
  # - vtkRenderingOpenXR
  include(${SlicerMixedReality_SOURCE_DIR}/FetchVTKExternalModule.cmake)

else()
  # Extension is bundled in a custom application

  # Additional external project dependencies
  ExternalProject_Add_Dependencies(VTK
    DEPENDS
      OpenXR
    )

  # Additional external project options
  set(VTK_MODULE_ENABLE_VTK_RenderingOpenXRRemoting YES)
  if(NOT WIN32)
    set(VTK_MODULE_ENABLE_VTK_RenderingOpenXRRemoting NO)
  endif()

  mark_as_superbuild(
    VARS
      VTK_MODULE_ENABLE_VTK_RenderingOpenXRRemoting:STRING
    PROJECTS
      VTK
    )

endif()

