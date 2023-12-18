#ifndef __ImageBitmap_H__
#define __ImageBitmap_H__

#include <Image/JCImage.h>
#include <map>
#include <stdio.h>
#include <string>

namespace laya
{
class ImageBitmap
{
  public:
    ImageBitmap();

    ~ImageBitmap();

    int getWidth();

    int getHeight();

    void close();

    int getImageID();

  public:
    std::shared_ptr<JCImage> m_image;
};
} // namespace laya
#endif