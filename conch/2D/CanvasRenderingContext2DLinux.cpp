#include "CanvasRenderingContext2DLinux.h"
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <utils/Log.h>
extern Display *g_X11_display;
extern Window g_X11_window;
namespace laya
{
#define RGBA(r, g, b, a) (int)((int)r | (((int)g) << 8) | (((int)b) << 16) | (((int)a) << 24))
class CanvasRenderingContext2DLinuxImpl
{
  public:
    Display *m_display{nullptr};
    Drawable m_window{0};
    Drawable m_pixmap{0};
    GC m_gc;
    XFontStruct *m_font{0};
};
CanvasRenderingContext2DLinux::CanvasRenderingContext2DLinux(int width, int height)
    : CanvasRenderingContext2D(width, height), m_impl(new CanvasRenderingContext2DLinuxImpl())
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    m_impl->m_display = g_X11_display;
    m_impl->m_window = g_X11_window;

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];

    m_impl->m_pixmap = XCreatePixmap(m_impl->m_display, m_impl->m_window, m_width, m_height, 32);
    m_impl->m_gc = XCreateGC(m_impl->m_display, m_impl->m_pixmap, 0, 0);
    setDefault();
}
CanvasRenderingContext2DLinux::~CanvasRenderingContext2DLinux()
{

    if (m_impl != nullptr)
    {
        XFreePixmap(m_impl->m_display, m_impl->m_pixmap);
        XFreeGC(m_impl->m_display, m_impl->m_gc);
        delete m_impl;
    }
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
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    XSetForeground(m_impl->m_display, m_impl->m_gc, RGBA(m_fillColorR, m_fillColorG, m_fillColorB, m_fillColorA));
    XSetFont(m_impl->m_display, m_impl->m_gc, m_impl->m_font->fid);
    XDrawString(m_impl->m_display, m_impl->m_pixmap, m_impl->m_gc, outX, outY, text.c_str(), (int)(text.length()));
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
    XQueryTextExtents(m_impl->m_display, m_impl->m_font->fid, text.c_str(), text.length(), &direction, &ascent,
                      &descent, &overall);

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
    XSetForeground(m_impl->m_display, m_impl->m_gc, 0x00000000);
    XFillRectangle(m_impl->m_display, m_impl->m_pixmap, m_impl->m_gc, x, y, width, height);
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

        XImage *image = XGetImage(m_impl->m_display, m_impl->m_pixmap, 0, 0, clampedW, clampedH, AllPlanes, ZPixmap);
        int width = image->width;
        int height = image->height;
        unsigned char *glImageData = &data.m_data[0];
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; x++)
            {
                *(((int *)glImageData + (y * width) + x)) = static_cast<int>(XGetPixel(image, x, y));
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
             isItalic ? "*I" : "", (int)m_fontDescription.m_size);
    if (m_impl->m_font)
    {
        XFreeFont(m_impl->m_display, m_impl->m_font);
        m_impl->m_font = 0;
    }

    m_impl->m_font = XLoadQueryFont(m_impl->m_display, buffer);
    if (!m_impl->m_font)
    {

        // std::string fontName = "helvetica";
        // char buffer[1024] = {0};
        // snprintf(buffer, sizeof(buffer) - 1, "*%s%s%s*--%d*", fontName.c_str(), isBold ? "*Bold" : "",
        //         isItalic ? "*I" : "", (int)m_fontDescription.m_size);
        // m_impl->m_font = XLoadQueryFont(m_impl->m_display, buffer);
        // LOGI("use default font %s %p", fontName.c_str(), m_impl->m_font);

        int _fontSize;
        static int fontSizes[] = {8, 10, 12, 14, 18, 24};
        int i = 0;
        int size = sizeof(fontSizes) / sizeof(fontSizes[0]);
        for (i = 0; i < size; ++i)
        {
            if (_fontSize < fontSizes[i])
            {
                break;
            }
        }
        if (i == 0)
        {
            _fontSize = fontSizes[0];
        }
        else if (i > 1 && i < size)
        {
            _fontSize = fontSizes[i - 1];
        }
        else
        {
            _fontSize = fontSizes[size - 1];
        }
        snprintf(buffer, sizeof(buffer) - 1, "*%s*%d*", "lucidasans", _fontSize);
        m_impl->m_font = XLoadQueryFont(m_impl->m_display, buffer);
        if (!m_impl->m_font)
        {
            m_impl->m_font = XLoadQueryFont(m_impl->m_display, buffer);
        }
        LOGI("use default font %s %p", "lucidasans", m_impl->m_font);
    }

    {
        const char *fontname = "-*-fixed-*-r-*-*-20-*-*-*-*-*-*-*";
        m_impl->m_font = XLoadQueryFont(m_impl->m_display, fontname);
        if (!m_impl->m_font)
        {
            fprintf(stderr, "unable to load font %s: using fixed\n", fontname);

            m_impl->m_font = XLoadQueryFont(m_impl->m_display, fontname);
        }
        else
        {
            LOGI("use default font %s %p", fontname, m_impl->m_font);
        }
    }
}
void CanvasRenderingContext2DLinux::getTextPosition(const std::string &text, double x, double y, double &outX,
                                                    double &outY)
{
    int ascent = 0;
    int descent = 0;
    int direction = 0;
    XCharStruct overall;
    XQueryTextExtents(m_impl->m_display, m_impl->m_font->fid, text.c_str(), text.length(), &direction, &ascent,
                      &descent, &overall);
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
