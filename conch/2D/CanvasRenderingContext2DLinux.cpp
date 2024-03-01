#include "CanvasRenderingContext2DLinux.h"
#include <utils/Log.h>
extern Display *g_X11_display;
extern Window g_X11_window;
namespace laya
{
#define RGBA(r, g, b, a) (int)((int)r | (((int)g) << 8) | (((int)b) << 16) | (((int)a) << 24))
CanvasRenderingContext2DLinux::CanvasRenderingContext2DLinux(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    m_display = g_X11_display;
    m_window = g_X11_window;

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];

    m_pixmap = XCreatePixmap(m_display, m_window, m_width, m_height, 32);
    m_gc = XCreateGC(m_display, m_pixmap, 0, 0);
    setDefault();
}
CanvasRenderingContext2DLinux::~CanvasRenderingContext2DLinux()
{
    XFreePixmap(m_display, m_pixmap);
    XFreeGC(m_display, m_gc);
}
void CanvasRenderingContext2DLinux::setLineWidth(double lineWidth)
{
}
void CanvasRenderingContext2DLinux::setLineJoin(const char *lineJoin)
{
}
void CanvasRenderingContext2DLinux::fillText(const std::string &text, double x, double y,
                                             std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    Point offsetPoint = convertDrawPoint(Point{x, y}, text);
    XSetForeground(m_display, m_gc, RGBA( m_fillColorR, m_fillColorG, m_fillColorB, m_fillColorA);
    XSetFont(m_display, m_gc, m_font->fid);
    XDrawString(m_display, m_pixmap, m_gc, offsetPoint[0], offsetPoint[1], text.c_str(), (int)(text.length()));
}
void CanvasRenderingContext2DLinux::strokeText(const std::string &text, double x, double y,
                                               std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
TextMetrics CanvasRenderingContext2DLinux::measureText(const std::string &text)
{

    TextMetrics metrics;

    int ascent = 0;
    int descent = 0;
    int direction = 0;
    XCharStruct overall;
    XQueryTextExtents(m_display, m_font->fid, text.c_str(), text.length(), &direction, &ascent, &descent, &overall);

    metrics.m_width = overall.width;
    metrics.m_height = overall.ascent + overall.descent;
    metrics.m_ascender = ascent;

    return metrics;
}
void CanvasRenderingContext2DLinux::clearRect(double x, double y, double width, double height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    XSetForeground(m_display, m_gc, 0xFF000000);
    XFillRectangle(m_display, m_pixmap, m_gc, x, y, width, height);
}
void CanvasRenderingContext2DLinux::save()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DLinux::restore()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
ImageData CanvasRenderingContext2DLinux::getImageData(double x, double y, double width, double height)
{
    int clampedX = std::clamp(x, 0.0, static_cast<double>(m_width));
    int clampedY = std::clamp(y, 0.0, static_cast<double>(m_height));
    int clampedW = std::clamp(width, 0.0, static_cast<double>(m_width));
    int clampedH = std::clamp(height, 0.0, static_cast<double>(m_height));

    if (clampedW > 0 && clampedH > 0)
    {
        ImageData data;
        data.m_width = clampedW;
        data.m_height = clampedH;
        data.m_data.resize(clampedW * clampedH * 4);

        XImage *image = XGetImage(m_display, m_pixmap, 0, 0, clampedW, clampedH, AllPlanes, ZPixmap);
        int width = image->width;
        int height = image->height;
        unsigned char *glImageData = &data.m_data[0];
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; x++)
            {
                *(((int *)data + (y * width) + x)) = static_cast<int>(XGetPixel(image, x, y));
            }
        }

        return data;
    }

    return ImageData();
}
const BitmapData &CanvasRenderingContext2DLinux::getBitmapData() const
{
    return m_bitmapData;
}
void CanvasRenderingContext2DLinux::setTransform(double a, double b, double c, double d, double e, double f)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DLinux::scale(double x, double y)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DLinux::setFont(const char *font)
{
    CanvasRenderingContext2D::setFont(font);
    bool isBold = m_fontDescription.isBold();
    bool isItalic = m_fontDescription.isItalic();

    char buffer[1024] = {0};
    snprintf(buffer, sizeof(buffer) - 1, "*%s%s%s*--%d*", m_fontDescription.m_family.c_str(), isBold ? "*Bold" : "",
             isItalic ? "*I" : "", m_fontDescription.m_size);
    if (m_bitmapData)
    {
        XFreeFont(_dis, m_font);
        m_font = 0;
    }

    m_font = XLoadQueryFont(_dis, serv);
    if (!m_font)
    {

        std::string fontName = "helvetica";
        char buffer[1024] = {0};
        snprintf(buffer, sizeof(buffer) - 1, "*%s%s%s*--%d*", fontName.c_str(), isBold ? "*Bold" : "",
                 isItalic ? "*I" : "", m_fontDescription.m_size);
        m_font = XLoadQueryFont(_dis, serv);
        LOGI("use default font %s", fontName.c_str());
    }
}
void CanvasRenderingContext2DLinux::getTextPosition(const std::string &text, double x, double y, double &outX,
                                                    double &outY)
{

    int ascent = 0;
    int descent = 0;
    int direction = 0;
    XCharStruct overall;
    XQueryTextExtents(_dis, _font->fid, text.c_str(), text.length(), &direction, &ascent, &descent, &overall);
    int width = overall.width;

    if (m_textAlign == TextAlign::Center)
    {
        outX = x - width / 2.0f;
    }
    else if (m_textAlign == TextAlign::Left)
    {
    }
    else if (m_textAlign == TextAlign::Right)
    {
        outX = x - width;
    }

    if (m_textBaseline == TextBaseline::Top)
    {
        outY = y + overall.ascent;
    }
    else if (m_textBaseline == TextBaseline::Middle)
    {
        outY = y + (overall.descent - overall.ascent) / 2 - overall.descent;
    }
    else if (m_textBaseline == TextBaseline::Bottom)
    {
        outY = y - overall.descent;
    }
    else if (m_textBaseline == TextBaseline::Alphabetic)
    {
        // outY = y - overall.ascent;;
    }
}
bool CanvasRenderingContext2DLinux::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    return true;
}
} // namespace laya
