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

#ifndef __qSlicerMixedRealityModuleWidget_h
#define __qSlicerMixedRealityModuleWidget_h

// Slicer includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerMixedRealityModuleExport.h"

class qSlicerMixedRealityModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_MIXEDREALITY_EXPORT qSlicerMixedRealityModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerMixedRealityModuleWidget(QWidget *parent=0);
  virtual ~qSlicerMixedRealityModuleWidget();

public slots:


protected:
  QScopedPointer<qSlicerMixedRealityModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qSlicerMixedRealityModuleWidget);
  Q_DISABLE_COPY(qSlicerMixedRealityModuleWidget);
};

#endif
