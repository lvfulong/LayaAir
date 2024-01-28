#ifndef __QuickSort_H__
#define __QuickSort_H__

#include <utils/JCSingletonList.h>

namespace laya
{
class GLESRenderElement3D;
class QuickSort ///: public ISortPass
{
  public:
    ~QuickSort();
    void sort(JCSingletonList<GLESRenderElement3D *> *elements, bool isTransparent, int left, int right);

  private:
    void _quickSort(int left, int right);

    int _partitionRenderObject(int left, int right);

    int _compare(GLESRenderElement3D *left, GLESRenderElement3D *right);

  private:
    JCSingletonList<GLESRenderElement3D *> *m_pElementArray = nullptr;
    bool m_bIsTransparent = false;
};
} // namespace laya
#endif