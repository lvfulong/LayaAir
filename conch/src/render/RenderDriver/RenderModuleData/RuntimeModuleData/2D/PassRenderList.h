#ifndef __PASSRENDERLIST_H__
#define __PASSRENDERLIST_H__

namespace laya
{

class PassRenderList {
public:
    PassRenderList();
    ~PassRenderList();

    void add(WebRenderStruct2D* struct);
    void remove(WebRenderStruct2D* struct);
    void clear();
    void clearRenderElements();
    void reset();
    void batch();

private:
    void _batchStart(int type, int elementLength);

    FastSinglelist<Batch2DInfo*> _batchInfoList;
    int _currentType = -1;
    int _currentElementCount = 0;
    Batch2DInfo* _currentBatch = nullptr;
    FastSinglelist<WebRenderStruct2D*>* structs = nullptr;
    FastSinglelist<IRenderElement2D*>* renderElements = nullptr;
    int renderListType = -1;
    int zOrder = 0;
    uint32_t _dirtyFlag = 0;
}; 
} // namespace laya
#endif // __PASSRENDERLIST_H__