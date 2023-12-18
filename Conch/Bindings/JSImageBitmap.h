#ifndef __JSImageBitmap_H__
#define __JSImageBitmap_H__

#include <Binder/JSInterface.h>
#include <Image/ImageBitmap.h>
#include <map>
#include <stdio.h>
#include <string>

namespace laya
{
class JSImageBitmap
{
  public:
    static void exportJS(Context &context);

    JSImageBitmap();

    ~JSImageBitmap();

    int getWidth();

    int getHeight();

    void close();

    int getImageID();

  public:
    ImageBitmap m_ImageBitmap;
};
} // namespace laya
#endif