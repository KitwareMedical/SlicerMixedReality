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

// Need to be included before qMRMLVRView_p
#include <vtkOpenXRCamera.h>
//#include <vtkMixedRealityViewInteractorStyle.h>
//#include <vtkOpenVRInteractorStyle.h> //TODO: For debugging the original interactor
#include <vtkOpenXRRenderWindowInteractor.h>
//#include <vtkOpenVRRenderWindowInteractor.h> //TODO: For debugging the original interactor
//#include <vtkOpenVRModel.h>
#include <vtkOpenXRRemotingRenderWindow.h>
#include <vtkOpenXRRenderer.h>

#include "qMRMLMixedRealityView_p.h"

// Qt includes
#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QTimer>

// CTK includes
//#include <ctkAxesWidget.h>
#include <ctkPimpl.h>

// qMRML includes
#include "qMRMLColors.h"
#include "qMRMLThreeDView.h"
#include "qMRMLThreeDWidget.h"

// Slicer includes
#include "qSlicerApplication.h"
#include "qSlicerLayoutManager.h"
#include "vtkSlicerConfigure.h" // For Slicer_USE_OpenVR
#include "vtkSlicerCamerasModuleLogic.h"

// MixedReality includes
#include "vtkMRMLMixedRealityViewNode.h"

// MRMLDisplayableManager includes
#include <vtkMRMLAbstractDisplayableManager.h>
#include <vtkMRMLDisplayableManagerGroup.h>
#include <vtkMRMLMixedRealityViewDisplayableManagerFactory.h>

//#include <vtkMRMLThreeDViewInteractorStyle.h>

// MRML includes
#include <vtkMRMLCameraNode.h>
//#include <vtkMRMLLinearTransformNode.h>
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkCollection.h>
//#include <vtkCullerCollection.h>
//#include <vtkLight.h>
//#include <vtkLightCollection.h>
#include <vtkNew.h>
//#include <vtkOpenGLFramebufferObject.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkRenderer.h>
//#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkTimerLog.h>
//#include <vtkTransform.h>

//--------------------------------------------------------------------------
// qMRMLMixedRealityViewPrivate methods

//---------------------------------------------------------------------------
qMRMLMixedRealityViewPrivate::qMRMLMixedRealityViewPrivate(qMRMLMixedRealityView& object)
  : q_ptr(&object)
  , CamerasLogic(nullptr)
{
  this->MRMLMixedRealityViewNode = nullptr;
}

//---------------------------------------------------------------------------
qMRMLMixedRealityViewPrivate::~qMRMLMixedRealityViewPrivate() = default;

//---------------------------------------------------------------------------
void qMRMLMixedRealityViewPrivate::init()
{
//  QObject::connect(&this->MixedRealityLoopTimer, SIGNAL(timeout()), this, SLOT(doOpenMixedReality()));
}

//----------------------------------------------------------------------------
CTK_SET_CPP(qMRMLMixedRealityView, vtkSlicerCamerasModuleLogic*, setCamerasLogic, CamerasLogic);
CTK_GET_CPP(qMRMLMixedRealityView, vtkSlicerCamerasModuleLogic*, camerasLogic, CamerasLogic);

//----------------------------------------------------------------------------
CTK_GET_CPP(qMRMLMixedRealityView, vtkOpenXRRenderer*, renderer, Renderer);

//----------------------------------------------------------------------------
CTK_GET_CPP(qMRMLMixedRealityView, vtkOpenXRRemotingRenderWindow*, renderWindow, RenderWindow);

//----------------------------------------------------------------------------
CTK_GET_CPP(qMRMLMixedRealityView, vtkOpenXRRenderWindowInteractor*, interactor, Interactor);

//---------------------------------------------------------------------------
void qMRMLMixedRealityViewPrivate::createRenderWindow()
{
  Q_Q(qMRMLMixedRealityView);

  this->LastViewUpdateTime = vtkSmartPointer<vtkTimerLog>::New();
  this->LastViewUpdateTime->StartTimer();
  this->LastViewUpdateTime->StopTimer();
//  this->LastViewDirection[0] = 0.0;
//  this->LastViewDirection[1] = 0.0;
//  this->LastViewDirection[2] = 1.0;
//  this->LastViewUp[0] = 0.0;
//  this->LastViewUp[1] = 1.0;
//  this->LastViewUp[2] = 0.0;
//  this->LastViewPosition[0] = 0.0;
//  this->LastViewPosition[1] = 0.0;
//  this->LastViewPosition[2] = 0.0;

//  this->RenderWindow = vtkSmartPointer<vtkOpenVRRenderWindow>::New();
//  this->Renderer = vtkSmartPointer<vtkOpenVRRenderer>::New();
//  this->Interactor = vtkSmartPointer<vtkMixedRealityViewInteractor>::New();
  this->Interactor = vtkSmartPointer<vtkOpenXRRenderWindowInteractor>::New(); //TODO: For debugging the original interactor
//  this->InteractorStyle = vtkSmartPointer<vtkMixedRealityViewInteractorStyle>::New();
  //this->InteractorStyle = vtkSmartPointer<vtkOpenVRInteractorStyle>::New(); //TODO: For debugging the original interactor
//  this->Interactor->SetInteractorStyle(this->InteractorStyle);
//  this->InteractorStyle->SetInteractor(this->Interactor);
//  this->InteractorStyle->SetCurrentRenderer(this->Renderer);
  this->Camera = vtkSmartPointer<vtkOpenXRCamera>::New();
  this->Renderer->SetActiveCamera(this->Camera);

//  this->RenderWindow->SetMultiSamples(0);
//  this->RenderWindow->AddRenderer(this->Renderer);
  this->RenderWindow->SetInteractor(this->Interactor);

  this->RenderWindow->SetRemotingIPAddress(q->mrmlMixedRealityViewNode()->GetPlayerIPAddress().c_str());
  // Set default 10x magnification (conversion: PhysicalScale = 1000 / Magnification)
//  this->RenderWindow->SetPhysicalScale(100.0);
  // Observe VR render window event
//  qvtkReconnect(this->RenderWindow, vtkOpenVRRenderWindow::PhysicalToWorldMatrixModified,
//                q, SLOT(onPhysicalToWorldMatrixModified()));

  // Observe button press event
//  qvtkReconnect(this->Interactor, vtkCommand::Button3DEvent, q, SLOT(onButton3DEvent(vtkObject*,void*,unsigned long,void*)));

  vtkMRMLMixedRealityViewDisplayableManagerFactory* factory
    = vtkMRMLMixedRealityViewDisplayableManagerFactory::GetInstance();

  vtkSlicerApplicationLogic* appLogic = qSlicerApplication::application()->applicationLogic();
  if (!appLogic)
  {
    qCritical() << Q_FUNC_INFO << ": Failed to access application logic";
    return;
  }
  factory->SetMRMLApplicationLogic(appLogic);

  QStringList displayableManagers;
  displayableManagers //<< "vtkMRMLCameraDisplayableManager"
  //<< "vtkMRMLViewDisplayableManager"
      << "vtkMRMLModelDisplayableManager"
      //<< "vtkMRMLThreeDReformatDisplayableManager"
      //<< "vtkMRMLCrosshairDisplayableManager3D"
      //<< "vtkMRMLOrientationMarkerDisplayableManager" // Not supported in VR
      //<< "vtkMRMLRulerDisplayableManager" // Not supported in VR
      //<< "vtkMRMLAnnotationDisplayableManager" // Not supported in VR
      //<< "vtkMRMLMarkupsDisplayableManager"
      //<< "vtkMRMLSegmentationsDisplayableManager3D"
      //<< "vtkMRMLTransformsDisplayableManager3D"
      //<< "vtkMRMLLinearTransformsDisplayableManager3D"
      //<< "vtkMRMLVolumeRenderingDisplayableManager"
      ;
  foreach (const QString& displayableManager, displayableManagers)
  {
    if (!factory->IsDisplayableManagerRegistered(displayableManager.toLatin1()))
    {
      factory->RegisterDisplayableManager(displayableManager.toLatin1());
    }
  }

  this->DisplayableManagerGroup = vtkSmartPointer<vtkMRMLDisplayableManagerGroup>::Take(
                                    factory->InstantiateDisplayableManagers(q->renderer()));
  this->DisplayableManagerGroup->SetMRMLDisplayableNode(this->MRMLMixedRealityViewNode);
//  this->InteractorStyle->SetDisplayableManagers(this->DisplayableManagerGroup);

  qDebug() << Q_FUNC_INFO << ": Number of registered displayable manager:" << this->DisplayableManagerGroup->GetDisplayableManagerCount();

  ///CONFIGURATION OF THE OPENVR ENVIRONEMENT

//  this->Renderer->RemoveCuller(this->Renderer->GetCullers()->GetLastItem());
//  this->Renderer->SetBackground(0.7, 0.7, 0.7);

  // Create 4 lights for even lighting
  // without this, one side of models may be very dark.
//  this->Lights = vtkSmartPointer<vtkLightCollection>::New();
//  {
//    vtkNew<vtkLight> light;
//    light->SetPosition(0.0, 1.0, 0.0);
//    light->SetIntensity(1.0);
//    light->SetLightTypeToSceneLight();
//    this->Lights->AddItem(light);
//  }
//  {
//    vtkNew<vtkLight> light;
//    light->SetPosition(0.8, -0.2, 0.0);
//    light->SetIntensity(0.8);
//    light->SetLightTypeToSceneLight();
//    this->Lights->AddItem(light);
//  }
//  {
//    vtkNew<vtkLight> light;
//    light->SetPosition(-0.3, -0.2, 0.7);
//    light->SetIntensity(0.6);
//    light->SetLightTypeToSceneLight();
//    this->Lights->AddItem(light);
//  }
//  {
//    vtkNew<vtkLight> light;
//    light->SetPosition(-0.3, -0.2, -0.7);
//    light->SetIntensity(0.4);
//    light->SetLightTypeToSceneLight();
//    this->Lights->AddItem(light);
//  }
//  this->Renderer->SetLightCollection(this->Lights);

//  q->updateViewFromReferenceViewCamera();

  this->RenderWindow->Initialize();
//  if (!this->RenderWindow->GetHMD())
//  {
//    qWarning() << Q_FUNC_INFO << ": Failed to initialize OpenVR RenderWindow";
//    return;
//  }
}

//---------------------------------------------------------------------------
void qMRMLMixedRealityViewPrivate::destroyRenderWindow()
{
  Q_Q(qMRMLMixedRealityView);
  this->MixedRealityLoopTimer.stop();
  // Must break the connection between interactor and render window,
  // otherwise they would circularly refer to each other and would not
  // be deleted.
  this->Interactor->SetRenderWindow(nullptr);
  this->Interactor = nullptr;
//  this->InteractorStyle = nullptr;
  this->DisplayableManagerGroup = nullptr;
  this->Renderer = nullptr;
  this->Camera = nullptr;
//  this->Lights = nullptr;
  this->RenderWindow = nullptr;
}

// --------------------------------------------------------------------------
void qMRMLMixedRealityViewPrivate::updateWidgetFromMRML()
{
  Q_Q(qMRMLMixedRealityView);
  if (!this->MRMLMixedRealityViewNode || !this->MRMLMixedRealityViewNode->GetVisibility())
  {
    if (this->RenderWindow != nullptr)
    {
      this->destroyRenderWindow();
    }
//    if (this->MRMLMixedRealityViewNode)
//    {
//      this->MRMLMixedRealityViewNode->ClearError();
//    }
    return;
  }

  if (!this->RenderWindow)
  {
    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
    this->createRenderWindow();
    QApplication::restoreOverrideCursor();
//    if (!q->isHardwareConnected())
//    {
//      this->MRMLMixedRealityViewNode->SetError("Connection failed");
//      return;
//    }
//    this->MRMLMixedRealityViewNode->ClearError();
  }

  if (this->DisplayableManagerGroup->GetMRMLDisplayableNode() != this->MRMLMixedRealityViewNode.GetPointer())
  {
    this->DisplayableManagerGroup->SetMRMLDisplayableNode(this->MRMLMixedRealityViewNode);
  }

//  // Renderer properties
//  this->Renderer->SetGradientBackground(1);
//  this->Renderer->SetBackground(this->MRMLMixedRealityViewNode->GetBackgroundColor());
//  this->Renderer->SetBackground2(this->MRMLMixedRealityViewNode->GetBackgroundColor2());

//  this->Renderer->SetTwoSidedLighting(this->MRMLMixedRealityViewNode->GetTwoSidedLighting());

//  bool switchOnAllLights = this->MRMLMixedRealityViewNode->GetBackLights();
//  for (int i = 2; i < this->Lights->GetNumberOfItems(); i++)
//  {
//    vtkLight* light = vtkLight::SafeDownCast(this->Lights->GetItemAsObject(i));
//    light->SetSwitch(switchOnAllLights);
//  }

//  this->Renderer->SetUseDepthPeeling(this->MRMLMixedRealityViewNode->GetUseDepthPeeling() != 0);
//  this->Renderer->SetUseDepthPeelingForVolumes(this->MRMLMixedRealityViewNode->GetUseDepthPeeling() != 0);

//  // Render window properties
//  if (this->RenderWindow)
//  {
//    // Desired update rate
//    this->RenderWindow->SetDesiredUpdateRate(this->desiredUpdateRate());

//    // Magnification
//    double magnification = this->MRMLMixedRealityViewNode->GetMagnification();
//    if (magnification < 0.01)
//    {
//      magnification = 0.01;
//    }
//    else if (magnification > 100.0)
//    {
//      magnification = 100.0;
//    }
//    this->InteractorStyle->SetMagnification(magnification);

//    // Dolly physical speed
//    double dollyPhysicalSpeedMps = this ->MRMLMixedRealityViewNode->GetMotionSpeed();

//    // 1.6666 m/s is walking speed (= 6 km/h), which is the default. We multiply it with the factor
//    this->InteractorStyle->SetDollyPhysicalSpeed(dollyPhysicalSpeedMps);

//    if (this->RenderWindow->GetHMD())
//    {
//      vtkEventDataDevice deviceIdsToUpdate[] = { vtkEventDataDevice::RightController, vtkEventDataDevice::LeftController, vtkEventDataDevice::Unknown };
//      for (int deviceIdIndex = 0; deviceIdsToUpdate[deviceIdIndex] != vtkEventDataDevice::Unknown; deviceIdIndex++)
//      {
//        vtkOpenVRModel* model = vtkOpenVRModel::SafeDownCast(this->RenderWindow->GetModelForDevice(deviceIdsToUpdate[deviceIdIndex]));
//        if (!model)
//        {
//          continue;
//        }
//        model->SetVisibility(this->MRMLMixedRealityViewNode->GetControllerModelsVisible());
//      }

//      // Update tracking reference visibility
//      for (uint32_t deviceIdIndex = 0; deviceIdIndex < vr::k_unMaxTrackedDeviceCount; ++deviceIdIndex)
//      {
//        if (this->RenderWindow->GetHMD()->GetTrackedDeviceClass(deviceIdIndex) == vr::TrackedDeviceClass_TrackingReference)
//        {
//          vtkOpenVRModel* model = vtkOpenVRModel::SafeDownCast(this->RenderWindow->GetModelForDevice(
//            this->RenderWindow->GetDeviceForOpenVRHandle(deviceIdIndex)));
//          if (!model)
//          {
//            continue;
//          }
//          model->SetVisibility(this->MRMLMixedRealityViewNode->GetLighthouseModelsVisible());
//        }
//      }
//    }
//  }

//  if (this->MRMLMixedRealityViewNode->GetActive())
//  {
//    this->MixedRealityLoopTimer.start(0);
//  }
//  else
//  {
//    this->MixedRealityLoopTimer.stop();
//  }
}

//---------------------------------------------------------------------------
//double qMRMLMixedRealityViewPrivate::desiredUpdateRate()
//{
//  Q_Q(qMRMLMixedRealityView);
//  double rate = this->MRMLMixedRealityViewNode->GetDesiredUpdateRate();

//  // enforce non-zero frame rate to avoid division by zero errors
//  const double defaultStaticViewUpdateRate = 0.0001;
//  if (rate < defaultStaticViewUpdateRate)
//  {
//    rate = defaultStaticViewUpdateRate;
//  }

//  return rate;
//}

//---------------------------------------------------------------------------
//double qMRMLMixedRealityViewPrivate::stillUpdateRate()
//{
//  Q_Q(qMRMLMixedRealityView);
//  return 0.0001;
//}

// --------------------------------------------------------------------------
//void qMRMLMixedRealityViewPrivate::doOpenMixedReality()
//{
//  if (this->Interactor && this->RenderWindow && this->RenderWindow->GetHMD() && this->Renderer)
//  {
//    this->Interactor->DoOneEvent(this->RenderWindow, this->Renderer);

//    this->LastViewUpdateTime->StopTimer();
//    if (this->LastViewUpdateTime->GetElapsedTime() > 0.0)
//    {
//      bool quickViewMotion = true;

//      if (this->MRMLMixedRealityViewNode->GetMotionSensitivity() > 0.999)
//      {
//        quickViewMotion = true;
//      }
//      else if (this->MRMLMixedRealityViewNode->GetMotionSensitivity() <= 0.001)
//      {
//        quickViewMotion = false;
//      }
//      else if (this->LastViewUpdateTime->GetElapsedTime() < 3.0) // don't consider stale measurements
//      {
//        // limit scale:
//        // sensitivity = 0    -> limit = 10.0x
//        // sensitivity = 50%  -> limit =  1.0x
//        // sensitivity = 100% -> limit =  0.1x
//        double limitScale = pow(100, 0.5 - this->MRMLMixedRealityViewNode->GetMotionSensitivity());

//        const double angularSpeedLimitRadiansPerSec = vtkMath::RadiansFromDegrees(5.0 * limitScale);
//        double viewDirectionChangeSpeed = vtkMath::AngleBetweenVectors(this->LastViewDirection,
//                                          this->Camera->GetViewPlaneNormal()) / this->LastViewUpdateTime->GetElapsedTime();
//        double viewUpDirectionChangeSpeed = vtkMath::AngleBetweenVectors(this->LastViewUp,
//                                            this->Camera->GetViewUp()) / this->LastViewUpdateTime->GetElapsedTime();

//        const double translationSpeedLimitMmPerSec = 100.0 * limitScale;
//        // Physical scale = 100 if virtual objects are real-world size; <100 if virtual objects are larger
//        double viewTranslationSpeedMmPerSec = this->RenderWindow->GetPhysicalScale() * 0.01 *
//                                              sqrt(vtkMath::Distance2BetweenPoints(this->LastViewPosition, this->Camera->GetPosition()))
//                                              / this->LastViewUpdateTime->GetElapsedTime();

//        if (viewDirectionChangeSpeed < angularSpeedLimitRadiansPerSec
//            && viewUpDirectionChangeSpeed < angularSpeedLimitRadiansPerSec
//            && viewTranslationSpeedMmPerSec  < translationSpeedLimitMmPerSec)
//        {
//          quickViewMotion = false;
//        }
//      }

//      double updateRate = quickViewMotion ? this->desiredUpdateRate() : this->stillUpdateRate();
//      this->RenderWindow->SetDesiredUpdateRate(updateRate);

//      this->Camera->GetViewPlaneNormal(this->LastViewDirection);
//      this->Camera->GetViewUp(this->LastViewUp);
//      this->Camera->GetPosition(this->LastViewPosition);

//      if (this->MRMLMixedRealityViewNode->GetControllerTransformsUpdate())
//      {
//        this->MRMLMixedRealityViewNode->CreateDefaultControllerTransformNodes();
//        updateTransformNodeWithControllerPose(vtkEventDataDevice::LeftController);
//        updateTransformNodeWithControllerPose(vtkEventDataDevice::RightController);
//      }
//      if (this->MRMLMixedRealityViewNode->GetHMDTransformUpdate())
//      {
//        this->MRMLMixedRealityViewNode->CreateDefaultHMDTransformNode();
//        updateTransformNodeWithHMDPose();
//      }
//      if (this->MRMLMixedRealityViewNode->GetTrackerTransformUpdate())
//      {
//        updateTransformNodesWithTrackerPoses();
//      }

//      this->LastViewUpdateTime->StartTimer();
//    }
//  }
//}

// --------------------------------------------------------------------------
//void qMRMLMixedRealityViewPrivate::updateTransformNodeWithControllerPose(vtkEventDataDevice device)
//{
//  vtkMRMLLinearTransformNode* node = this->MRMLMixedRealityViewNode->GetControllerTransformNode(device);

//  if (node == nullptr)
//  {
//    qCritical() << Q_FUNC_INFO << ": Unable to retrieve linear transform node for device: " << (int)device;
//    return;
//  }

//  int disabledModify = node->StartModify();
//  auto handle = this->RenderWindow->GetDeviceHandleForDevice(device);
//  if (handle == vr::k_unTrackedDeviceIndexInvalid)
//  {
//    node->SetAttribute("MixedReality.ControllerActive", "0");
//    node->SetAttribute("MixedReality.ControllerConnected", "0");
//    node->EndModify(disabledModify);
//    return;
//  }

//  vr::TrackedDevicePose_t* tdPose;
//  this->RenderWindow->GetOpenVRPose(device, &tdPose);

//  if (tdPose == nullptr)
//  {
//    qCritical() << Q_FUNC_INFO << ": Unable to retrieve pose associated with VR tracker with ID: " << (int)device;
//  }
//  if (tdPose == nullptr || tdPose->eTrackingResult != vr::TrackingResult_Running_OK)
//  {
//    node->SetAttribute("MixedReality.ControllerActive", "0");
//  }
//  else
//  {
//    node->SetAttribute("MixedReality.ControllerActive", "1");
//  }

//  if (tdPose == nullptr || !tdPose->bDeviceIsConnected)
//  {
//    node->SetAttribute("MixedReality.ControllerConnected", "0");
//  }
//  else
//  {
//    node->SetAttribute("MixedReality.ControllerConnected", "1");
//  }
//  updateTransformNodeWithPose(node, tdPose);

//  node->EndModify(disabledModify);
//}

//----------------------------------------------------------------------------
//void qMRMLMixedRealityViewPrivate::updateTransformNodeWithHMDPose()
//{
//  vtkMRMLLinearTransformNode* node = this->MRMLMixedRealityViewNode->GetHMDTransformNode();

//  if (node == nullptr)
//  {
//    qCritical() << Q_FUNC_INFO << ": Unable to retrieve linear transform node for HMD";
//    return;
//  }

//  int disabledModify = node->StartModify();
//  if (this->RenderWindow->GetDeviceHandleForDevice(vtkEventDataDevice::HeadMountedDisplay) == vr::k_unTrackedDeviceIndexInvalid)
//  {
//    node->SetAttribute("MixedReality.HMDActive", "0");
//    node->EndModify(disabledModify);
//    return;
//  }

//  vr::TrackedDevicePose_t* tdPose;
//  this->RenderWindow->GetOpenVRPose(vtkEventDataDevice::HeadMountedDisplay, &tdPose);

//  if (tdPose == nullptr)
//  {
//    qCritical() << Q_FUNC_INFO << ": Unable to retrieve HMD pose";
//  }
//  if (tdPose == nullptr || tdPose->eTrackingResult != vr::TrackingResult_Running_OK)
//  {
//    node->SetAttribute("MixedReality.HMDActive", "0");
//  }
//  else
//  {
//    node->SetAttribute("MixedReality.HMDActive", "1");
//  }

//  updateTransformNodeWithPose(node, tdPose);

//  node->EndModify(disabledModify);
//}

//----------------------------------------------------------------------------
//void qMRMLMixedRealityViewPrivate::updateTransformNodesWithTrackerPoses()
//{
//  Q_Q(qMRMLMixedRealityView);
//  for (uint32_t i = 0; i < this->RenderWindow->GetNumberOfDeviceHandlesForDevice(vtkEventDataDevice::GenericTracker); ++i)
//  {
//    vr::TrackedDeviceIndex_t dev = this->RenderWindow->GetDeviceHandleForDevice(vtkEventDataDevice::GenericTracker, i);
//    std::stringstream ss;
//    ss << dev;

//    vtkMRMLTransformNode* node = vtkMRMLTransformNode::SafeDownCast(this->MRMLMixedRealityViewNode->GetTrackerTransformNode(ss.str().c_str()));
//    if (node == nullptr)
//    {
//      // Node wasn't found for this device, let's create one
//      node = vtkMRMLLinearTransformNode::SafeDownCast(this->MRMLMixedRealityViewNode->GetScene()->AddNode(vtkMRMLLinearTransformNode::New()));
//      if (node == nullptr)
//      {
//        qCritical() << Q_FUNC_INFO << ": Unable to add transform node to scene. Can't update VR tracker with ID: " << dev;
//        continue;
//      }
//      node->SetAttribute("MixedReality.VRDeviceID", ss.str().c_str());
//      node->SetName("MixedReality.GenericTracker");
//      this->MRMLMixedRealityViewNode->SetAndObserveTrackerTransformNode(node, ss.str().c_str());
//    }

//    int disabledModify = node->StartModify();

//    // Now, we have our generic tracker node, let's update it!
//    vr::TrackedDevicePose_t* tdPose;
//    this->RenderWindow->GetOpenVRPose(vtkEventDataDevice::GenericTracker, dev, &tdPose);

//    if (tdPose == nullptr)
//    {
//      qCritical() << Q_FUNC_INFO << ": Unable to retrieve pose associated with VR tracker with ID: " << dev;
//    }
//    if (tdPose == nullptr || tdPose->eTrackingResult != vr::TrackingResult_Running_OK)
//    {
//      node->SetAttribute("MixedReality.TrackerActive", "0");
//    }
//    else
//    {
//      node->SetAttribute("MixedReality.TrackerActive", "1");
//    }
//    updateTransformNodeWithPose(node, tdPose);

//    node->EndModify(disabledModify);
//  }
//}


//----------------------------------------------------------------------------
//void qMRMLMixedRealityViewPrivate::updateTransformNodeWithPose(vtkMRMLTransformNode* node, vr::TrackedDevicePose_t* tdPose)
//{
//  if (tdPose == nullptr)
//  {
//    node->SetAttribute("MixedReality.PoseValid", "False");
//    node->SetAttribute("MixedReality.PoseStatus", "Uninitialized");
//    return;
//  }
//  double pos[3] = { 0. };
//  double ppos[3] = { 0. };
//  double wxyz[4] = { 1., 0., 0., 0. };
//  double wdir[3] = { 1., 0., 0. };

//  vtkNew<vtkMatrix4x4> pose;
//  this->RenderWindow->SetMatrixFromOpenVRPose(pose, *tdPose);
//  this->Interactor->ConvertPoseToWorldCoordinates(pose, pos, wxyz, ppos, wdir);
//  vtkNew<vtkTransform> transform;
//  transform->Translate(pos);
//  transform->RotateWXYZ(wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
//  if (node != nullptr)
//  {
//    node->SetMatrixTransformToParent(transform->GetMatrix());
//  }
//  node->SetAttribute("MixedReality.PoseValid", tdPose->bPoseIsValid ? "True" : "False");
//  node->SetAttribute("MixedReality.PoseStatus", PoseStatusToString(tdPose->eTrackingResult).c_str());
//}


// --------------------------------------------------------------------------
// qMRMLMixedRealityView methods

// --------------------------------------------------------------------------
qMRMLMixedRealityView::qMRMLMixedRealityView(QWidget* _parent) : Superclass(_parent)
  , d_ptr(new qMRMLMixedRealityViewPrivate(*this))
{
  Q_D(qMRMLMixedRealityView);
  d->init();
}

// --------------------------------------------------------------------------
qMRMLMixedRealityView::~qMRMLMixedRealityView()
{
}

//------------------------------------------------------------------------------
void qMRMLMixedRealityView::addDisplayableManager(const QString& displayableManagerName)
{
  Q_D(qMRMLMixedRealityView);
  vtkSmartPointer<vtkMRMLAbstractDisplayableManager> displayableManager;
  displayableManager.TakeReference(
    vtkMRMLDisplayableManagerGroup::InstantiateDisplayableManager(
      displayableManagerName.toLatin1()));
  d->DisplayableManagerGroup->AddDisplayableManager(displayableManager);
}

//------------------------------------------------------------------------------
void qMRMLMixedRealityView::setMRMLMixedRealityViewNode(vtkMRMLMixedRealityViewNode* newViewNode)
{
  Q_D(qMRMLMixedRealityView);
  if (d->MRMLMixedRealityViewNode == newViewNode)
  {
    return;
  }

  d->qvtkReconnect(
    d->MRMLMixedRealityViewNode, newViewNode,
    vtkCommand::ModifiedEvent, d, SLOT(updateWidgetFromMRML()));

  d->MRMLMixedRealityViewNode = newViewNode;

  d->updateWidgetFromMRML();

  // Enable/disable widget
  this->setEnabled(newViewNode != nullptr);
}

//---------------------------------------------------------------------------
vtkMRMLMixedRealityViewNode* qMRMLMixedRealityView::mrmlMixedRealityViewNode()const
{
  Q_D(const qMRMLMixedRealityView);
  return d->MRMLMixedRealityViewNode;
}

//------------------------------------------------------------------------------
void qMRMLMixedRealityView::getDisplayableManagers(vtkCollection* displayableManagers)
{
  Q_D(qMRMLMixedRealityView);

  if (!displayableManagers || !d->DisplayableManagerGroup)
  {
    return;
  }
  int num = d->DisplayableManagerGroup->GetDisplayableManagerCount();
  for (int n = 0; n < num; n++)
  {
    displayableManagers->AddItem(d->DisplayableManagerGroup->GetNthDisplayableManager(n));
  }
}

//------------------------------------------------------------------------------
//bool qMRMLMixedRealityView::isHardwareConnected()const
//{
//  vtkOpenVRRenderWindow* renWin = this->renderWindow();
//  if (!renWin)
//  {
//    return false;
//  }
//  if (!renWin->GetHMD())
//  {
//    return false;
//  }
//  // connected successfully
//  return true;
//}

//------------------------------------------------------------------------------
//void qMRMLMixedRealityView::setGrabObjectsEnabled(bool enable)
//{
//  Q_D(qMRMLMixedRealityView);
//  if (enable)
//  {
//    d->InteractorStyle->GrabEnabledOn();
//  }
//  else
//  {
//    d->InteractorStyle->GrabEnabledOff();
//  }
//}

//------------------------------------------------------------------------------
//bool qMRMLMixedRealityView::isGrabObjectsEnabled()
//{
//  Q_D(qMRMLMixedRealityView);
//  return d->InteractorStyle->GetGrabEnabled() != 0;
//}

//------------------------------------------------------------------------------
//void qMRMLMixedRealityView::setDolly3DEnabled(bool enable)
//{
//  Q_D(qMRMLMixedRealityView);
//  if (enable)
//  {
//    d->InteractorStyle->MapInputToAction(vtkEventDataDevice::RightController, vtkEventDataDeviceInput::TrackPad, VTKIS_DOLLY);
//  }
//  else
//  {
//    d->InteractorStyle->MapInputToAction(vtkEventDataDevice::RightController, vtkEventDataDeviceInput::TrackPad, VTKIS_NONE);
//  }
//}

//------------------------------------------------------------------------------
//bool qMRMLMixedRealityView::isDolly3DEnabled()
//{
//  Q_D(qMRMLMixedRealityView);

//  return d->InteractorStyle->GetMappedAction(vtkEventDataDevice::RightController, vtkEventDataDeviceInput::TrackPad) == VTKIS_DOLLY;
//}

//------------------------------------------------------------------------------
//void qMRMLMixedRealityView::setGestureButtonToTrigger()
//{
//  Q_D(qMRMLMixedRealityView);
//  d->Interactor->SetGestureButtonToTrigger();
//}

//------------------------------------------------------------------------------
//void qMRMLMixedRealityView::setGestureButtonToGrip()
//{
//  Q_D(qMRMLMixedRealityView);
//  d->Interactor->SetGestureButtonToGrip();
//}

//------------------------------------------------------------------------------
//void qMRMLMixedRealityView::setGestureButtonToTriggerAndGrip()
//{
//  Q_D(qMRMLMixedRealityView);
//  d->Interactor->SetGestureButtonToTriggerAndGrip();
//}

//------------------------------------------------------------------------------
//void qMRMLMixedRealityView::setGestureButtonToNone()
//{
//  Q_D(qMRMLMixedRealityView);
//  d->Interactor->SetGestureButtonToNone();
//}

//------------------------------------------------------------------------------
//void qMRMLMixedRealityView::onPhysicalToWorldMatrixModified()
//{
//  Q_D(qMRMLMixedRealityView);

//  d->MRMLMixedRealityViewNode->SetMagnification(d->InteractorStyle->GetMagnification());

//  emit physicalToWorldMatrixModified();
//}

//------------------------------------------------------------------------------
//void qMRMLMixedRealityView::onButton3DEvent(vtkObject* caller, void* call_data, unsigned long vtk_event, void* client_data)
//{
//  Q_UNUSED(caller);
//  Q_UNUSED(vtk_event);
//  Q_UNUSED(client_data);
//  Q_D(qMRMLMixedRealityView);

//  vtkEventDataDevice3D * ed = reinterpret_cast<vtkEventDataDevice3D*>(call_data);

//  if(ed->GetInput() == vtkEventDataDeviceInput::Trigger)
//  {
//    if(ed->GetDevice() == vtkEventDataDevice::LeftController)
//    {
//      if (ed->GetAction() == vtkEventDataAction::Press)
//      {
//        emit leftControllerTriggerPressed();
//      }
//      else if (ed->GetAction() == vtkEventDataAction::Release)
//      {
//        emit leftControllerTriggerReleased();
//      }
//    }
//    else if (ed->GetDevice() == vtkEventDataDevice::RightController)
//    {
//      if (ed->GetAction() == vtkEventDataAction::Press)
//      {
//        emit rightControllerTriggerPressed();
//      }
//      else if (ed->GetAction() == vtkEventDataAction::Release)
//      {
//        emit rightControllerTriggerReleased();
//      }
//    }
//  }
//  else if (ed->GetInput() == vtkEventDataDeviceInput::Grip)
//  {
//    if (ed->GetDevice() == vtkEventDataDevice::LeftController)
//    {
//      if (ed->GetAction() == vtkEventDataAction::Press)
//      {
//        emit leftControllerGripPressed();
//      }
//      else if (ed->GetAction() == vtkEventDataAction::Release)
//      {
//        emit leftControllerGripReleased();
//      }
//    }
//    else if (ed->GetDevice() == vtkEventDataDevice::RightController)
//    {
//      if (ed->GetAction() == vtkEventDataAction::Press)
//      {
//        emit rightControllerGripPressed();
//      }
//      else if (ed->GetAction() == vtkEventDataAction::Release)
//      {
//        emit rightControllerGripReleased();
//      }
//    }
//  }
//  else if (ed->GetInput() == vtkEventDataDeviceInput::TrackPad)
//  {
//    if (ed->GetDevice() == vtkEventDataDevice::LeftController)
//    {
//      if (ed->GetAction() == vtkEventDataAction::Press)
//      {
//        emit leftControllerTrackpadPressed(ed->GetTrackPadPosition()[0],ed->GetTrackPadPosition()[1]);
//      }
//      else if (ed->GetAction() == vtkEventDataAction::Release)
//      {
//        emit leftControllerTrackpadReleased(ed->GetTrackPadPosition()[0], ed->GetTrackPadPosition()[1]);
//      }
//    }
//    else if (ed->GetDevice() == vtkEventDataDevice::RightController)
//    {
//      if (ed->GetAction() == vtkEventDataAction::Press)
//      {
//        emit rightControllerTrackpadPressed(ed->GetTrackPadPosition()[0], ed->GetTrackPadPosition()[1]);
//      }
//      else if (ed->GetAction() == vtkEventDataAction::Release)
//      {
//        emit rightControllerTrackpadReleased(ed->GetTrackPadPosition()[0], ed->GetTrackPadPosition()[1]);
//      }
//    }
//  }
//}

//---------------------------------------------------------------------------
//void qMRMLMixedRealityView::updateViewFromReferenceViewCamera()
//{
//  Q_D(qMRMLMixedRealityView);
//  if (!d->MRMLMixedRealityViewNode)
//  {
//    return;
//  }
//  vtkMRMLViewNode* refrenceViewNode = d->MRMLMixedRealityViewNode->GetReferenceViewNode();
//  if (!refrenceViewNode)
//  {
//    qWarning() << Q_FUNC_INFO << " failed: no reference view node is set";
//    return;
//  }
//  if (!d->CamerasLogic)
//  {
//    qWarning() << Q_FUNC_INFO << " failed: cameras module logic is not set";
//    return;
//  }
//  vtkMRMLCameraNode* cameraNode = d->CamerasLogic->GetViewActiveCameraNode(refrenceViewNode);
//  if (!cameraNode || !cameraNode->GetCamera())
//  {
//    qWarning() << Q_FUNC_INFO << " failed: camera node is not found";
//    return;
//  }
//  if (!d->RenderWindow)
//  {
//    qWarning() << Q_FUNC_INFO << " failed: RenderWindow has not been created";
//    return;
//  }

//  // The following is based on d->RenderWindow->InitializeViewFromCamera(sourceCamera),
//  // but that is not usable for us, as it puts the headset in the focal point (so we
//  // need to step back to see the full content) and snaps view direction to the closest axis.

//  vtkCamera* sourceCamera = cameraNode->GetCamera();

//  vtkRenderer* ren = static_cast<vtkRenderer*>(d->RenderWindow->GetRenderers()->GetItemAsObject(0));
//  if (!ren)
//  {
//    qWarning() << Q_FUNC_INFO << ": The renderer must be set prior to calling InitializeViewFromCamera";
//    return;
//  }
//  vtkOpenVRCamera* cam = vtkOpenVRCamera::SafeDownCast(ren->GetActiveCamera());
//  if (!cam)
//  {
//    qWarning() << Q_FUNC_INFO << ": The renderer's active camera must be set prior to calling InitializeViewFromCamera";
//    return;
//  }

//  double newPhysicalScale = 100.0; // Default 10x magnification

//  double* sourceViewUp = sourceCamera->GetViewUp();
//  cam->SetViewUp(sourceViewUp);
//  d->RenderWindow->SetPhysicalViewUp(sourceViewUp);

//  double* sourcePosition = sourceCamera->GetPosition();
//  double* viewUp = cam->GetViewUp();
//  cam->SetFocalPoint(sourcePosition);
//  d->RenderWindow->SetPhysicalTranslation(
//    viewUp[0] * newPhysicalScale - sourcePosition[0],
//    viewUp[1] * newPhysicalScale - sourcePosition[1],
//    viewUp[2] * newPhysicalScale - sourcePosition[2]);

//  double* sourceDirectionOfProjection = sourceCamera->GetDirectionOfProjection();
//  d->RenderWindow->SetPhysicalViewDirection(sourceDirectionOfProjection);
//  double* idop = d->RenderWindow->GetPhysicalViewDirection();
//  cam->SetPosition(
//    -idop[0] * newPhysicalScale + sourcePosition[0],
//    -idop[1] * newPhysicalScale + sourcePosition[1],
//    -idop[2] * newPhysicalScale + sourcePosition[2]);

//  d->RenderWindow->SetPhysicalScale(newPhysicalScale);

//  ren->ResetCameraClippingRange();
//}
