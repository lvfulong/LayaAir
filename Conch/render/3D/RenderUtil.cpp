#include "RenderUtil"
namespace layaRender{
    void RenderUtil::opaqueRenderSort(SingleList<RenderElement*>& list, uint32_t left, uint32_t right){
       _quickSort(list,left,right,true);
    }

    void RenderUtil::transparentSort(SingleList<RenderElement*>& list, uint32_t left, uint32_t right){
        _quickSort(list,left,right,false)
    }

  static void _quickSort(SingleList<RenderElement*>& list,uint32_t left, uint32_t right,bool nearToFar)
    {
        if (list->getLength() > 1)
        {
            uint32_t index = _partitionRenderObject(list,left, right,nearToFar);
            uint32_t leftIndex = index - 1;
            if (left < leftIndex)
                _quickSort(list,left, leftIndex,nearToFar);

            if (index < right)
                _quickSort(list,index, right,nearToFar);
        }
    }

    static uint32_t _partitionRenderObject(SingleList<RenderElement*>& list,uint32_t left, uint32_t right,bool nearToFar)
    {
        std::vector<RenderElement*>& elements = list.m_vElements;
        uint32_t tempIndex = floor((right + left) / 2.0f);
        RenderElement* pivot = elements[tempIndex];
        while (left <= right)
        {
            while (_compare(list,elements[left], pivot,nearToFar) < 0)
                left++;
            while (_compare(list,elements[right], pivot,nearToFar) > 0)
                right--;
            if (left < right) 
            {
                RenderElement* temp = elements[left];
                elements[left] = elements[right];
                elements[right] = temp;
                left++;
                right--;
            }
            else if (left == right)
            {
                left++;
                break;
            }
        }
        return left;
    }

    static uint32_t _compare(RenderElement* left , RenderElement* right,bool nearToFar)
    {
        uint32_t renderQueue = left->composeData.m_nRenderQueue - right->composeData.m_nRenderQueue;
        if (renderQueue == 0) 
        {
            uint32_t sort = (!nearToFar) ? right->composeData.m_nDistanceForSort - left->composeData.m_nDistanceForSort : left->composeData.m_nDistanceForSort - right->composeData.m_nDistanceForSort;
            return sort + right->composeData->m_nSortingFudge - left->composeData.m_nSortingFudge;
        }
        else
        {
            return renderQueue;
        }
    }
    void RenderUtil::cullByCameraCullInfo(const CullInfo& cullInfo, const SingleList<RenderElement*>& cullListIn, SingleList<RenderElement*>& cullListOut)
    {

    }
    void RenderUtil::cullByShadowCullInfo(const CullInfo& cullInfo, const SingleList<RenderElement*>& cullListIn, SingleList<RenderElement*>& cullListOut) 
    {

    }
    void RenderUtil::cullingSpotShadow(const CullInfo& cameraCullInfo, const SingleList<RenderElement*>& cullListIn, SingleList<RenderElement*>& cullListOut)
    {
        cullListOut.setLength(0);
        std::vector<RenderElement*>& renders = cullListIn.m_vElements;
        const BoundFrustum& boundFrustum = _cameraFrustumCullInfo._boundFrustum;
        int index = 0;
        for (int i = 0, n = cullListIn.getLength(); i < n; i++)
        {
            RenderElement* render = renders[i];
            //bool canPass = render->getCastShadow()/*&& render._enabled*/ && (render->m_renderbitFlag == 0);
            //if (canPass)
            //{
                //lvtodo Stat.frustumCulling++;
                //if (render->_needRender(boundFrustum))
                //{
                //}
            //}
        }

    }
}
#endif