#include "CanvasRenderingContext2DWin.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <utils/JCCrypto.h>
#include <utils/Log.h>

extern HWND g_hWnd;

namespace laya
{
static int s_count = 0;
CanvasRenderingContext2DWin::CanvasRenderingContext2DWin(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    if (s_count == 0)
    {
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartupInput gdiStartupInput;
        Gdiplus::Status gdiStatus = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiStartupInput, NULL);
        if (Gdiplus::Status::Ok != gdiStatus)
            return;
    }
    s_count++;
    m_gdiBitmap = new Gdiplus::Bitmap(m_width, m_height, PixelFormat32bppARGB);
    m_gdiGraphics = new Gdiplus::Graphics(m_gdiBitmap);

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];

    // setDefault();
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
wchar_t *utf8ToUtf16(const std::string &str, int *pRetLen /* = nullptr*/)
{
    wchar_t *pwszBuffer = nullptr;
    do
    {
        if (str.empty())
        {
            break;
        }
        int nLen = static_cast<int>(str.size());
        int nBufLen = nLen + 1;
        pwszBuffer = new wchar_t[nBufLen];
        assert(!pwszBuffer);
        memset(pwszBuffer, 0, sizeof(wchar_t) * nBufLen);
        // str.size() not equal actuallyLen for Chinese char
        int actuallyLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), nLen, pwszBuffer, nBufLen);
        // SE_LOGE("_utf8ToUtf16, str:%s, strLen:%d, retLen:%d\n", str.c_str(), str.size(), actuallyLen);
        if (pRetLen != nullptr)
        {
            *pRetLen = actuallyLen;
        }
    } while (false);
    return pwszBuffer;
}
void CanvasRenderingContext2DWin::fillText(const std::string &text, double x, double y, std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    int bufferLen = 0;
    wchar_t *pwszBuffer = utf8ToUtf16(text, &bufferLen);

    WCHAR *p = NULL;
    Gdiplus::FontFamily fontfamily(L"Arial");

    Gdiplus::Font font(&fontfamily, 60, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    Gdiplus::StringFormat strFormat;
    strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);     // 水平居左
    strFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直居中
    m_gdiGraphics->DrawRectangle(&Gdiplus::Pen(Gdiplus::Color::Red, 2), Gdiplus::RectF(0, 0, 480, 480));
    // m_gdiGraphics->DrawString(pwszBuffer, bufferLen, &font, Gdiplus::RectF(0, 0, 480, 480), &strFormat,
    // &Gdiplus::SolidBrush(Gdiplus::Color::Black));
}

void CanvasRenderingContext2DWin::strokeText(const std::string &text, double x, double y,
                                             std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    int bufferLen = 0;
    wchar_t *pwszBuffer = utf8ToUtf16(text, &bufferLen);

    WCHAR *p = NULL;
    Gdiplus::FontFamily fontfamily(L"Arial");

    Gdiplus::Font font(&fontfamily, 60, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    Gdiplus::StringFormat strFormat;
    strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);     // 水平居左
    strFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直居中
    m_gdiGraphics->DrawRectangle(&Gdiplus::Pen(Gdiplus::Color::Red, 2), Gdiplus::RectF(0, 0, 480, 480));
    // m_gdiGraphics->DrawString(pwszBuffer, bufferLen, &font, Gdiplus::RectF(0, 0, 480, 480), &strFormat,
    // &Gdiplus::SolidBrush(Gdiplus::Color::Black));
}

TextMetrics CanvasRenderingContext2DWin::measureText(const std::string &text)
{

    TextMetrics metrics;

    Gdiplus::GraphicsPath graphicsPathObj;

    Gdiplus::FontFamily fontfamily(L"Arial");

    Gdiplus::Font font(&fontfamily, 60, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::StringFormat strFormat;
    strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);     // 水平居左
    strFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直居中
    // Gdiplus::FontFamily fontfamily;
    // font.GetFamily(&fontfamily);

    int bufferLen = 0;
    wchar_t *pwszBuffer = utf8ToUtf16(text, &bufferLen);

    graphicsPathObj.AddString(pwszBuffer, bufferLen /* -1 */, &fontfamily, font.GetStyle(), font.GetSize(),
                              Gdiplus::PointF(0, 0), &strFormat);
    Gdiplus::RectF rcBound;
    /// 获取边界范围
    graphicsPathObj.GetBounds(&rcBound);

    Gdiplus::RectF layoutRect(0, 0, m_width, m_height);
    m_gdiGraphics->MeasureString(pwszBuffer, bufferLen, &font, layoutRect, &strFormat, &rcBound);

    /// 返回文本的宽高
    metrics.m_width = rcBound.Width;
    metrics, m_height = rcBound.Height;
    LOGE("%f %f", rcBound.Width, rcBound.Height);
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

static std::string toBase64(const char *type, float encoderOptions, char *pPixels, int nABLen, int w, int h, bool flipY)
{
    int size = sizeof(GLubyte) * w * h * 4;
    if (w == 0 || h == 0 || size != nABLen)
    {
        const char *pstrHeader = "data:";
        int length = strlen(pstrHeader);
        std::unique_ptr<char[]> pDest(new char[length + 1]);
        memcpy(pDest.get(), pstrHeader, length);
        pDest.get()[length] = '\0';
        return std::string(pDest.get());
    }

    if (flipY)
    {
        // laya::flipPixelsY((uint8_t*)pPixels, w * 4, h);
    }

    std::string strType(type);
    int length = (size + 2) / 3 * 4;
    std::unique_ptr<char[]> pDest(new char[length]);
    memset(pDest.get(), 0, length);
    char *pCurrent = pDest.get();
    std::pair<unsigned char *, unsigned long> result;
    if (strType == "image/jpeg")
    {
        const char *pstrHeader = "data:image/jpeg;base64,";
        int length = strlen(pstrHeader);
        for (int i = 0; i < length; i++)
        {
            *pCurrent = pstrHeader[i];
            pCurrent++;
        }
        result = convertBitmapToJpeg((const char *)pPixels, w, h, 32);
    }
    else
    {
        const char *pstrHeader = "data:image/png;base64,";
        int length = strlen(pstrHeader);
        for (int i = 0; i < length; i++)
        {
            *pCurrent = pstrHeader[i];
            pCurrent++;
        }
        result = laya::convertBitmapToPng((const char *)pPixels, w, h, 8);
    }

    base64Encode(pCurrent, (const char *)result.first, result.second);
    delete[] result.first;
    return std::string(pDest.get());
}
ImageData CanvasRenderingContext2DWin::getImageData(double x, double y, double width, double height)
{
    int clampedX = std::clamp(x, 0.0, static_cast<double>(m_width));
    int clampedY = std::clamp(y, 0.0, static_cast<double>(m_height));
    int clampedW = std::clamp(width, 0.0, static_cast<double>(m_width));
    int clampedH = std::clamp(height, 0.0, static_cast<double>(m_height));

    Gdiplus::Rect bitmapArea(clampedX, clampedY, clampedW, clampedH);
    Gdiplus::BitmapData bitmapData;
    m_gdiBitmap->LockBits(&bitmapArea, Gdiplus::ImageLockModeRead, m_gdiBitmap->GetPixelFormat(), &bitmapData);

    const BYTE *imageData = (BYTE *)bitmapData.Scan0;
    int stride = bitmapData.Stride;
    unsigned char *glImageData = NULL;
    if (clampedW > 0 && clampedH > 0 && imageData != 0)
    {
#define CUR_LINE ((clampedH - 1 - y) * stride)
#define CUR_POS (y * clampedW + x)

        ImageData data;
        data.m_width = clampedW;
        data.m_height = clampedH;
        data.m_data.resize(clampedW * clampedH * 4);
        COLORREF *pImage = nullptr;

        unsigned char *glImageData = &data.m_data[0];
        const int destR = 0, destG = 1, destB = 2, destA = 3;
        int srcR, srcG, srcB, srcA, bytesPerPixel;

        // 根据像素格式设置初始值
        switch (bitmapData.PixelFormat)
        {
        case PixelFormat24bppRGB:
            srcR = 2;
            srcG = 1;
            srcB = 0;
            bytesPerPixel = 3;
            break;
        case PixelFormat32bppARGB:
            srcA = 3;
            srcR = 2;
            srcG = 1;
            srcB = 0;
            bytesPerPixel = 4;
            break;
        default: // 图片的像素格式不支持
            // delete[] glImageData;
            m_gdiBitmap->UnlockBits(&bitmapData);
            // delete pBitmap;
            LOGE("");
            return ImageData();
        }

        // 复制及转换图像数据
        for (unsigned int y = 0; y < clampedH; ++y)
        {
            for (unsigned int x = 0; x < clampedW; ++x)
            {
                glImageData[CUR_POS * 4 + destR] = imageData[CUR_LINE + x * bytesPerPixel + srcR];
                glImageData[CUR_POS * 4 + destG] = imageData[CUR_LINE + x * bytesPerPixel + srcG];
                glImageData[CUR_POS * 4 + destB] = imageData[CUR_LINE + x * bytesPerPixel + srcB];
                if (bytesPerPixel == 4)
                    glImageData[CUR_POS * 4 + destA] = imageData[CUR_LINE + x * bytesPerPixel + srcA];
                else
                    glImageData[CUR_POS * 4 + destA] = 255;
            }
        }
        m_gdiBitmap->UnlockBits(&bitmapData);
        // delete pBitmap;

        printf("%s \n", toBase64("image/png", 0.8f, (char *)&data.m_data[0], data.m_data.size(), data.m_width,
                                 data.m_height, false)
                            .c_str());
        return data;
    }
    else
    {
        LOGE("");
        return ImageData();
    }
}
const BitmapData &CanvasRenderingContext2DWin::getBitmapData() const
{
    return m_bitmapData;
    // return getImageData(0, 0, m_width, m_height);
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
    // setTypeface(env, m_fontDescription.m_family, style);
    // setTextSize(env, m_fontDescription.m_size);
}
bool CanvasRenderingContext2DWin::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    return true;
}
} // namespace laya
