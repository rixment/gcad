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

#ifndef _GCAD_DEVICECAPABILITIES_H_
#define _GCAD_DEVICECAPABILITIES_H_

namespace Gcad {
namespace RenderingSystem {

/**
  @brief
*/
class DeviceCapabilities {
 public:
   virtual ~DeviceCapabilities();

   enum DeviceType {
     DT_HAL,
     DT_REF,
     DT_SW 
   };   
   virtual DeviceType getDeviceType() const = 0;
 
   virtual bool supportReturnCurrScanLine() const = 0;
   virtual bool supportAutoGenMipMaps() const = 0;
   virtual bool supportCalibrateGammaRamp() const = 0;
   virtual bool supportManageResource() const = 0;  
   virtual bool supportDynamicTextures() const = 0;
   virtual bool supportDynamicGammaRampInFullScreen() const = 0;
   virtual bool supportAlphaBlendOnFullScrUsingFlipOrDiscard() const = 0;
   virtual bool supportAccelerateCopyToVideoMem() const = 0;
   virtual bool supportAccelerateCopyToSystemMem() const = 0;
   virtual bool supportLinearTosRGBPresentation() const = 0;

   enum PresentationInterval {
     PI_IMMEDIATE,
     PI_ONE,
     PI_TWO,
     PI_THREE,
     PI_FOUR
   };
   virtual bool supportPresentationInterval(PresentationInterval p) const = 0;

   virtual bool supportBlitsFromSysMemTexturesToVideoMemTextures() const = 0;
   virtual bool canQueueRenderingCommandsAfterPageFlip() const = 0;
   virtual bool supportDrawPrimitives2() const = 0;
   virtual bool supportDrawPrimitives2Ex() const = 0;
   virtual bool canUseExecuteBuffersFromSysMemory() const = 0;
   virtual bool canUseExecuteBuffersFromVideoMemory() const = 0;
   virtual bool hasHardwareAccelerationForSceneRaster() const = 0;
   virtual bool supportHardwareTAndL() const = 0;
   virtual bool supportNPatches() const = 0;
   virtual bool supportHardwareRasterizationTransformationLightningShading() const = 0;
   virtual bool supportQuinticBezierCurvesAndBSplines() const = 0;
   virtual bool supportRectAndTriAngularPatches() const = 0;
   virtual bool texturingFromSeparateMemoryPools() const = 0;
   virtual bool canRetrieveTexturesFromNonLocalVideoMem() const = 0;
   virtual bool canRetreiveTexturesFromSysMem() const = 0;
   virtual bool canRetreiveTexturesFromDevMem() const = 0;
   virtual bool canUseBuffersFromSysMemForTransAndLitVertices() const = 0;
   virtual bool canUseBuffersFromVideoMemForTransAndLitVertices() const = 0;

   //! @todo Wzorujac sie na DXSDK dokonczyc interfejs
};

} // namespace RenderingSystem
} // namespace Gcad

#endif
