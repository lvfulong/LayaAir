#include "CanvasRenderingContext2DWin.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <regex>
#include <utils/JCCrypto.h>
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include <gdiplusenums.h>
#include <gdiplusgraphics.h>
#include <map>
#include "FontDescription.h"

extern HWND g_hWnd;
extern std::string gRedistPath;

using namespace Gdiplus;
std::map<std::wstring, Gdiplus::FontFamily*> privateFontMap;

namespace laya
{
void CanvasRenderingContext2DWin::init()
{
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiStartupInput;
    Gdiplus::Status gdiStatus = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiStartupInput, NULL);
    if (Gdiplus::Status::Ok != gdiStatus)
        return;
}
CanvasRenderingContext2DWin::CanvasRenderingContext2DWin(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
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
    // m_stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);       // 水平
    m_stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear); // 垂直
    setDefault();
}
CanvasRenderingContext2DWin::~CanvasRenderingContext2DWin()
{
    if (m_pLastFontFamily) {
        delete m_pLastFontFamily;
    }
}
void CanvasRenderingContext2DWin::setLineWidth(double lineWidth)
{
    m_lineWidth = lineWidth;
}
void CanvasRenderingContext2DWin::setLineJoin(const char *lineJoin)
{
}

void CanvasRenderingContext2DWin::fillText(const std::string &text, double x, double y, std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    int bufferLen = 0;
    std::wstring strWide = utf8ToWide(text);
    TextMetrics matrics = measureTextUtf16(strWide.data(), strWide.size());
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    //这个由于windows自己的排版导致左边空隙过大
    //m_gdiGraphics->DrawString(
    //    pwszBuffer, bufferLen, m_font, Gdiplus::PointF(outX, outY), &m_stringFormat,
    //    &Gdiplus::SolidBrush(Gdiplus::Color(m_fillColorA, m_fillColorR, m_fillColorG, m_fillColorB)));

    m_gdiGraphics->DrawString(
        strWide.data(), strWide.size(), m_font, Gdiplus::PointF(outX, outY), Gdiplus::StringFormat::GenericTypographic(),
        &Gdiplus::SolidBrush(Gdiplus::Color(m_fillColorA, m_fillColorR, m_fillColorG, m_fillColorB)));

}

void CanvasRenderingContext2DWin::strokeText(const std::string &text, double x, double y,
                                             std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }

    std::wstring strWide = utf8ToWide(text);
    TextMetrics matrics = measureTextUtf16(strWide.data(), strWide.size());
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    //m_gdiGraphics->DrawString(pwszBuffer, bufferLen, m_font, Gdiplus::PointF(outX, outY), &m_stringFormat,
    //                          &Gdiplus::SolidBrush(Gdiplus::SolidBrush(
    //                              Gdiplus::Color(m_strokeColorA, m_strokeColorR, m_strokeColorG, m_strokeColorB))));

    Gdiplus::FontFamily* pCurFamily = m_pCustomFamily;
    if (!pCurFamily) {
        pCurFamily = new Gdiplus::FontFamily();
        m_font->GetFamily(pCurFamily);
    }

    int size = m_font->GetSize();

    Gdiplus::GraphicsPath path;
    path.AddString(strWide.data(), -1, pCurFamily, m_font->GetStyle(), size, PointF(outX, outY), Gdiplus::StringFormat::GenericTypographic());
    Pen pen(Color(m_strokeColorR, m_strokeColorG, m_strokeColorB), m_lineWidth);
    //SolidBrush brush(Color(255, 255, 255, 255)); // 白色填充
    m_gdiGraphics->DrawPath(&pen, &path); // 绘制描边
    //graphics.FillPath(&brush, &path); // 填充内部
}
TextMetrics CanvasRenderingContext2DWin::measureTextUtf16(wchar_t *pwszBuffer, int bufferLen)
{
    TextMetrics metrics;

    Gdiplus::GraphicsPath graphicsPathObj;
    Gdiplus::FontFamily* pCurFamily = m_pCustomFamily;
    if (!pCurFamily) {
        pCurFamily = new Gdiplus::FontFamily();
        m_font->GetFamily(pCurFamily);
    }
    //fontFamily.Clone();

    graphicsPathObj.AddString(pwszBuffer, bufferLen /* -1 */, pCurFamily, m_font->GetStyle(), m_font->GetSize(),
                              Gdiplus::PointF(0, 0), &m_stringFormat);
    Gdiplus::RectF rcBound;
    graphicsPathObj.GetBounds(&rcBound);

    // 测量不能用layout来限制，设置一个很大的范围避免超过（希望实际的measure不要依赖实际画布大小）
    Gdiplus::RectF layoutRect(0, 0, 100000, 100000);
    // m_gdiGraphics->MeasureString(pwszBuffer, bufferLen, m_font, layoutRect, &m_stringFormat, &rcBound);
    // stringFormat必须用StringFormat::GenericTypographic(), 否则偏大
    m_gdiGraphics->MeasureString(pwszBuffer, bufferLen, m_font, layoutRect, Gdiplus::StringFormat::GenericTypographic(),
                                 &rcBound);
    metrics.m_width = rcBound.Width;
    metrics.m_height = rcBound.Height;
    // UINT16 desent = fontFamily.GetCellDescent(m_fontStyle);
    // UINT16 descentPixel = m_font->GetSize() * desent / fontFamily.GetEmHeight(m_fontStyle);
    UINT16 ascender = pCurFamily->GetCellAscent(m_fontStyle);
    UINT16 ascenderPixel = m_font->GetSize() * ascender / pCurFamily->GetEmHeight(m_fontStyle);
    metrics.m_ascender = ascenderPixel;
    metrics.m_descender = pCurFamily->GetCellDescent(m_fontStyle);
    // LOGI("measureText %f %f", rcBound.Width, rcBound.Height);
    if (pCurFamily && pCurFamily != m_pCustomFamily) {
        delete pCurFamily;
    }
    return metrics;
}
TextMetrics CanvasRenderingContext2DWin::measureText(const std::string &text)
{
    std::wstring strWide = utf8ToWide(text);
    return measureTextUtf16(strWide.data(), strWide.size());
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
    int clampedW = std::clamp(width, 0.0, static_cast<double>(m_width) - x);
    int clampedH = std::clamp(height, 0.0, static_cast<double>(m_height) - y);

    if (clampedW > 0 && clampedH > 0)
    {
        ImageData data;
        data.m_width = clampedW;
        data.m_height = clampedH;
        data.m_data.resize(clampedW * clampedH * 4);
        unsigned char *glImageData = &data.m_data[0];
        // ZeroMemory(glImageData, clampedW * clampedH * 4);

        Gdiplus::Rect rect(clampedX, clampedY, clampedW, clampedH);

        Status status;
        Gdiplus::BitmapData lockedbmp;
        status = m_gdiBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &lockedbmp);
        if (status != Ok)
        {
            LOGE("CanvasRenderingContext2DWin::getImageData GDI+ error %d", static_cast<int>(status));
            return ImageData();
        }

        byte *pixels = static_cast<byte *>(lockedbmp.Scan0);
        UINT rowBytes = lockedbmp.Stride; // 扫描线宽度，可能会包含填充字节

        for (int y = 0; y < rect.Height; ++y)
        {
            byte *row = pixels + (y * rowBytes);
            for (int x = 0; x < rect.Width; ++x)
            {
                // Pixels stored in BGRA order
                BYTE *pixel = row + (x * 4);
                float alpha = pixel[3] / 255.0;

                glImageData[(x + y * clampedW) * 4 + 0] /*r*/ = pixel[2] /*r*/;
                glImageData[(x + y * clampedW) * 4 + 1] /*g*/ = pixel[1];
                glImageData[(x + y * clampedW) * 4 + 2] /*b*/ = pixel[0];
                glImageData[(x + y * clampedW) * 4 + 3] = pixel[3]; // a

            }
        }
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
    // Gdiplus::Matrix m(a,  b,  c,  d,  e,  f);
    // m_gdiGraphics->SetTransform(&m);
}
void CanvasRenderingContext2DWin::scale(double x, double y)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    // m_gdiGraphics->ScaleTransform(x, y);
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
        // TODO outY = y + textMetrics.m_height;
    }
    else if (m_textBaseline == TextBaseline::Alphabetic)
    {
        // TODO outY = y + textMetrics.m_ascender;
    }
}
void CanvasRenderingContext2DWin::setFont(const char *font)
{
    if (strcmp(font, getFont()) == 0)
        return;
    CanvasRenderingContext2D::setFont(font);
    bool isBold = m_fontDescription->isBold();
    bool isItalic = m_fontDescription->isItalic();

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

    if (m_font != nullptr){
        delete m_font;
    }

    std::wstring strWide = utf8ToWide(m_fontDescription->m_family);
    const Gdiplus::FontFamily* pFontFamily = nullptr;
    auto it = privateFontMap.find(strWide);
    if (it == privateFontMap.end()) {
        pFontFamily = new  Gdiplus::FontFamily(strWide.data());
        if (pFontFamily->GetLastStatus() != Gdiplus::Ok) {
            delete pFontFamily;
            //回退到通用无衬线字体
            pFontFamily = nullptr;
        }
        if (m_pLastFontFamily) {
            delete m_pLastFontFamily;
        }
        m_pLastFontFamily = pFontFamily;
        if(!pFontFamily){
            pFontFamily = Gdiplus::FontFamily::GenericSansSerif();
        }
        m_font = new Gdiplus::Font(pFontFamily, m_fontDescription->m_size, m_fontStyle, Gdiplus::UnitPixel);
        m_pCustomFamily = nullptr;
    }
    else {
        //family要clone，后面会删掉m_font，会顺便把family也删掉
        m_pCustomFamily = it->second->Clone();
        m_font = new Gdiplus::Font(m_pCustomFamily, m_fontDescription->m_size, m_fontStyle, Gdiplus::UnitPixel);
        //这里是测试GetFamily的，下面的会导致异常，不知道为什么
        //for( int i=0; i<10; i++)
        //{
        //    Gdiplus::FontFamily fontFamily;
        //    m_font->GetFamily(&fontFamily);
        //}

    }
}

bool CanvasRenderingContext2DWin::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    bool isAbsPath = (path[0]=='/' || path[1]==':');
    std::wstring nameW = utf8ToWide(fontName.c_str());
	// 创建一个PrivateFontCollection对象 
    std::wstring strWide = utf8ToWide(isAbsPath?path.c_str():(gRedistPath+path).c_str());
    auto coll = new PrivateFontCollection();
	coll->AddFontFile(strWide.data());

    Gdiplus::FontFamily fontFamilies[1];
    int findNum = 0;
    Gdiplus::Status status = coll->GetFamilies(1, fontFamilies, &findNum); // 从字体集中获取字体家族
    if (status == Gdiplus::Ok) {
        privateFontMap[nameW] = fontFamilies[0].Clone();
    }

    return true;
}

std::vector<char*> CanvasRenderingContext2DWin::fontBuffers;
bool CanvasRenderingContext2DWin::registerFontFromBuffer(const std::string& fontName, const uint8_t* buff, int len) {
    std::wstring nameW = utf8ToWide(fontName.c_str());
    auto coll = new PrivateFontCollection();    //先用new，防止被释放，释放了的话family就都无效了
    // AddMemoryFont 需要引用这个内存，所以new一个
    char *pmem = new char[len];
    memcpy(pmem, buff, len);
    coll->AddMemoryFont(pmem,len);

     Gdiplus::FontFamily fontFamilies[1];
     int findNum = 0;
     Gdiplus::Status status = coll->GetFamilies(1, fontFamilies, &findNum); // 从字体集中获取字体家族
     if (status == Gdiplus::Ok) {
         privateFontMap[nameW] = fontFamilies[0].Clone();
     }

    //privateFontMap[nameW] = coll;
    CanvasRenderingContext2DWin::fontBuffers.push_back(pmem);
    return true;
}

void CanvasRenderingContext2DWin::clearAllBuffer() {
    auto& all = CanvasRenderingContext2DWin::fontBuffers;
    for (auto i = all.begin(); i != all.end(); i++) {
        delete [] *i;
    }
    all.clear();
}

} // namespace laya
