#include "CanvasRenderingContext2DWin.h"
#include <utils/Log.h>
#include <algorithm>
namespace laya
{

CanvasRenderingContext2DWin::CanvasRenderingContext2DWin(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }

    m_hdcScreen = GetDC(NULL);
    m_hBitmap = CreateCompatibleBitmap(m_hdcScreen, m_width, m_height);
    if (m_hBitmap == NULL)
    {
        LOGE("create bitmap failed");
        return;
    }

    m_hdcMem = CreateCompatibleDC(m_hdcScreen);
    m_hbmOld = (HBITMAP)SelectObject(m_hdcMem, m_hBitmap);

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];

    //setDefault();
}
CanvasRenderingContext2DWin::~CanvasRenderingContext2DWin()
{
    SelectObject(m_hdcMem, m_hbmOld);
    DeleteDC(m_hdcMem);
    DeleteObject(m_hBitmap);

    ReleaseDC(NULL, m_hdcScreen);
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
    LPCSTR szText = "Hello, GDI!";
    SetTextColor(m_hdcMem, RGB(255, 0, 0)); // 设置文本颜色为红色
    SetBkMode(m_hdcMem, TRANSPARENT);       // 设置文本背景为透明
    TextOut(m_hdcMem, 300, 150, szText, lstrlenA(szText)); // 位置(300, 150)
    
}
void CanvasRenderingContext2DWin::strokeText(const std::string &text, double x, double y,
                                                 std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    LPCSTR szText = "Hello, GDI!";
    SetTextColor(m_hdcMem, RGB(255, 0, 0)); // 设置文本颜色为红色
    SetBkMode(m_hdcMem, TRANSPARENT);       // 设置文本背景为透明
    TextOut(m_hdcMem, 300, 150, szText, lstrlenA(szText)); // 位置(300, 150)
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
    int clampedX = std::clamp(x, 0.0, static_cast<double>(m_width));
    int clampedY = std::clamp(y, 0.0, static_cast<double>(m_height));
    int clampedW = std::clamp(width, 0.0, static_cast<double>(m_width));
    int clampedH = std::clamp(height, 0.0, static_cast<double>(m_height));
    if (clampedW > 0 && clampedH > 0)
    {
        BITMAP bmpInfo;
        if (GetObject(m_hBitmap, sizeof(BITMAP), &bmpInfo)) {
            BITMAPINFOHEADER bi;
            ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
            bi.biSize = sizeof(BITMAPINFOHEADER);
            bi.biWidth = bmpInfo.bmWidth;
            bi.biHeight = -bmpInfo.bmHeight; // 使图片从上到下
            bi.biPlanes = 1;
            bi.biBitCount = 32;  // 假设我们想获取一个32位色彩深度的位图
            bi.biCompression = BI_RGB;

            // 计算位图图片的大小（行必须是4字节对齐）
            DWORD dwBmpSize = ((bmpInfo.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpInfo.bmHeight;

            ImageData data;
            std::vector<BYTE> buffer(dwBmpSize);

            BITMAPINFO biInfo;
            ZeroMemory(&biInfo, sizeof(BITMAPINFO));
            biInfo.bmiHeader = bi;

            if (GetDIBits(m_hdcMem, m_hBitmap, 0, (UINT)bmpInfo.bmHeight, &buffer[0], &biInfo, DIB_RGB_COLORS)) {
                ImageData data;
                data.m_width = clampedW;
                data.m_height = clampedH;
                data.m_data.resize(clampedW * clampedH * 4);
                int start = 0;
                int bytePerLineDes = clampedW * 4;
                int bytePerLineSrc = m_width * 4;
                int startI = clampedH + clampedY - 1;
                // for (int i = startI; i >= clampedY; i--)
                for (int i = clampedY; i < clampedH + clampedY; i++)
                {
                    std::memcpy(static_cast<void*>(&data.m_data[start]),
                        &buffer[clampedX * 4 + i * bytePerLineSrc], bytePerLineDes);
                    start += bytePerLineDes;
                }
                return data;
            }
            else {
                LOGE("");
            }
        }
        else {
            LOGE("");
        }
    }
    return ImageData();
    // ...
    // Release resources and clean up here if necessary
    // ...
}
const BitmapData &CanvasRenderingContext2DWin::getBitmapData() const
{
    return m_bitmapData;
    //return getImageData(0, 0, m_width, m_height);
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
