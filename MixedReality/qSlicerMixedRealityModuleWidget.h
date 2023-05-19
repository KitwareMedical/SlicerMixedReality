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

protected slots:
  void updateWidgetFromMRML();

protected:
  QScopedPointer<qSlicerMixedRealityModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qSlicerMixedRealityModuleWidget);
  Q_DISABLE_COPY(qSlicerMixedRealityModuleWidget);
};

#endif
