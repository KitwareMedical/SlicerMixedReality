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

// MRMLDisplayableManager includes
#include "vtkMRMLMixedRealityViewDisplayableManagerFactory.h"

// VTK includes
#include <vtkObjectFactory.h>

//----------------------------------------------------------------------------
// vtkMRMLMixedRealityViewDisplayableManagerFactory methods

//----------------------------------------------------------------------------
// Up the reference count so it behaves like New
vtkMRMLMixedRealityViewDisplayableManagerFactory* vtkMRMLMixedRealityViewDisplayableManagerFactory::New()
{
  vtkMRMLMixedRealityViewDisplayableManagerFactory* instance = Self::GetInstance();
  instance->Register(nullptr);
  return instance;
}

//----------------------------------------------------------------------------
vtkMRMLMixedRealityViewDisplayableManagerFactory* vtkMRMLMixedRealityViewDisplayableManagerFactory::GetInstance()
{
  if(!Self::Instance)
    {
    // Try the factory first
    Self::Instance = (vtkMRMLMixedRealityViewDisplayableManagerFactory*)
                     vtkObjectFactory::CreateInstance("vtkMRMLMixedRealityViewDisplayableManagerFactory");

    // if the factory did not provide one, then create it here
    if(!Self::Instance)
      {
      Self::Instance = new vtkMRMLMixedRealityViewDisplayableManagerFactory;
#ifdef VTK_HAS_INITIALIZE_OBJECT_BASE
      Self::Instance->InitializeObjectBase();
#endif
      }
    }
  // return the instance
  return Self::Instance;
}

//----------------------------------------------------------------------------
vtkMRMLMixedRealityViewDisplayableManagerFactory::
    vtkMRMLMixedRealityViewDisplayableManagerFactory():Superclass()
{
}

//----------------------------------------------------------------------------
vtkMRMLMixedRealityViewDisplayableManagerFactory::~vtkMRMLMixedRealityViewDisplayableManagerFactory()
{
}

//----------------------------------------------------------------------------
void vtkMRMLMixedRealityViewDisplayableManagerFactory::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

VTK_SINGLETON_CXX(vtkMRMLMixedRealityViewDisplayableManagerFactory);
