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

#ifndef __vtkMRMLMixedRealityViewDisplayableManagerFactory_h
#define __vtkMRMLMixedRealityViewDisplayableManagerFactory_h

// MRMLDisplayableManager includes
#include "vtkMRMLDisplayableManagerFactory.h"

// VTK includes
#include <vtkSingleton.h>

#include "vtkSlicerMixedRealityModuleMRMLExport.h"

class vtkRenderer;

/// \brief Factory where displayable manager classes are registered.
///
/// A displayable manager class is responsible to represente a
/// MRMLDisplayable node in a renderer.
class VTK_SLICER_MIXEDREALITY_MODULE_MRML_EXPORT vtkMRMLMixedRealityViewDisplayableManagerFactory
  : public vtkMRMLDisplayableManagerFactory
{
public:

  vtkTypeMacro(vtkMRMLMixedRealityViewDisplayableManagerFactory,
                       vtkMRMLDisplayableManagerFactory);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// This is a singleton pattern New.  There will only be ONE
  /// reference to a vtkMRMLMixedRealityViewDisplayableManagerFactory object per process. Clients that
  /// call this must call Delete on the object so that the reference counting will work.
  /// The single instance will be unreferenced when the program exits.
  static vtkMRMLMixedRealityViewDisplayableManagerFactory *New();

  /// Return the singleton instance with no reference counting.
  static vtkMRMLMixedRealityViewDisplayableManagerFactory* GetInstance();

protected:

  vtkMRMLMixedRealityViewDisplayableManagerFactory();
  virtual ~vtkMRMLMixedRealityViewDisplayableManagerFactory();

  VTK_SINGLETON_DECLARE(vtkMRMLMixedRealityViewDisplayableManagerFactory);

private:

  vtkMRMLMixedRealityViewDisplayableManagerFactory(const vtkMRMLMixedRealityViewDisplayableManagerFactory&);
  void operator=(const vtkMRMLMixedRealityViewDisplayableManagerFactory&);

};

#ifndef __VTK_WRAP__
//BTX
VTK_SINGLETON_DECLARE_INITIALIZER(VTK_SLICER_MIXEDREALITY_MODULE_MRML_EXPORT,
                                  vtkMRMLMixedRealityViewDisplayableManagerFactory);
//ETX
#endif // __VTK_WRAP__

#endif
