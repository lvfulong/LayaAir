#include "CanvasRenderingContext2DWin.h"
#include <utils/Log.h>

namespace laya
{

CanvasRenderingContext2DWin::CanvasRenderingContext2DWin(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
   

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];

    //setDefault();
}
CanvasRenderingContext2DWin::~CanvasRenderingContext2DWin()
{
    
}
void CanvasRenderingContext2DWin::setLineWidth(double lineWidth)
{
    
}
void CanvasRenderingContext2DWin::setLineJoin(const char *lineJoin)
{
    
}
void CanvasRenderingContext2DWin::fillText(const std::string &text, double x, double y,
                                               std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }

    
}
void CanvasRenderingContext2DWin::strokeText(const std::string &text, double x, double y,
                                                 std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
   
}
TextMetrics CanvasRenderingContext2DWin::measureText(const std::string &text)
{
   
    TextMetrics metrics;

    //metrics.m_width = ceilf(width);
    //metrics.m_height = ceilf(height);
    //metrics.m_ascender = ceilf(ascent);
    return metrics;
}
void CanvasRenderingContext2DWin::clearRect(double x, double y, double width, double height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
   
}
void CanvasRenderingContext2DWin::save()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DWin::restore()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
ImageData CanvasRenderingContext2DWin::getImageData(double x, double y, double width, double height)
{
    //int clampedX = std::clamp(x, 0.0, static_cast<double>(m_width));
    //int clampedY = std::clamp(y, 0.0, static_cast<double>(m_height));
    //int clampedW = std::clamp(width, 0.0, static_cast<double>(m_width));
    //int clampedH = std::clamp(height, 0.0, static_cast<double>(m_height));

    //if (clampedW > 0 && clampedH > 0)
    {
        ImageData data;
        //data.m_width = clampedW;
        //data.m_height = clampedH;
        //data.m_data.resize(clampedW * clampedH * 4);

        return data;
    }

    return ImageData();
}
const BitmapData &CanvasRenderingContext2DWin::getBitmapData() const
{
    return m_bitmapData;
}
void CanvasRenderingContext2DWin::setTransform(double a, double b, double c, double d, double e, double f)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DWin::scale(double x, double y)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DWin::setTextAlign(const char *textAlign)
{
}
void CanvasRenderingContext2DWin::setTextBaseline(const char *textBaseline)
{
}
void CanvasRenderingContext2DWin::setFillStyle(const char *color)
{
}
void CanvasRenderingContext2DWin::setStrokeStyle(const char *color)
{

}
void CanvasRenderingContext2DWin::setFont(const char *font)
{
    CanvasRenderingContext2D::setFont(font);
    bool isBold = m_fontDescription.isBold();
    bool isItalic = m_fontDescription.isItalic();
    int style = 0;
    if (isBold)
    {
        style |= 1;
    }
    if (isItalic)
    {
        style |= 2;
    }
    //setTypeface(env, m_fontDescription.m_family, style);
    //setTextSize(env, m_fontDescription.m_size);
}
bool CanvasRenderingContext2DWin::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    return true;
}
} // namespace laya
