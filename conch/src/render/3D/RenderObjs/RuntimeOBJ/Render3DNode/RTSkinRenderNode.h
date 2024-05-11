#ifndef __RTSkinRenderNode_H_
#define __RTSkinRenderNode_H_

#include "RTBaseRenderNode.h"
#include <vector>
namespace laya
{
    class RTSkinRenderNode :public RTBaseRenderNode {
        struct SkinnedMatrixCache
        {
            int subMeshIndex;
            int batchIndex;
            int batchBoneIndex;
        };
    public:
        RTSkinRenderNode() {};
        ~RTSkinRenderNode() {};
        void _renderUpdate(GLESRenderContext3D* context) override;
    public://binding
        void computeSkinnedData(int loopCount);
        void setRootBoneTransfom(JSTransform* value) { rootBoneTransform = value; };
        void setOwnerTransform(JSTransform* value) { ownerTransform = value; }

        //SkinnedMatrixData
        void resizeSkinnedData(uint32_t dataArrayLength);
        void setSkinnedDataByIndex(uint32_t index, JSValueAsParam pData);
        //bones
        void clearBoneTransform();
        void addBoneTransform(JSTransform* value);
        //--------cacheMeshData--------
        //cacheMesh._inverseBindPoses
        void resizeCacheMeshInverseBindPoes(uint32_t value);
        void setinverseBindPoseDataByIndex(uint32_t index, const Matrix4x4& value);
        //this._cacheMesh._skinnedMatrixCaches
        void resizeMatrixCache(uint32_t value);
        void setMatrixCacheByIndex(uint32_t index, int batchBoneIndex, int batchIndex, int subMeshIndex);
        //cacheMesh.subMeshCount
        void setSubMeshCount(int value) { subMeshCount = value; };
        //cacheMesh._boneIndicesList
        void resizeBoneIndicesList(uint32_t value);
        void setBoneIndicesList(uint32_t index, JSValueAsParam pData);
    private:
        void _computeSubSkinnedData(uint16_t* boneIndices, int boneIndicesLength, float* data, int loopCount);
    public://Property
        JSTransform* rootBoneTransform = nullptr;
        JSTransform* ownerTransform = nullptr;
        std::vector<JSTransform*> bones{};
        std::vector<std::vector<std::pair<float*, int>>> skinnedData{};
        std::vector<Matrix4x4> inverseBindPose{};
        std::vector<std::vector<std::pair<uint16_t*, int>>> boneIndicesList{};
    private:
        std::vector<SkinnedMatrixCache> _skinnedMatrixCache{};//”≈ªØ
        int subMeshCount;
        std::vector<int> _skinnedDataLoopMarks;

    };
} // namespace laya
#endif