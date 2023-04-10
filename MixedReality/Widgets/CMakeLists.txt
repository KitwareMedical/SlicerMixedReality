project(qSlicer${MODULE_NAME}ModuleWidgets)

set(KIT ${PROJECT_NAME})

#-----------------------------------------------------------------------------
find_package(vtkRenderingOpenXRRemoting REQUIRED)

#-----------------------------------------------------------------------------
set(${KIT}_EXPORT_DIRECTIVE "Q_SLICER_MODULE_${MODULE_NAME_UPPER}_WIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  )

set(${KIT}_SRCS
  qMRML${MODULE_NAME}View.cxx
  qMRML${MODULE_NAME}View_p.h
  qMRML${MODULE_NAME}View.h
  )

set(${KIT}_MOC_SRCS
  qMRML${MODULE_NAME}View.h
  qMRML${MODULE_NAME}View_p.h
  )

set(${KIT}_UI_SRCS
  )

set(${KIT}_RESOURCES
  ../Resources/qSlicer${MODULE_NAME}Module.qrc
  )

set(${KIT}_TARGET_LIBRARIES
  vtkSlicer${MODULE_NAME}ModuleLogic
  vtkSlicerCamerasModuleLogic
  ${VTK_LIBRARIES}
  VTK::RenderingOpenXRRemoting
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleWidgets(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  UI_SRCS ${${KIT}_UI_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  RESOURCES ${${KIT}_RESOURCES}
  WRAP_PYTHONQT
  )
