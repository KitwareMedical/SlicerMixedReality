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

#ifndef __qMRMLMixedRealityView_p_h
#define __qMRMLMixedRealityView_p_h

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Slicer API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// VTK includes
#include <vtkEventData.h>
#include <vtkWeakPointer.h>

// CTK includes
#include <ctkPimpl.h>
#include <ctkVTKObject.h>

// qMRML includes
#include "qMRMLMixedRealityView.h"

// Qt includes
#include <QTimer>

class QLabel;
//class vtkLightCollection;
class vtkMRMLCameraNode;
class vtkMRMLDisplayableManagerGroup;
//class vtkMRMLTransformNode;
class vtkMRMLMixedRealityViewNode;
class vtkObject;;
class vtkOpenXRRemotingRenderWindow;
class vtkOpenXRRenderWindowInteractor;
class vtkTimerLog;
class vtkMixedRealityViewInteractor;
class vtkMixedRealityViewInteractorStyle;

//-----------------------------------------------------------------------------
class qMRMLMixedRealityViewPrivate: public QObject
{
  Q_OBJECT
  QVTK_OBJECT
  Q_DECLARE_PUBLIC(qMRMLMixedRealityView);
protected:
  qMRMLMixedRealityView* const q_ptr;
public:
  qMRMLMixedRealityViewPrivate(qMRMLMixedRealityView& object);
  ~qMRMLMixedRealityViewPrivate();

  virtual void init();

//  void startMixedReality();
//  void stopMixedReality();

//  double desiredUpdateRate();
//  double stillUpdateRate();

public slots:
  void updateWidgetFromMRML();
  void doOpenMixedReality();

protected:
//  void updateTransformNodeWithControllerPose(vtkEventDataDevice device);
//  void updateTransformNodeWithHMDPose();
//  void updateTransformNodesWithTrackerPoses();
//  void updateTransformNodeWithPose(vtkMRMLTransformNode* node, vr::TrackedDevicePose_t* tdPose);
  void createRenderWindow();
  void destroyRenderWindow();

  vtkSlicerCamerasModuleLogic* CamerasLogic;

  vtkSmartPointer<vtkMRMLDisplayableManagerGroup> DisplayableManagerGroup;
  vtkWeakPointer<vtkMRMLMixedRealityViewNode> MRMLMixedRealityViewNode;
  vtkSmartPointer<vtkOpenXRRenderer> Renderer;
  vtkSmartPointer<vtkOpenXRRemotingRenderWindow> RenderWindow;
  vtkSmartPointer<vtkMixedRealityViewInteractor> Interactor;
  //vtkSmartPointer<vtkOpenVRRenderWindowInteractor> Interactor; //TODO: For debugging the original interactor
  vtkSmartPointer<vtkMixedRealityViewInteractorStyle> InteractorStyle;
  //vtkSmartPointer<vtkOpenVRInteractorStyle> InteractorStyle; //TODO: For debugging the original interactor
  vtkSmartPointer<vtkOpenXRCamera> Camera;
//  vtkSmartPointer<vtkLightCollection> Lights;

  vtkSmartPointer<vtkTimerLog> LastViewUpdateTime;
//  double LastViewDirection[3];
//  double LastViewUp[3];
//  double LastViewPosition[3];

  QString ActionManifestPath;

  QTimer MixedRealityLoopTimer;
};

#endif
