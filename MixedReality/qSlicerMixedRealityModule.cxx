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

// Qt includes
#include <QDebug>

// Slicer includes
#include <qSlicerCoreApplication.h>

// MixedReality Logic includes
#include <vtkSlicerMixedRealityLogic.h>

// MixedReality includes
#include "qSlicerMixedRealityModule.h"
#include "qSlicerMixedRealityModuleWidget.h"
#include "qMRMLMixedRealityView.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerMixedRealityModulePrivate
{
  Q_DECLARE_PUBLIC(qSlicerMixedRealityModule);
protected:
  qSlicerMixedRealityModule* const q_ptr;
public:
  qSlicerMixedRealityModulePrivate(qSlicerMixedRealityModule& object);
  ~qSlicerMixedRealityModulePrivate();

  /// Adds Mixed Reality view widget
  void addViewWidget();

  vtkSlicerMixedRealityLogic* logic();

  qMRMLMixedRealityView* MixedRealityViewWidget{nullptr};
};

//-----------------------------------------------------------------------------
// qSlicerMixedRealityModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerMixedRealityModulePrivate::qSlicerMixedRealityModulePrivate(qSlicerMixedRealityModule& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
qSlicerMixedRealityModulePrivate::~qSlicerMixedRealityModulePrivate()
{
  delete this->MixedRealityViewWidget;
  this->MixedRealityViewWidget = nullptr;
}

//-----------------------------------------------------------------------------
void qSlicerMixedRealityModulePrivate::addViewWidget()
{
  Q_Q(qSlicerMixedRealityModule);

  if (this->MixedRealityViewWidget != nullptr)
  {
    return;
  }

  this->MixedRealityViewWidget = new qMRMLMixedRealityView();
  this->MixedRealityViewWidget->setObjectName(QString("MixedRealityViewWidget"));

  if(q->isInstalled())
  {
    QString actionManifestPath = qSlicerCoreApplication::application()->extensionsInstallPath()  + "/bin";
    this->MixedRealityViewWidget->setActionManifestPath(actionManifestPath);
  }
  else
  {
    // Since the output of qSlicerAbstractCoreModule::path() is
    //
    //   <module-lib-dir>/qt-loadable-modules[/(Release|Debug|...)]
    //
    // where
    //
    //   <module-lib-dir> is equivalent to <extension-build-dir>/inner-build/lib/Slicer-X.Y
    //
    // and the action manifest files are in this directory
    //
    //   <extension-build-dir>/vtkRenderingOpenVR-build/externals/vtkRenderingOpenXR/
    //
    // we compose the path as such:

    // First, we retrieve <module-lib-dir>
    std::string moduleLibDirectory = vtkSlicerApplicationLogic::GetModuleSlicerXYLibDirectory(q->path().toStdString());

    // ... then we change the directory to vtkRenderingOpenXR-build
    QString actionManifestPath = QString::fromStdString(moduleLibDirectory + "/../../../vtkRenderingOpenXR-build/externals/vtkRenderingOpenXR");

    this->MixedRealityViewWidget->setActionManifestPath(actionManifestPath);
  }

  qDebug() << "actionManifestPath:" << this->MixedRealityViewWidget->actionManifestPath();
}

//-----------------------------------------------------------------------------
vtkSlicerMixedRealityLogic* qSlicerMixedRealityModulePrivate::logic()
{
  Q_Q(qSlicerMixedRealityModule);
  return vtkSlicerMixedRealityLogic::SafeDownCast(q->logic());
}

//-----------------------------------------------------------------------------
// qSlicerMixedRealityModule methods

//-----------------------------------------------------------------------------
qSlicerMixedRealityModule::qSlicerMixedRealityModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerMixedRealityModulePrivate(*this))
{
}

//-----------------------------------------------------------------------------
qSlicerMixedRealityModule::~qSlicerMixedRealityModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerMixedRealityModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerMixedRealityModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerMixedRealityModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)")
                     << QString("Lucas Gandel (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerMixedRealityModule::icon() const
{
  return QIcon(":/Icons/MixedReality.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerMixedRealityModule::categories() const
{
  return QStringList() << "Mixed Reality";
}

//-----------------------------------------------------------------------------
QStringList qSlicerMixedRealityModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
qMRMLMixedRealityView* qSlicerMixedRealityModule::viewWidget()
{
  Q_D(qSlicerMixedRealityModule);
  return d->MixedRealityViewWidget;
}

//-----------------------------------------------------------------------------
void qSlicerMixedRealityModule::setMRMLScene(vtkMRMLScene* scene)
{
  this->Superclass::setMRMLScene(scene);

  vtkSlicerMixedRealityLogic* logic = vtkSlicerMixedRealityLogic::SafeDownCast(this->logic());
  if (!logic)
  {
    qCritical() << Q_FUNC_INFO << " failed: logic is invalid";
    return;
  }

  vtkMRMLMixedRealityViewNode* defaultViewNode = logic->GetDefaultMixedRealityViewNode();
  if (!defaultViewNode)
  {
    qCritical() << Q_FUNC_INFO << " failed: defaultViewNode is invalid";
    return;
  }
//  QSettings settings;
//  settings.beginGroup("Default3DView");
//  if (settings.contains("UseDepthPeeling"))
//  {
//    defaultViewNode->SetUseDepthPeeling(settings.value("UseDepthPeeling").toBool());
//  }
}

// --------------------------------------------------------------------------
void qSlicerMixedRealityModule::onViewNodeModified()
{
  Q_D(qSlicerMixedRealityModule);
  vtkMRMLMixedRealityViewNode* xrViewNode = d->logic()->GetMixedRealityViewNode();

  // Update view node in view widget
  if (d->MixedRealityViewWidget != nullptr)
  {
    vtkMRMLMixedRealityViewNode* oldXrViewNode = d->MixedRealityViewWidget->mrmlMixedRealityViewNode();
    if (oldXrViewNode != xrViewNode)
    {
      d->logic()->SetDefaultReferenceView();
    }
    d->MixedRealityViewWidget->setMRMLMixedRealityViewNode(xrViewNode);
  }

  // Update toolbar
  //d->updateToolBar();
}

//-----------------------------------------------------------------------------
void qSlicerMixedRealityModule::setup()
{
  Q_D(qSlicerMixedRealityModule);
  this->Superclass::setup();

  d->addViewWidget();

  vtkSlicerMixedRealityLogic* xrLogic = vtkSlicerMixedRealityLogic::SafeDownCast(this->logic());

  // If mixed reality logic is modified it indicates that the view node may changed
  qvtkConnect(xrLogic, vtkCommand::ModifiedEvent, this, SLOT(onViewNodeModified()));
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerMixedRealityModule
::createWidgetRepresentation()
{
  return new qSlicerMixedRealityModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerMixedRealityModule::createLogic()
{
  return vtkSlicerMixedRealityLogic::New();
}
