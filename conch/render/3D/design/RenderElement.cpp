#include "RenderElement.h"

namespace laya
{
   ComposeData::ComposeData(){
   }

   ComposeData::~ComposeData(){

   }

    void ComposeData::set_composeData_distanceForsort(float value){
        m_nDistanceForSort = value;
    }
    void ComposeData::set_composeData_sortingFudge(float value){
        m_nSortingFudge = value;
    }
    void ComposeData::set_composeData_renderQueue(uint32_t value){
        renderQueue = value;
    }
    void ComposeData::set_composeData_castShadow(bool value){
        castShadow = value;
    }
    void ComposeData::set_composeData_renderEnable(bool value){
        enable = value;
    }
    void ComposeData::set_composeData_renderbitFlag(uint32_t value){
        renderbitFlag = value;
    }
    void ComposeData::set_composeData_layer(uint32_t value){
        layer = value;
    }
    void ComposeData::set_composeData_bounds(Bounds* value){
        bounds = value;
    }

    void ComposeData::set_composeData_customCull(bool value){
        customCull = value;
    }
    
    void ComposeData::set_composeData_customCullResoult(bool value){
        customCullResoult = value;
    }

    RenderElement3D::RenderElement3D()
    {
        
    }
    RenderElement3D::~RenderElement3D()
    {
        
    }
}
