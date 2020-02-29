/***************************************************************************
 *   Copyright (C) 2005 by Eryk Klebanski                                  *
 *   rixment@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "GcadSceneGraph.h"
#include "GcadMatrixGenerateUtil.h"
#include "GcadMatrixUtil.h"
#include <stack>
#include <algorithm>

using namespace Gcad::Math;
using namespace Gcad::Utilities;
using namespace std;

namespace Gcad {
namespace Framework {

SceneGraph::NodeUpdate
::~NodeUpdate()
{
}

void
SceneGraph::NullNodeUpdate
::update(float elapsedTime)
{
}

SceneGraph::NodeRender
::~NodeRender()
{
}

// Scene Node Implementation

void
SceneGraph::Node
::update(float elapsedTime)
{
  update_->update(elapsedTime);
  
  for(ChildsItor childItor = childs_.begin();
    childItor != childs_.end();
    ++childItor)
  {
    (*childItor)->update(elapsedTime);
  }
}
      
void
SceneGraph::Node
::render()
{
  render_->render();

  for(ChildsItor childItor = childs_.begin();
    childItor != childs_.end();
    ++childItor)
  {
    (*childItor)->render();
  }
}

SceneGraph::Node
::Node(const Id& id)
  : id_(id)
  , update_(new NullNodeUpdate)
{
  parent_ = this;
}

SceneGraph::Node
::Node(Node& parent, const Id& id)
  : parent_(&parent)
  , id_(id)
  , update_(new NullNodeUpdate)
{
}

void 
SceneGraph::Node
::resetOrientation(const Quaternion& q)
{
  orientation_ = q;
}

void 
SceneGraph::Node
::resetTranslation(const Vector3& t)
{
  translation_ = t;
}
      
void 
SceneGraph::Node
::rotateBy(const Quaternion& q)
{
  orientation_ = orientation_ * q;
}

void 
SceneGraph::Node
::translateBy(const Vector3& t)
{
  translation_ += t;
}
      
void 
SceneGraph::Node
::attach(Node& newParent)
{
  Childs::iterator founded = find(
    parent_->childs_.begin(),
    parent_->childs_.end(),
    NodeCntPtr(this));
  parent_->childs_.erase(founded);
  parent_ = &newParent;
  parent_->childs_.push_back(this);
}

void 
SceneGraph::Node
::setRenderAction(NodeRender* act)
{
  render_ = act;
}

void 
SceneGraph::Node
::setUpdateAction(NodeUpdate* act)
{
  update_ = act;
}

void 
SceneGraph::Node
::setMesh(MeshId meshId)
{
  meshId_ = meshId;
}

SceneGraph::Node& 
SceneGraph::Node
::getParent() const
{
  return *parent_;
}

SceneGraph::Node::Id
SceneGraph::Node
::getId() const
{
  return id_;
}

SceneGraph::Node::Id
SceneGraph::Node
::getParentId() const
{
  return parent_->getId();
}

SceneGraph::Node::MeshId
SceneGraph::Node
::getMeshId() const
{
  return meshId_;
}

SceneGraph::Node::Vector3
SceneGraph::Node
::getTranslation() const
{
  return translation_;
}

SceneGraph::Node::Quaternion
SceneGraph::Node
::getOrientation() const
{
  return orientation_;
}

SceneGraph::Node::Matrix4x4 
SceneGraph::Node
::getMatrix() const
{
  Matrix<3, 3, float> m3x3;
  quaternionToMatrix(orientation_, &m3x3);
  Matrix<4, 4, float> m4x4;
  MatrixGenerateUtil::identity(&m4x4);
  for(int r=0; r<3; ++r)
    for(int c=0; c<3; ++c)
      m4x4[r][c] = m3x3[r][c];
  m4x4[3][0] = translation_.x();
  m4x4[3][1] = translation_.y();
  m4x4[3][2] = translation_.z();
  return m4x4;
}

// Viewer Implementation

SceneGraph::Viewer
::Viewer()
  : nodeView_(0)
{
  MatrixGenerateUtil::identity(&relativeView_);
}

void
SceneGraph::Viewer
::setMatrix(const Matrix4x4& newView)
{
  relativeView_ = newView;
}

void
SceneGraph::Viewer
::mulMatrix(const Matrix4x4& m)
{
  Matrix4x4 view(relativeView_);
  MatrixUtil::mul(view, m,
    &relativeView_);
}

SceneGraph::Viewer::Matrix4x4 
SceneGraph::Viewer
::getMatrixCompound() const
{
  stack<const Node*> nodesPatch;
  const Node* viewerActualNode( getNode() );
  nodesPatch.push(viewerActualNode);
  
  // We are creating an patch that runs from root node
  // to node actually assigned to the viewer

  while(nodesPatch.top()->getId() != "ROOT")
  {
    nodesPatch.push(&nodesPatch.top()->getParent());
  }

  // After that we are ready to compute view matrix form
  // concatention of visited matrixes

  Matrix4x4 compoundViewMatrix;
  MatrixGenerateUtil::identity(&compoundViewMatrix);

  while(!nodesPatch.empty())
  {
    Matrix4x4 matrixConcatention(compoundViewMatrix);
    
    MatrixUtil::mul(
      nodesPatch.top()->getMatrix(),
      matrixConcatention,
      &compoundViewMatrix);      

    nodesPatch.pop();
  }  
  
  Matrix4x4 viewMatrix;

  MatrixUtil::mul(
    compoundViewMatrix,
    relativeView_,
    &viewMatrix);

  return viewMatrix;
}

void
SceneGraph::Viewer
::attach(const Node& node)
{
  nodeView_ = &node;
}

const SceneGraph::Node* 
SceneGraph::Viewer
::getNode() const
{
  assertion(nodeView_ != 0, "Bledny wskaznik wezla widoku!");
  return nodeView_;
}

// Scene Graph Implementation

SceneGraph
::SceneGraph()
  : root_(new Node("ROOT"))
{
  anagrams_.add("ROOT", root_.get());
  viewer_.attach(getNode("ROOT"));
}

SceneGraph::Node&
SceneGraph
::createNode(const std::string& nodeId)
{
  NodeCountPtr newNode( new Node(*root_.get(), nodeId) );
  anagrams_.add(nodeId, newNode);
  return *newNode;
}
   
SceneGraph::Node&
SceneGraph
::createNode(const std::string& nodeId,
             const std::string& parentNodeId)
{
  NodeCountPtr newNode( new Node(getNode(parentNodeId), nodeId) );
  anagrams_.add(nodeId, newNode);
  return *newNode;
}

SceneGraph::Node&
SceneGraph
::getNode(const std::string& nodeId)
{
  return *anagrams_.getElement(nodeId);
}
   
void 
SceneGraph
::removeNode(const std::string& nodeId)
{
  Node& toRemove = getNode(nodeId);
  Node::Childs& parentChilds = toRemove.parent_->childs_;
  parentChilds.erase( 
    find(parentChilds.begin(), 
         parentChilds.end(), 
         Node::NodeCntPtr(&toRemove) ) );
  anagrams_.remove(nodeId);
}

void 
SceneGraph
::update(float elapsedTime)
{
  getNode("ROOT").update(elapsedTime);
}

void 
SceneGraph
::render()
{
  // visiblity algorithm right here
  getNode("ROOT").render();
}

bool
operator ==(const SceneGraph::Node& lhs,
            const SceneGraph::Node& rhs)
{
  return lhs.getId() == rhs.getId();    
}

}
}
