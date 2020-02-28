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

#ifndef _GCAD_SCENEGRAPH_H_
#define _GCAD_SCENEGRAPH_H_

#include "GcadBase.h"
#include "GcadException.h"
#include "GcadMatrix.h"
#include "GcadQuaternion.h"
#include "GcadVector3.h"
#include "GcadRefCountPtr.h"
#include "GcadAnagramSet.h"
#include <string>
#include <vector>

namespace Gcad {
namespace Framework {

//! @b Virtual world representation
class GCAD_EXPORT SceneGraph {
 public:
   class GCAD_EXPORT Node;
   
   typedef Gcad::Utilities::RefCountPtr<Node>         NodeCountPtr;
   typedef Gcad::Utilities::AnagramSet<NodeCountPtr>  NodesAnagrams;

 public:
   class GCAD_EXPORT AmbiguousIdentifier
     : public Gcad::Utilities::Exception
   {
     AmbiguousIdentifier(const std::string& e)
       : Gcad::Utilities::Exception(e)
     {}
   };

   //! @b Action for every node of scene performed per frame
   class GCAD_EXPORT NodeUpdate {
    public:
      virtual ~NodeUpdate();
      virtual void update(float elapsedTime) = 0;
   };

   //! @b Null action for node update
   class GCAD_EXPORT NullNodeUpdate 
     : public NodeUpdate
   {
    public:
      virtual void update(float elapsedTime);
   };

   //! @b Rendering function
   class GCAD_EXPORT NodeRender {
    public:
      virtual ~NodeRender();
      virtual void render() = 0;
   };

   //! @b Fundamental scene element
   class GCAD_EXPORT Node {  
    public:
      typedef Gcad::Math::Quaternion<float>    Quaternion;
      typedef Gcad::Math::Matrix<4, 4, float>  Matrix4x4;
      typedef Gcad::Math::Vector3<float>       Vector3;
      
      typedef Gcad::Utilities::RefCountPtr<Node>  NodeCntPtr;
      typedef std::vector<NodeCntPtr>             Childs;
      typedef Childs::iterator                    ChildsItor;

      typedef std::string  Id;
      typedef std::string  MeshId;

      typedef Gcad::Utilities::RefCountPtr<NodeUpdate> NodeUpdateCntPtr;
      typedef Gcad::Utilities::RefCountPtr<NodeRender> NodeRenderCntPtr;

    public:
      void update(float elapsedTime);
      void render();
    
    private:
      friend class SceneGraph;

      Node(const Id& id); // for root node
      Node(Node& parent, const Id& id);

    public:
      void resetOrientation(const Quaternion& q);
      void resetTranslation(const Vector3& t);
      
      void rotateBy(const Quaternion& q);
      void translateBy(const Vector3& t);
      
      void attach(Node& newParent);

      void setRenderAction(NodeRender* act);
      void setUpdateAction(NodeUpdate* act);

      void setMesh(MeshId meshId);
    
    public:
      Node&      getParent() const;

      Id         getId() const;
      Id         getParentId() const;
      MeshId     getMeshId() const;

      Vector3    getTranslation() const;
      Quaternion getOrientation() const;
      Matrix4x4  getMatrix() const;

    private:
      Id      id_;
      MeshId  meshId_;
      
      Node*   parent_;
      Childs  childs_;
      
      Vector3     translation_;
      Quaternion  orientation_;
      
      NodeUpdateCntPtr  update_;
      NodeRenderCntPtr  render_;
   };

   //! @b Class representing a viewer in world scene space
   class GCAD_EXPORT Viewer {
    public:
      typedef Gcad::Math::Matrix<4, 4, float>  Matrix4x4;

      friend class SceneGraph;

    public:
      //! @b Set an relative matrix for a viewer
      void setMatrix(const Matrix4x4& newView);

      //! @b Mutiply actual view matrix by matrix m
      void mulMatrix(const Matrix4x4& m);

      //! @b Get compouded cumulative view matrix
      Matrix4x4 getMatrixCompound() const;

      //! @b Assign viewer to specified node
      void attach(const Node& node);    
     
    private:
      //! @b Default constructor for newly created viewer
      Viewer();
 
    private:
      //! @b Hided functionality used inside implementation
      const Node* getNode() const;

    private:
      Matrix4x4    relativeView_; /**< @b Relative matrix view */
      const Node*  nodeView_;     /**< @b Viewer is attached to specyfic node */
   };

 public:
   //! @b Root node is created
   SceneGraph();

   //! @b Created node is primary attached to root of the scene
   Node& createNode(const std::string& nodeId);
   
   //! @b Create node as a child of parentNodeId node
   Node& createNode(const std::string& nodeId,
                    const std::string& parentNodeId);

   //! @b Getting node by specified ident
   Node& getNode(const std::string& nodeId);
   
   //! @b Erasing node from scene
   void removeNode(const std::string& nodeId);

   //! @b Update all entitiy nodes based on elapsed time value
   void update(float elapsedTime);

   //! @b Scene rendering function
   void render();

   //! @b Dereference an viewer
   Viewer& getViewer();
   
 private:
   NodeCountPtr   root_;
   Viewer         viewer_;
   NodesAnagrams  anagrams_;   
};

bool
operator ==(const SceneGraph::Node& lhs,
            const SceneGraph::Node& rhs);

inline
bool
operator !=(const SceneGraph::Node& lhs,
            const SceneGraph::Node& rhs)
{
  return !(lhs==rhs);
}

}
}

#endif
