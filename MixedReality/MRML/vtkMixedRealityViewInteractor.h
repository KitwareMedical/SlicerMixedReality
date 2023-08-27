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

#ifndef vtkMixedRealityViewInteractor_h
#define vtkMixedRealityViewInteractor_h

#include "vtkSlicerMixedRealityModuleMRMLExport.h"

#include <vtkOpenXRRenderWindowInteractor.h>

#include <vector>

// vtkRenderingOpenXR is not python wrapped, so wrapping New causes linking error //TODO:
#ifndef __VTK_WRAP__

class VTK_SLICER_MIXEDREALITY_MODULE_MRML_EXPORT vtkMixedRealityViewInteractor : public vtkOpenXRRenderWindowInteractor
{
public:
  static vtkMixedRealityViewInteractor *New();

  typedef vtkMixedRealityViewInteractor Self;

  vtkTypeMacro(vtkMixedRealityViewInteractor,vtkOpenXRRenderWindowInteractor);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:

private:
  vtkMixedRealityViewInteractor();
  ~vtkMixedRealityViewInteractor() override;

  vtkMixedRealityViewInteractor(const vtkMixedRealityViewInteractor&) = delete;
  void operator=(const vtkMixedRealityViewInteractor&) = delete;
};

#endif // __VTK_WRAP__

#endif
