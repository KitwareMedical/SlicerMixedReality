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

#ifndef __vtkMRMLMixedRealityViewNode_h
#define __vtkMRMLMixedRealityViewNode_h

// MRML includes
#include <vtkMRMLViewNode.h>
#include <vtkMRMLLinearTransformNode.h>

// VTK includes
#include <vtkEventData.h>

#include "vtkSlicerMixedRealityModuleMRMLExport.h"

/// \brief MRML node to represent a 3D view.
///
/// View node contains view parameters.
class VTK_SLICER_MIXEDREALITY_MODULE_MRML_EXPORT vtkMRMLMixedRealityViewNode
  : public vtkMRMLViewNode
{
public:
  static vtkMRMLMixedRealityViewNode* New();
  vtkTypeMacro(vtkMRMLMixedRealityViewNode, vtkMRMLViewNode);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //--------------------------------------------------------------------------
  /// MRMLNode methods
  //--------------------------------------------------------------------------

  vtkMRMLNode* CreateNodeInstance() override;

  /// Read node attributes from XML file
  void ReadXMLAttributes(const char** atts) override;

  /// Write this node's information to a MRML file in XML format.
  void WriteXML(ostream& of, int indent) override;

  /// Copy the node's attributes to this object
  void Copy(vtkMRMLNode* node) override;

  /// Get node XML tag name (like Volume, Model)
  const char* GetNodeTagName() override;

  /// Update the references of the node to the scene.
  void SetSceneReferences() override;

  /// Get reference view node.
  /// Reference view node is a regular 3D view node, which content
  /// or view may be synchronized with the virtual reality camera view.
  vtkMRMLViewNode* GetReferenceViewNode();
  /// Set reference view node.
  /// \sa GetReferenceViewNode
  void SetAndObserveReferenceViewNodeID(const char* layoutNodeId);
  /// Set reference view node.
  /// \sa GetReferenceViewNode
  bool SetAndObserveReferenceViewNode(vtkMRMLViewNode* node);

protected:
  vtkMRMLMixedRealityViewNode();
  ~vtkMRMLMixedRealityViewNode() override;
  vtkMRMLMixedRealityViewNode(const vtkMRMLMixedRealityViewNode&);
  void operator=(const vtkMRMLMixedRealityViewNode&);

  static const char* ReferenceViewNodeReferenceRole;
};

#endif
