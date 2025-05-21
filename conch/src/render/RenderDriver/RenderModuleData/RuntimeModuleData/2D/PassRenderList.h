#ifndef __PASSRENDERLIST_H__
#define __PASSRENDERLIST_H__

#include <utils/FastSinglelist.h>
namespace laya
{
class Batch2DInfo;
class RTRenderStruct2D;
class GLESRenderElement2D;

class PassRenderList {
public:
    PassRenderList();
    ~PassRenderList();

    void add(RTRenderStruct2D* struct2d);
    void remove(RTRenderStruct2D* struct2d);
    void clear();
    void clearRenderElements();
    void reset();
    void batch();

public:
    void _batchStart(int type, int elementLength);

    FastSinglelist<Batch2DInfo*> _batchInfoList;
    int _currentType = -1;
    int _currentElementCount = 0;
    Batch2DInfo* _currentBatch = nullptr;
    FastSinglelist<RTRenderStruct2D*>* structs = nullptr;
    FastSinglelist<GLESRenderElement2D*>* renderElements = nullptr;
    int renderListType = -1;
    int zOrder = 0;
    uint32_t _dirtyFlag = 0;
}; 
} // namespace laya
#endif // __PASSRENDERLIST_H__