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

// MRML includes
#include "vtkMRMLScene.h"
#include "vtkMRMLViewNode.h"
#include "vtkMRMLMixedRealityViewNode.h"

// VTK includes
#include <vtkObjectFactory.h>

// STD includes
#include <sstream>

const char* vtkMRMLMixedRealityViewNode::ReferenceViewNodeReferenceRole = "ReferenceViewNodeRef";

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLMixedRealityViewNode);

//----------------------------------------------------------------------------
vtkMRMLMixedRealityViewNode::vtkMRMLMixedRealityViewNode()
{
  this->Visibility = 0; // hidden by default to not connect to the headset until it is needed
}

//----------------------------------------------------------------------------
vtkMRMLMixedRealityViewNode::~vtkMRMLMixedRealityViewNode() = default;

//----------------------------------------------------------------------------
const char* vtkMRMLMixedRealityViewNode::GetNodeTagName()
{
  return "MixedRealityView";
}

//----------------------------------------------------------------------------
void vtkMRMLMixedRealityViewNode::WriteXML(ostream& of, int nIndent)
{
  this->Superclass::WriteXML(of, nIndent);

  vtkMRMLWriteXMLBeginMacro(of);
  vtkMRMLWriteXMLStdStringMacro(playerIPAddress, PlayerIPAddress);
  vtkMRMLWriteXMLEndMacro();
}

//----------------------------------------------------------------------------
void vtkMRMLMixedRealityViewNode::ReadXMLAttributes(const char** atts)
{
  int disabledModify = this->StartModify();

  this->Superclass::ReadXMLAttributes(atts);

  vtkMRMLReadXMLBeginMacro(atts);
  vtkMRMLReadXMLStdStringMacro(playerIPAddress, PlayerIPAddress);
  vtkMRMLReadXMLEndMacro();

  this->EndModify(disabledModify);
}

//----------------------------------------------------------------------------
// Copy the node's attributes to this object.
// Does NOT copy: ID, FilePrefix, Name, ID
void vtkMRMLMixedRealityViewNode::Copy(vtkMRMLNode* anode)
{
  int disabledModify = this->StartModify();

  this->Superclass::Copy(anode);

  vtkMRMLCopyBeginMacro(anode);
  vtkMRMLCopyStringMacro(PlayerIPAddress);
  vtkMRMLCopyEndMacro();

  this->EndModify(disabledModify);
}

//----------------------------------------------------------------------------
void vtkMRMLMixedRealityViewNode::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  vtkMRMLPrintBeginMacro(os, indent);
  vtkMRMLPrintStdStringMacro(PlayerIPAddress);
  vtkMRMLPrintEndMacro();
}

//----------------------------------------------------------------------------
void vtkMRMLMixedRealityViewNode::SetSceneReferences()
{
  if (!this->Scene)
  {
    vtkErrorMacro( << "SetSceneReferences: Scene is expected to be non NULL.");
    return;
  }

  this->SetAndObserveParentLayoutNode(this);
}

//----------------------------------------------------------------------------
vtkMRMLViewNode* vtkMRMLMixedRealityViewNode::GetReferenceViewNode()
{
  return vtkMRMLViewNode::SafeDownCast(this->GetNodeReference(vtkMRMLMixedRealityViewNode::ReferenceViewNodeReferenceRole));
}

//----------------------------------------------------------------------------
void vtkMRMLMixedRealityViewNode::SetAndObserveReferenceViewNodeID(const char* viewNodeId)
{
  this->SetAndObserveNodeReferenceID(vtkMRMLMixedRealityViewNode::ReferenceViewNodeReferenceRole, viewNodeId);
}

//----------------------------------------------------------------------------
bool vtkMRMLMixedRealityViewNode::SetAndObserveReferenceViewNode(vtkMRMLViewNode* node)
{
  if (node == nullptr)
  {
    this->SetAndObserveReferenceViewNodeID(nullptr);
    return true;
  }
  if (this->Scene != node->GetScene() || node->GetID() == nullptr)
  {
    vtkErrorMacro("SetAndObserveReferenceViewNode: The referenced and referencing node are not in the same scene");
    return false;
  }
  this->SetAndObserveReferenceViewNodeID(node->GetID());
  return true;
}

//----------------------------------------------------------------------------
bool vtkMRMLMixedRealityViewNode::HasError()
{
  return !this->LastErrorMessage.empty();
}

//----------------------------------------------------------------------------
void vtkMRMLMixedRealityViewNode::ClearError()
{
  this->SetError("");
}

//----------------------------------------------------------------------------
void vtkMRMLMixedRealityViewNode::SetError(const std::string& errorText)
{
  if (this->LastErrorMessage == errorText)
  {
    // no change
    return;
  }
  this->LastErrorMessage = errorText;
  this->Modified();
}

//----------------------------------------------------------------------------
std::string vtkMRMLMixedRealityViewNode::GetError() const
{
  return this->LastErrorMessage;
}
