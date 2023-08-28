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

// MixedReality Logic includes
#include "vtkMRMLMixedRealityViewNode.h"
#include "vtkSlicerMixedRealityLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerMixedRealityLogic);

//----------------------------------------------------------------------------
vtkSlicerMixedRealityLogic::vtkSlicerMixedRealityLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerMixedRealityLogic::~vtkSlicerMixedRealityLogic()
{
  this->SetActiveViewNode(nullptr);
}

//----------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  events->InsertNextValue(vtkMRMLScene::EndImportEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
  // Register MixedReality view node class
  this->GetMRMLScene()->RegisterNodeClass((vtkSmartPointer<vtkMRMLMixedRealityViewNode>::New()));
}

//---------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::UpdateFromMRMLScene()
{
  vtkMRMLMixedRealityViewNode* xrViewNode = nullptr;
  if (this->GetMRMLScene())
  {
    xrViewNode = vtkMRMLMixedRealityViewNode::SafeDownCast(
                   this->GetMRMLScene()->GetSingletonNode("Active", "vtkMRMLMixedRealityViewNode"));
  }
  this->SetActiveViewNode(xrViewNode);
}

//---------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* node)
{
  vtkMRMLMixedRealityViewNode* xrViewNode = vtkMRMLMixedRealityViewNode::SafeDownCast(node);
  if (!xrViewNode)
  {
    return;
  }
  // If a new active VR view node added then use it automatically.
  if (xrViewNode->GetSingletonTag() &&
      strcmp(xrViewNode->GetSingletonTag(), "Active") == 0)
  {
    this->SetActiveViewNode(xrViewNode);
  }
}

//---------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* node)
{
  vtkMRMLMixedRealityViewNode* deletedXrViewNode = vtkMRMLMixedRealityViewNode::SafeDownCast(node);
  if (!deletedXrViewNode)
  {
    return;
  }
  if (deletedXrViewNode == this->ActiveViewNode)
  {
    this->SetActiveViewNode(nullptr);
  }
}

//----------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::OnMRMLSceneEndImport()
{
  if (this->ActiveViewNode != nullptr && this->ActiveViewNode->GetActive())
  {
    // Override the active flag and visibility flags, as XR connection is not restored on scene load
    this->ActiveViewNode->SetActive(0);
    this->ActiveViewNode->SetVisibility(0);
  }

  this->Modified();
}

//----------------------------------------------------------------------------
vtkMRMLMixedRealityViewNode* vtkSlicerMixedRealityLogic::GetMixedRealityViewNode()
{
  return this->ActiveViewNode;
}

//---------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::SetDefaultReferenceView()
{
  if (!this->ActiveViewNode)
  {
    return;
  }
  if (this->ActiveViewNode->GetReferenceViewNode() != nullptr)
  {
    // Reference view is already set, there is nothing to do
    return;
  }
  if (!this->GetMRMLScene())
  {
    return;
  }
  vtkSmartPointer<vtkCollection> nodes = vtkSmartPointer<vtkCollection>::Take(
      this->GetMRMLScene()->GetNodesByClass("vtkMRMLViewNode"));
  vtkMRMLViewNode* viewNode = nullptr;
  vtkCollectionSimpleIterator it;
  for (nodes->InitTraversal(it); (viewNode = vtkMRMLViewNode::SafeDownCast(
                                    nodes->GetNextItemAsObject(it)));)
  {
    if (viewNode->GetVisibility() && viewNode->IsMappedInLayout())
    {
      // Found a view node displayed in current layout, use this
      break;
    }
  }
  // Either use a view node displayed in current layout or just any 3D view node found in the scene
  this->ActiveViewNode->SetAndObserveReferenceViewNode(viewNode);
}

//---------------------------------------------------------------------------
vtkMRMLMixedRealityViewNode* vtkSlicerMixedRealityLogic::GetDefaultMixedRealityViewNode()
{
  vtkMRMLScene* scene = this->GetMRMLScene();
  if (!scene)
  {
    vtkErrorMacro("GetDefaultMixedRealityViewNode failed: invalid scene");
    return nullptr;
  }
  vtkMRMLNode* defaultNode = scene->GetDefaultNodeByClass("vtkMRMLMixedRealityViewNode");
  if (!defaultNode)
  {
    defaultNode = scene->CreateNodeByClass("vtkMRMLMixedRealityViewNode");
    scene->AddDefaultNode(defaultNode);
    defaultNode->Delete(); // scene owns it now
  }
  return vtkMRMLMixedRealityViewNode::SafeDownCast(defaultNode);
}

//---------------------------------------------------------------------------
vtkMRMLMixedRealityViewNode* vtkSlicerMixedRealityLogic::AddMixedRealityViewNode()
{
  vtkMRMLScene* scene = this->GetMRMLScene();
  if (!scene)
  {
    vtkErrorMacro("AddMixedRealityViewNode: Invalid MRML scene");
    return nullptr;
  }

  if (this->ActiveViewNode)
  {
    // There is already a usable VR node, return that
    return this->ActiveViewNode;
  }

  // Create MixedReality view node. Use CreateNodeByClass so that node properties
  // can be overridden with default node properties defined in the scene.
  vtkSmartPointer<vtkMRMLMixedRealityViewNode> xrViewNode = vtkSmartPointer<vtkMRMLMixedRealityViewNode>::Take(
        vtkMRMLMixedRealityViewNode::SafeDownCast(scene->CreateNodeByClass("vtkMRMLMixedRealityViewNode")));
  // We create the node as a singleton to make sure there is only one VR view node in the scene.
  xrViewNode->SetSingletonTag("Active");
  // If a singleton node by that name exists already then it is overwritten
  // and pointer of that node is returned.
  xrViewNode = vtkMRMLMixedRealityViewNode::SafeDownCast(scene->AddNode(xrViewNode));

  return xrViewNode;
}

//----------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::SetActiveViewNode(vtkMRMLMixedRealityViewNode* xrViewNode)
{
  if (this->ActiveViewNode == xrViewNode)
  {
    return;
  }

  if (!this->GetMRMLScene())
  {
    return;
  }

  this->GetMRMLNodesObserverManager()->SetAndObserveObject(vtkObjectPointer(&this->ActiveViewNode), xrViewNode);

  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::ProcessMRMLNodesEvents(vtkObject* caller, unsigned long event, void* vtkNotUsed(callData))
{
  if (caller == this->ActiveViewNode && event == vtkCommand::ModifiedEvent)
  {
    this->Modified();
  }
}

//-----------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::SetMixedRealityConnected(bool connect, const std::string& playerIPAddress)
{
  vtkMRMLScene* scene = this->GetMRMLScene();
  if (!scene)
  {
    vtkErrorMacro("SetMixedRealityConnected: Invalid MRML scene");
    return;
  }

  if (connect)
  {
    if (!this->ActiveViewNode)
    {
      // Check if there is a MixedReality view node in the scene, in case the scene has been loaded
      // from file and VR view properties has been changed
      if (scene->GetNumberOfNodesByClass("vtkMRMLMixedRealityViewNode") > 0)
      {
        // Use the first one if any found
        this->SetActiveViewNode(
          vtkMRMLMixedRealityViewNode::SafeDownCast(scene->GetNthNodeByClass(0, "vtkMRMLMixedRealityViewNode")));
      }
      else
      {
        vtkMRMLMixedRealityViewNode* newViewNode = this->AddMixedRealityViewNode();
        newViewNode->SetPlayerIPAddress(playerIPAddress);
        this->SetActiveViewNode(newViewNode);
      }
    }
    if (this->ActiveViewNode)
    {
      this->ActiveViewNode->SetVisibility(1);
    }
    else
    {
      vtkErrorMacro("Failed to create virtual reality view node");
    }
  }
  else
  {
    if (this->ActiveViewNode)
    {
      this->ActiveViewNode->SetVisibility(0);
    }
  }
}

//-----------------------------------------------------------------------------
bool vtkSlicerMixedRealityLogic::GetMixedRealityConnected()
{
  if (!this->ActiveViewNode)
  {
    return false;
  }
  return (this->ActiveViewNode->GetVisibility() != 0);
}

//-----------------------------------------------------------------------------
void vtkSlicerMixedRealityLogic::SetMixedRealityActive(bool activate)
{
  if (activate)
  {
    if (this->GetMixedRealityConnected()
        && this->GetMixedRealityViewNode()
        && this->GetMixedRealityViewNode()->HasError())
    {
      // If it is connected already but there is an error then disconnect first then reconnect
      // as the error may be resolved by reconnecting.
      this->SetMixedRealityConnected(false, this->ActiveViewNode->GetPlayerIPAddress());
    }
    this->SetMixedRealityConnected(true, this->ActiveViewNode->GetPlayerIPAddress());
    if (this->ActiveViewNode)
    {
      this->ActiveViewNode->SetActive(1);
    }
    else
    {
      vtkErrorMacro("vtkSlicerMixedRealityLogic::SetMixedRealityActive failed: view node is not available");
    }
  }
  else
  {
    if (this->ActiveViewNode != nullptr)
    {
      this->ActiveViewNode->SetActive(0);
    }
  }
}

//-----------------------------------------------------------------------------
bool vtkSlicerMixedRealityLogic::GetMixedRealityActive()
{
  if (!this->ActiveViewNode)
  {
    return false;
  }
  return (this->ActiveViewNode->GetVisibility() != 0 && this->ActiveViewNode->GetActive() != 0);
}
