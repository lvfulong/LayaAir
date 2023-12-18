#include "ImageBitmap.h"
#include <Utils/Log.h>
#include <assert.h>

namespace laya
{
ImageBitmap::ImageBitmap()
{
}
ImageBitmap::~ImageBitmap()
{
}
int ImageBitmap::getWidth()
{
    assert(m_image);
    return m_image->getWidth();
}
int ImageBitmap::getHeight()
{
    assert(m_image);
    return m_image->getHeight();
}
void ImageBitmap::close()
{
    // todo
}
int ImageBitmap::getImageID()
{
    assert(m_image);
    return m_image->getImageID();
}
} // namespace laya
