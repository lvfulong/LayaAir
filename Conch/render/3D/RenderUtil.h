#ifndef __RenderUtil_H_
#define __RenderUtil_H_

#include "render/tempbase.h"
namespace layaRender{
  public:
  static void opaqueRenderSort(SingleList<RenderElement*>&, uint32_t, uint32_t);
  static void transparentSort(SingleList<RenderElement*>&, uint32_t, uint32_t);
  
}
#endif