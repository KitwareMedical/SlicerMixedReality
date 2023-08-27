/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __vtkMixedRealityViewInteractorStyle_h
#define __vtkMixedRealityViewInteractorStyle_h

// MRML includes
//#include "vtkMRMLViewInteractorStyle.h"
#include <vtkOpenXRInteractorStyle.h>
//class vtkMRMLScene;

// VTK includes
#include <vtkObject.h>
#include <vtkEventData.h>

#include "vtkSlicerMixedRealityModuleMRMLExport.h"

/// \brief Mixed reality interactions
class VTK_SLICER_MIXEDREALITY_MODULE_MRML_EXPORT vtkMixedRealityViewInteractorStyle :
//    public vtkMRMLViewInteractorStyle
    public vtkOpenXRInteractorStyle
{
public:
  static vtkMixedRealityViewInteractorStyle *New();
//  vtkTypeMacro(vtkMixedRealityViewInteractorStyle,vtkMRMLViewInteractorStyle);
  vtkTypeMacro(vtkMixedRealityViewInteractorStyle,vtkOpenXRInteractorStyle);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Get MRML scene from the displayable manager group (the first displayable manager's if any)
//  vtkMRMLScene* GetMRMLScene();

protected:
  vtkMixedRealityViewInteractorStyle();
  ~vtkMixedRealityViewInteractorStyle() override;

private:
  vtkMixedRealityViewInteractorStyle(const vtkMixedRealityViewInteractorStyle&);  /// Not implemented.
  void operator=(const vtkMixedRealityViewInteractorStyle&);  /// Not implemented.

  class vtkInternal;
  vtkInternal* Internal;
};

#endif
