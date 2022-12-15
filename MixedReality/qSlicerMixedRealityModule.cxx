/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// MixedReality Logic includes
#include <vtkSlicerMixedRealityLogic.h>

// MixedReality includes
#include "qSlicerMixedRealityModule.h"
#include "qSlicerMixedRealityModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerMixedRealityModulePrivate
{
public:
  qSlicerMixedRealityModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerMixedRealityModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerMixedRealityModulePrivate::qSlicerMixedRealityModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerMixedRealityModule methods

//-----------------------------------------------------------------------------
qSlicerMixedRealityModule::qSlicerMixedRealityModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerMixedRealityModulePrivate)
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
  moduleContributors << QString("John Doe (AnyWare Corp.)");
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
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerMixedRealityModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerMixedRealityModule::setup()
{
  this->Superclass::setup();
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
