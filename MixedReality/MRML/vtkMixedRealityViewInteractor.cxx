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

#include "vtkMixedRealityViewInteractor.h"

// VTK includes
#include <vtkObjectFactory.h>

// SlicerMixedReality includes
#include "vtkMixedRealityViewInteractorStyle.h"

vtkStandardNewMacro(vtkMixedRealityViewInteractor);

vtkMixedRealityViewInteractor::vtkMixedRealityViewInteractor() = default;
vtkMixedRealityViewInteractor::~vtkMixedRealityViewInteractor() = default;

//----------------------------------------------------------------------------
void vtkMixedRealityViewInteractor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
