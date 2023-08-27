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

#include "vtkMixedRealityViewInteractorStyle.h"

// VR MRML includes
//#include "vtkMRMLMixedRealityViewNode.h"

// MRML includes
//#include "vtkMRMLAbstractThreeDViewDisplayableManager.h"
//#include "vtkMRMLDisplayableManagerGroup.h"
//#include "vtkMRMLScene.h"

// VTK includes
#include <vtkObjectFactory.h>

// VTK OpenXR
#include <vtkOpenXRRenderWindow.h>
#include <vtkOpenXRRenderWindowInteractor.h>


vtkStandardNewMacro(vtkMixedRealityViewInteractorStyle);

//----------------------------------------------------------------------------
class vtkMixedRealityViewInteractorStyle::vtkInternal
{
public:
  vtkInternal() = default;
  ~vtkInternal() = default;
public:
};

//---------------------------------------------------------------------------
// vtkMixedRealityViewInteractorStyle methods

//----------------------------------------------------------------------------
vtkMixedRealityViewInteractorStyle::vtkMixedRealityViewInteractorStyle()
{
  this->Internal = new vtkInternal();
}

//----------------------------------------------------------------------------
vtkMixedRealityViewInteractorStyle::~vtkMixedRealityViewInteractorStyle()
{
  delete this->Internal;
}

//----------------------------------------------------------------------------
void vtkMixedRealityViewInteractorStyle::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------
// Generic events binding
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Interaction methods
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Interaction entry points
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Multitouch interaction methods
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Utility routines
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------
//vtkMRMLScene* vtkMixedRealityViewInteractorStyle::GetMRMLScene()
//{
//  if (!this->DisplayableManagers
//    || this->DisplayableManagers->GetDisplayableManagerCount() == 0)
//  {
//    return nullptr;
//  }

//  return this->DisplayableManagers->GetNthDisplayableManager(0)->GetMRMLScene();
//}
