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

#ifndef __qSlicerMixedRealityModule_h
#define __qSlicerMixedRealityModule_h

// Slicer includes
#include "qSlicerLoadableModule.h"

#include "qSlicerMixedRealityModuleExport.h"

class qSlicerMixedRealityModulePrivate;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_MIXEDREALITY_EXPORT
qSlicerMixedRealityModule
  : public qSlicerLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.slicer.modules.loadable.qSlicerLoadableModule/1.0");
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerMixedRealityModule(QObject *parent=nullptr);
  ~qSlicerMixedRealityModule() override;

  qSlicerGetTitleMacro(QTMODULE_TITLE);

  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;

  QIcon icon()const override;

  QStringList categories()const override;
  QStringList dependencies() const override;

protected:

  /// Initialize the module. Register the volumes reader/writer
  void setup() override;

  /// Create and return the widget representation associated to this module
  qSlicerAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qSlicerMixedRealityModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerMixedRealityModule);
  Q_DISABLE_COPY(qSlicerMixedRealityModule);

};

#endif
