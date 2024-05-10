#include "CanvasRenderingContext2DWin.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <regex>
#include <utils/JCCrypto.h>
#include <utils/Log.h>
#include <gdiplusenums.h>
#include <gdiplusgraphics.h>
extern HWND g_hWnd;

using namespace Gdiplus;

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
    m_gdiGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
    m_gdiGraphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];
    // Gdiplus::Matrix matrix;
    // matrix.Translate(0.0f, height);
    // matrix.Scale(1.0f, -1.0f);
    // m_gdiGraphics->SetTransform(&matrix);
    //m_stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);       // 水平
    m_stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear); // 垂直
    setDefault();
}
CanvasRenderingContext2DWin::~CanvasRenderingContext2DWin()
{
}
void CanvasRenderingContext2DWin::setLineWidth(double lineWidth)
{
    m_lineWidth = lineWidth;
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
        assert(pwszBuffer != nullptr);
        memset(pwszBuffer, 0, sizeof(wchar_t) * nBufLen);
        int actuallyLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), nLen, pwszBuffer, nBufLen);
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
    TextMetrics matrics = measureTextUtf16(pwszBuffer, bufferLen);
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    //这个由于windows自己的排版导致左边空隙过大
    //m_gdiGraphics->DrawString(
    //    pwszBuffer, bufferLen, m_font, Gdiplus::PointF(outX, outY), &m_stringFormat,
    //    &Gdiplus::SolidBrush(Gdiplus::Color(m_fillColorA, m_fillColorR, m_fillColorG, m_fillColorB)));

    m_gdiGraphics->DrawString(
        pwszBuffer, bufferLen, m_font, Gdiplus::PointF(outX, outY), Gdiplus::StringFormat::GenericTypographic(),
        &Gdiplus::SolidBrush(Gdiplus::Color(m_fillColorA, m_fillColorR, m_fillColorG, m_fillColorB)));

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
    TextMetrics matrics = measureTextUtf16(pwszBuffer, bufferLen);
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    //m_gdiGraphics->DrawString(pwszBuffer, bufferLen, m_font, Gdiplus::PointF(outX, outY), &m_stringFormat,
    //                          &Gdiplus::SolidBrush(Gdiplus::SolidBrush(
    //                              Gdiplus::Color(m_strokeColorA, m_strokeColorR, m_strokeColorG, m_strokeColorB))));

    FontFamily fontFamily;
    m_font->GetFamily(&fontFamily);
    
    int size = m_font->GetSize();

    Gdiplus::GraphicsPath path;
    path.AddString(pwszBuffer, -1, &fontFamily, Gdiplus::FontStyleRegular, size, PointF(outX, outY), Gdiplus::StringFormat::GenericTypographic());
    Pen pen(Color(m_strokeColorR, m_strokeColorG, m_strokeColorB), m_lineWidth);
    //SolidBrush brush(Color(255, 255, 255, 255)); // 白色填充
    m_gdiGraphics->DrawPath(&pen, &path); // 绘制描边
    //graphics.FillPath(&brush, &path); // 填充内部
}
TextMetrics CanvasRenderingContext2DWin::measureTextUtf16(wchar_t *pwszBuffer, int bufferLen)
{
    TextMetrics metrics;

    Gdiplus::GraphicsPath graphicsPathObj;
    Gdiplus::FontFamily fontFamily;
    m_font->GetFamily(&fontFamily);

    graphicsPathObj.AddString(pwszBuffer, bufferLen /* -1 */, &fontFamily, m_font->GetStyle(), m_font->GetSize(),
                              Gdiplus::PointF(0, 0), &m_stringFormat);
    Gdiplus::RectF rcBound;
    graphicsPathObj.GetBounds(&rcBound);

    Gdiplus::RectF layoutRect(0, 0, m_width, m_height);
    //m_gdiGraphics->MeasureString(pwszBuffer, bufferLen, m_font, layoutRect, &m_stringFormat, &rcBound);
    //stringFormat必须用StringFormat::GenericTypographic(), 否则偏大
    m_gdiGraphics->MeasureString(pwszBuffer, bufferLen, m_font, layoutRect, Gdiplus::StringFormat::GenericTypographic(), &rcBound);
    metrics.m_width = rcBound.Width;
    metrics.m_height = rcBound.Height;
    // UINT16 desent = fontFamily.GetCellDescent(m_fontStyle);
    // UINT16 descentPixel = m_font->GetSize() * desent / fontFamily.GetEmHeight(m_fontStyle);
    UINT16 ascender = fontFamily.GetCellAscent(m_fontStyle);
    UINT16 ascenderPixel = m_font->GetSize() * ascender / fontFamily.GetEmHeight(m_fontStyle);
    metrics.m_ascender = ascenderPixel;
    metrics.m_descender = fontFamily.GetCellDescent(m_fontStyle);
    // LOGI("measureText %f %f", rcBound.Width, rcBound.Height);
    return metrics;
}
TextMetrics CanvasRenderingContext2DWin::measureText(const std::string &text)
{
    int bufferLen = 0;
    wchar_t *pwszBuffer = utf8ToUtf16(text, &bufferLen);
    return measureTextUtf16(pwszBuffer, bufferLen);
}
void CanvasRenderingContext2DWin::clearRect(double x, double y, double width, double height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    
    m_gdiGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));
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
        ImageData data;
        data.m_width = clampedW;
        data.m_height = clampedH;
        data.m_data.resize(clampedW * clampedH * 4);
        unsigned char *glImageData = &data.m_data[0];
        //ZeroMemory(glImageData, clampedW * clampedH * 4);

        Gdiplus::Rect rect(clampedX, clampedY, clampedW, clampedH);

        Status status;
        Gdiplus::BitmapData  lockedbmp;
        status = m_gdiBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB,&lockedbmp);
        if (status != Ok) {
            //TODO
        }

        byte* pixels = static_cast<byte*>(lockedbmp.Scan0);
        UINT rowBytes = lockedbmp.Stride; // 扫描线宽度，可能会包含填充字节

        for (int y = 0; y < rect.Height; ++y) {
            byte* row = pixels + (y * rowBytes);
            for (int x = 0; x < rect.Width; ++x) {
                // Pixels stored in BGRA order
                BYTE* pixel = row + (x * 4);
                float alpha = pixel[3] / 255.0;

                glImageData[(x + y * clampedW) * 4 + 0]/*r*/ = pixel[2]/*r*/ * alpha;
                glImageData[(x + y * clampedW) * 4 + 1]/*g*/ = pixel[1] * alpha;
                glImageData[(x + y * clampedW) * 4 + 2]/*b*/ = pixel[0] * alpha;
                glImageData[(x + y * clampedW) * 4 + 3] = pixel[3]; // a

                //if (pixel[3] > 0 || pixel[2] > 0 || pixel[1] > 0 || pixel[0] > 0)
                //    printf("+");
                //else 
                //    printf("-");
            }
            //printf("\n");
        }

        //for (auto y = 0; y < clampedH; y++)
        //{
        //    for (auto x = 0; x < clampedW; x++)
        //    {
        //        Gdiplus::Color color;
        //        m_gdiBitmap->GetPixel(x, y, &color); // m_gdiBitmap->GetPixel(x, clampedH - y - 1, &color);
        //        BYTE a = color.GetA();
        //        if (a < 255 && a>0) {
        //            int a = 0;
        //        }
        //        glImageData[(x + y * clampedW) * 4 + 0] = color.GetA();
        //        glImageData[(x + y * clampedW) * 4 + 1] = color.GetA();
        //        glImageData[(x + y * clampedW) * 4 + 2] = color.GetA();
        //        glImageData[(x + y * clampedW) * 4 + 3] = color.GetA();
        //    }
        //}
        m_gdiBitmap->UnlockBits(&lockedbmp);

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
/*void CanvasRenderingContext2DWin::setTextAlign(const char* textAlign)
{
    if (strcmp(textAlign, "left") == 0)
    {
        m_textAlign = TextAlign::Left;
        m_stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);       // 水平
        m_stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直
    }
    else if (strcmp(textAlign, "right") == 0)
    {
        m_textAlign = TextAlign::Right;
        m_stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);        // 水平
        m_stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直
    }
    else if (strcmp(textAlign, "center") == 0)
    {
        m_textAlign = TextAlign::Center;
        m_stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);     // 水平
        m_stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直
    }
    else
    {
        LOGE("textAlign invalid");
    }
}
void CanvasRenderingContext2DWin::setTextBaseline(const char *textBaseline)
{
    CanvasRenderingContext2D::setTextBaseline(textBaseline);
    if (m_textBaseline == TextBaseline::Alphabetic)
    {
    }
    else if (m_textBaseline == TextBaseline::Top)
    {

    }
    else if (m_textBaseline == TextBaseline::Hanging)
    {

    }
    else if (m_textBaseline == TextBaseline::Middle)
    {

    }
    else if (m_textBaseline == TextBaseline::Ideographic)
    {

    }
    else if (m_textBaseline == TextBaseline::Bottom)
    {

    }
    else
    {
        LOGE("textBaseline invalid");
    }
}*/
void CanvasRenderingContext2DWin::getTextPosition(const std::string &text, double x, double y, double &outX,
                                                  double &outY)
{
    TextMetrics textMetrics = measureText(text);
    outX = x;
    outY = y;
    if (m_textAlign == TextAlign::Center)
    {
        outX = x - textMetrics.m_width / 2.0f;
    }
    else if (m_textAlign == TextAlign::Left)
    {
    }
    else if (m_textAlign == TextAlign::Right)
    {
        outX = x - textMetrics.m_width;
    }

    if (m_textBaseline == TextBaseline::Top)
    {
    }
    else if (m_textBaseline == TextBaseline::Middle)
    {
        outY = y - textMetrics.m_height / 2.0f;
    }
    else if (m_textBaseline == TextBaseline::Bottom)
    {
        //TODO outY = y + textMetrics.m_height;
    }
    else if (m_textBaseline == TextBaseline::Alphabetic)
    {
        //TODO outY = y + textMetrics.m_ascender;
    }
}
void CanvasRenderingContext2DWin::setFont(const char *font)
{
    if (strcmp(font, getFont())==0)
        return;
    CanvasRenderingContext2D::setFont(font);
    bool isBold = m_fontDescription.isBold();
    bool isItalic = m_fontDescription.isItalic();

    if (isBold && isItalic)
    {
        m_fontStyle = Gdiplus::FontStyle::FontStyleBoldItalic;
    }
    else if (isBold)
    {
        m_fontStyle = Gdiplus::FontStyle::FontStyleBold;
    }
    else if (isItalic)
    {
        m_fontStyle = Gdiplus::FontStyle::FontStyleItalic;
    }
    else
    {
        m_fontStyle = Gdiplus::FontStyle::FontStyleRegular;
    }
    int bufferLen = 0;
    wchar_t *pwszBuffer = utf8ToUtf16(m_fontDescription.m_family, &bufferLen);
    Gdiplus::FontFamily fontfamily(pwszBuffer);
    if (m_font != nullptr)
    {
        delete m_font;
    }
    m_font = new Gdiplus::Font(&fontfamily, m_fontDescription.m_size, m_fontStyle, Gdiplus::UnitPixel);
    // LOGI("setFont %s %f", font, m_fontDescription.m_size);
}
bool CanvasRenderingContext2DWin::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    return true;
}
} // namespace laya
