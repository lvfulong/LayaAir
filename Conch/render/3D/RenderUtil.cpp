#include "render/3D/RenderUtil.h"

namespace laya{

    static void _quickSort(JCSingletonList<laya::RenderElement3D*>& list, uint32_t left, uint32_t right, bool nearToFar)
    {
        if (list.getLength() > 1)
        {
            uint32_t index = _partitionRenderObject(list, left, right, nearToFar);
            uint32_t leftIndex = index - 1;
            if (left < leftIndex)
                _quickSort(list, left, leftIndex, nearToFar);

            if (index < right)
                _quickSort(list, index, right, nearToFar);
        }
    }

    static uint32_t _partitionRenderObject(JCSingletonList<laya::RenderElement3D*>& list, uint32_t left, uint32_t right, bool nearToFar)
    {
        std::vector<laya::RenderElement3D*>& elements = list.m_vElements;
        uint32_t tempIndex = floor((right + left) / 2.0f);
        laya::RenderElement3D* pivot = elements[tempIndex];
        while (left <= right)
        {
            while (_compare(list, elements[left], pivot, nearToFar) < 0)
                left++;
            while (_compare(list, elements[right], pivot, nearToFar) > 0)
                right--;
            if (left < right)
            {
                laya::RenderElement3D* temp = elements[left];
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

    static uint32_t _compare(laya::RenderElement3D* left, laya::RenderElement3D* right, bool nearToFar)
    {
        uint32_t renderQueue = left->composeData->renderQueue - right->composeData->renderQueue;
        if (renderQueue == 0)
        {
            uint32_t sort = (!nearToFar) ? right->composeData->m_nDistanceForSort - left->composeData->m_nDistanceForSort : left->composeData->m_nDistanceForSort - right->composeData->m_nDistanceForSort;
            return sort + right->composeData->m_nSortingFudge - left->composeData->m_nSortingFudge;
        }
        else
        {
            return renderQueue;
        }
    }

    void RenderUtil::opaqueRenderSort(JCSingletonList<laya::RenderElement3D*>& list, uint32_t left, uint32_t right){
       _quickSort(list,left,right,true);
    }

    void RenderUtil::transparentSort(JCSingletonList<laya::RenderElement3D*>& list, uint32_t left, uint32_t right){
        _quickSort(list, left, right, false);
    }

 
    bool RenderUtil::cullingRenderBounds(const laya::Bounds& bounds, const CullInfo& cullInfo)
    {
        int cullPlaneCount = cullInfo._directLightFrustumCullInfo._cullPlaneCount;
        const std::vector<Plane>& cullPlanes = cullInfo._directLightFrustumCullInfo._cullPlanes;

        const Vector3& min = bounds.getMin();
        const Vector3& max = bounds.getMax();
        float minX = min.x;
        float minY = min.y;
        float minZ = min.z;
        float maxX = max.x;
        float maxY = max.y;
        float maxZ = max.z;
        //TODO:ͨ������ü�ֱ��pass

        bool pass = true;
        // cull by planes
        // Improve:Maybe use sphre and direction cull can savle the far plane cull
        for (int j = 0; j < cullPlaneCount; j++)
        {
            const Plane& plane = cullPlanes[j];
            const Vector3& normal = plane.normal;
            if (plane.distance + (normal.x * (normal.x < 0.0 ? minX : maxX)) + (normal.y * (normal.y < 0.0 ? minY : maxY)) + (normal.z * (normal.z < 0.0 ? minZ : maxZ)) < 0.0)
            {
                pass = false;
                break;
            }
        }
        return pass;
    }
    void RenderUtil::cullByCameraCullInfo(const CullInfo& cullInfo, const JCSingletonList<laya::RenderElement3D*>& cullListIn, JCSingletonList<RenderElement*>& cullListOut)
    {

    }
    void RenderUtil::cullByShadowCullInfo(const CullInfo& cullInfo, const JCSingletonList<laya::RenderElement3D*>& cullListIn, JCSingletonList<RenderElement*>& cullListOut)
    {
        /*cullListOut.resetLength();
        const std::vector<laya::RenderElement3D*>& renders = cullListIn.m_vElements;
        for (int i = 0, n = cullListIn.getLength(); i < n; i++)
        {
            laya::RenderElement3D* render = renders[i];
            bool canPass = render->shadowCullPass();
            if (canPass)
            {
                //lvtodo Stat.frustumCulling++;
                if (cullingRenderBounds(render->composeData->bounds, cullInfo))
                {
                    cullListOut.add(render);
                }
            }
        }*/
    }
    void RenderUtil::cullingSpotShadow(const CullInfo& cullInfo, const JCSingletonList<RenderElement*>& cullListIn, JCSingletonList<RenderElement*>& cullListOut)
    {
        /*cullListOut.resetLength();
        const std::vector<laya::RenderElement3D*>& renders = cullListIn.m_vElements;
        const BoundFrustum& boundFrustum = cullInfo._cameraFrustumCullInfo._boundFrustum;
        int index = 0;
        for (int i = 0, n = cullListIn.getLength(); i < n; i++)
        {
            laya::RenderElement3D* render = renders[i];
            bool canPass = render->composeData.castShadow && render->composeData.enable && (render->composeData.renderbitFlag == 0);
            if (canPass)
            {
                //lvtodo Stat.frustumCulling++;
                //lvtodo if (render->_needRender(boundFrustum))
                {
                    cullListOut.add(render);
                }
            }
        }*/
    }
}