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

#include "GcadDataFileResourceManagerBase.h"

namespace Gcad {
namespace Platform {

DataFileResourceManagerBase::ResourceManage
::~ResourceManage()
{
}

void 
DataFileResourceManagerBase::NullResourceManage
::acquire(const std::string& directory,
          const std::string& fileName)
{
}

void 
DataFileResourceManagerBase::NullResourceManage
::load(const std::string& directory,
       const std::string& fileName)
{
}
     
void 
DataFileResourceManagerBase::NullResourceManage
::release(const std::string& directory,
          const std::string& fileName)
{
}

void 
DataFileResourceManagerBase::NullResourceManage
::remove(const std::string& directory,
         const std::string& fileName)
{
}

DataFileResourceManagerBase::ElementMatcher
::~ElementMatcher()
{
}

bool 
DataFileResourceManagerBase::NullElementMatcher
::match(const std::string& element) const
{
  return true;
}

} // namespace Platform
} // namespace Gcad
