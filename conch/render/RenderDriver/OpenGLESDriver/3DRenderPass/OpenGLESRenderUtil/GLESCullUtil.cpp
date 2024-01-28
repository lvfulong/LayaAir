#include "GLESCullUtil.h"
#include "render/3D/FrustumCulling.h"

namespace laya
{
void GLESCullUtil::cullByCameraCullInfo(CameraCullInfo &cameraCullInfo, std::vector<RTBaseRenderNode *> &list,
                                        uint32_t count, GLESRenderListQueue &opaqueList,
                                        GLESRenderListQueue &transparent, RTRenderContext3D *context)
{
    std::vector<RTBaseRenderNode *> &renders = list;
    BoundFrustum &boundFrustum = cameraCullInfo._boundFrustum;
    uint32_t cullMask = cameraCullInfo._cullingMask;
    uint32_t staticMask = cameraCullInfo._staticMask;
    for (int i = 0, n = count; i < n; i++)
    {
        auto render = renders[i];
        bool canPass;
        canPass = (static_cast<uint32_t>(pow(static_cast<uint32_t>(2), render->layer)) & cullMask) != 0 &&
                  (render->renderbitFlag == 0);
        canPass = canPass && ((render->staticMask & staticMask) != 0);
        if (canPass)
        {
            // Stat.frustumCulling++;todo
            //  needRender 方案有问题 会造成native和js的差异
            if (!cameraCullInfo._useOcclusionCulling || render->_needRender(&boundFrustum)) // NEEDRENDER TS OR NATIVE
            {
                render->distanceForSort = Vector3::distance(render->getBounds()->getCenter(), cameraCullInfo._position);
                render->_renderUpdatePre(context); // TS OR Native
                std::vector<GLESRenderElement3D *> &elements = render->renderelements;
                if (elements.size() == 1)
                { // js 优化
                    if (elements[0]->materialRenderQueue > 2500)
                        transparent.addRenderElement(elements[0]);
                    else
                        opaqueList.addRenderElement(elements[0]);
                }
                else
                {
                    for (int j = 0, m = elements.size(); j < m; j++)
                    {
                        GLESRenderElement3D *element = elements[j];
                        if (element->materialRenderQueue > 2500)
                            transparent.addRenderElement(element);
                        else
                            opaqueList.addRenderElement(element);
                    }
                }
            }
        }
    }
}

void GLESCullUtil::culldirectLightShadow(const ShadowCullInfo &shadowCullInfo, std::vector<RTBaseRenderNode *> &list,
                                         uint32_t count, GLESRenderListQueue &opaqueList, RTRenderContext3D *context)
{
    opaqueList.clear();
    std::vector<RTBaseRenderNode *> &renders = list;
    for (int i = 0, n = count; i < n; i++)
    {
        auto render = renders[i];
        bool canPass = render->shadowCullPass();
        if (canPass)
        {
            // Stat.frustumCulling++; todo
            bool pass = FrustumCulling::cullingRenderBounds(render->getBounds(), shadowCullInfo);
            if (pass)
            {
                render->distanceForSort = Vector3::distance(
                    render->getBounds()->getCenter(), shadowCullInfo.position); // TODO:合并计算浪费,或者合并后取平均值
                render->_renderUpdatePre(context);                              // TS OR Native
                std::vector<GLESRenderElement3D*> &elements = render->renderelements;
                for (int j = 0, m = elements.size(); j < m; j++)
                {
                    GLESRenderElement3D*element = elements[j];
                    if (element->materialRenderQueue < 2500)
                        opaqueList.addRenderElement(element);
                }
            }
        }
    }
}

void GLESCullUtil::cullingSpotShadow(CameraCullInfo &cameraCullInfo, std::vector<RTBaseRenderNode *> &list,
                                     uint32_t count, GLESRenderListQueue &opaqueList, RTRenderContext3D *context)
{
    opaqueList.clear();
    std::vector<RTBaseRenderNode *> &renders = list;
    BoundFrustum &boundFrustum = cameraCullInfo._boundFrustum;
    for (int i = 0, n = count; i < n; i++)
    {
        auto render = renders[i];
        bool canPass = render->shadowCullPass();
        render->_renderUpdatePre(context); // TS OR Native
        if (canPass)
        {
            // Stat.frustumCulling++; todo
            render->distanceForSort = Vector3::distance(render->getBounds()->getCenter(), cameraCullInfo._position);
            if (render->_needRender(&boundFrustum))
            {
                std::vector<GLESRenderElement3D*> &elements = render->renderelements;
                for (int j = 0, m = elements.size(); j < m; j++)
                {
                    GLESRenderElement3D*element = elements[j];
                    if (element->materialRenderQueue < 2500)
                        opaqueList.addRenderElement(element);
                }
            }
        }
    }
}
} // namespace laya
