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

// .NAME vtkSlicerMixedRealityLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerMixedRealityLogic_h
#define __vtkSlicerMixedRealityLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkSlicerMixedRealityModuleLogicExport.h"


/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_MIXEDREALITY_MODULE_LOGIC_EXPORT vtkSlicerMixedRealityLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerMixedRealityLogic *New();
  vtkTypeMacro(vtkSlicerMixedRealityLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkSlicerMixedRealityLogic();
  ~vtkSlicerMixedRealityLogic() override;

  void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  void RegisterNodes() override;
  void UpdateFromMRMLScene() override;
  void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
  void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;
private:

  vtkSlicerMixedRealityLogic(const vtkSlicerMixedRealityLogic&); // Not implemented
  void operator=(const vtkSlicerMixedRealityLogic&); // Not implemented
};

#endif
