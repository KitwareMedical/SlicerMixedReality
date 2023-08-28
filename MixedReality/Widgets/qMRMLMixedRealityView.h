/*==============================================================================

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qMRMLMixedRealityView_h
#define __qMRMLMixedRealityView_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkVTKRenderView.h>

// Qt includes
#include <QWidget>

#include "qSlicerMixedRealityModuleWidgetsExport.h"

// CTK includes
#include <ctkVTKObject.h>

class qMRMLMixedRealityViewPrivate;
class vtkMRMLMixedRealityViewNode;
class vtkCollection;
class vtkGenericOpenGLRenderWindow;
class vtkRenderWindowInteractor;
class vtkSlicerCamerasModuleLogic;

class vtkOpenXRCamera;
class vtkOpenXRRemotingRenderWindow;
class vtkOpenXRRenderer;
class vtkOpenXRRenderWindowInteractor;

/// \brief 3D view for view nodes.
/// For performance reasons, the view block refreshes when the scene is in
/// batch process state.
/// VR hardware connection state is controlled by associated view node's properties:
/// - Visible: connection is made with OpenVR.
/// - Active: scene is rendered in the VR headset.
/// \sa vtkMRMLMixedRealityViewNode
class Q_SLICER_MODULE_MIXEDREALITY_WIDGETS_EXPORT qMRMLMixedRealityView : public QWidget
{
  Q_OBJECT
  QVTK_OBJECT
public:
  /// Superclass typedef
  typedef QWidget Superclass;

  /// Constructors
  explicit qMRMLMixedRealityView(QWidget* parent = nullptr);
  virtual ~qMRMLMixedRealityView();

  /// Add a displayable manager to the view,
  /// the displayable manager is proper to the 3D view and is not shared
  /// with other views.
  /// If you want to register a displayable manager with all the 3D
  /// views (existing or future), you need to do it via
  /// vtkMRMLMixedRealityViewDisplayableManagerFactory::RegisterDisplayableManager()
  /// By default: vtkMRMLCameraDisplayableManager,
  /// vtkMRMLViewDisplayableManager and vtkMRMLModelDisplayableManager are
  /// already registered.
  void addDisplayableManager(const QString& displayableManager);
  Q_INVOKABLE void getDisplayableManagers(vtkCollection *displayableManagers);

  /// Set Cameras module logic.
  /// Required for updating camera from reference view node.
  void setCamerasLogic(vtkSlicerCamerasModuleLogic* camerasLogic);
  vtkSlicerCamerasModuleLogic* camerasLogic()const;

  /// Get the 3D View node observed by view.
  Q_INVOKABLE vtkMRMLMixedRealityViewNode* mrmlMixedRealityViewNode()const;

  /// Get a reference to the associated vtkRenderer
  vtkOpenXRRenderer* renderer()const;

  /// Get underlying RenderWindow
  Q_INVOKABLE vtkOpenXRRemotingRenderWindow* renderWindow()const;

  /// Get underlying RenderWindow
  Q_INVOKABLE vtkOpenXRRenderWindowInteractor* interactor()const;

  /// Initialize the virtual reality view to most closely
  /// matched the camera of the reference view camera.
//  Q_INVOKABLE void updateViewFromReferenceViewCamera();

  /// Get underlying RenderWindow
  Q_INVOKABLE bool isHardwareConnected()const;

  /// Enable/disable grabbing and moving objects in the scene
//  Q_INVOKABLE void setGrabObjectsEnabled(bool enable);
  /// Get whether grabbing and moving objects in the scene is enabled
//  Q_INVOKABLE bool isGrabObjectsEnabled();

  /// Enable/disable dolly (flying in the scene with the TrackPad)
//  Q_INVOKABLE void setDolly3DEnabled(bool enable);
  // Get whether dolly (flying in the scene with the TrackPad) is enabled
//  Q_INVOKABLE bool isDolly3DEnabled();

  ///@{
  /// Path where the action manifest .json files are located.
  void setActionManifestPath(const QString& path);
  QString actionManifestPath() const;
  ///@}

public slots:
  /// Set the current \a viewNode to observe
  void setMRMLMixedRealityViewNode(vtkMRMLMixedRealityViewNode* newViewNode);

protected:

  QScopedPointer<qMRMLMixedRealityViewPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qMRMLMixedRealityView);
  Q_DISABLE_COPY(qMRMLMixedRealityView);
};

#endif
