#include "CanvasRenderingContext2DOHOS.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <regex>
#include <utils/JCCrypto.h>
#include <utils/Log.h>
#include <rawfile/raw_file_manager.h>

extern NativeResourceManager *g_pAssetManager;
namespace laya
{
static int s_count = 0;
OH_Drawing_FontCollection *CanvasRenderingContext2DOHOS::m_fontCollection = nullptr;
CanvasRenderingContext2DOHOS::CanvasRenderingContext2DOHOS(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    if (s_count == 0)
    {

    }
    s_count++;

    m_canvas = OH_Drawing_CanvasCreate();
    m_bitmap = OH_Drawing_BitmapCreate();
    OH_Drawing_BitmapFormat cFormat{COLOR_FORMAT_RGBA_8888, ALPHA_FORMAT_OPAQUE}; 
    OH_Drawing_BitmapBuild(m_bitmap, m_width, m_height, &cFormat);

    OH_Drawing_CanvasBind(m_canvas, m_bitmap);
    OH_Drawing_CanvasClear(m_canvas, OH_Drawing_ColorSetArgb(0xFF, 0xFF, 0xFF, 0xFF));

    m_fontTypography = OH_Drawing_CreateTypographyStyle();
    m_fontTextStyle = OH_Drawing_CreateTextStyle();
    if (CanvasRenderingContext2DOHOS::m_fontCollection == nullptr)
    {
        CanvasRenderingContext2DOHOS::m_fontCollection = OH_Drawing_CreateFontCollection();
    }

    OH_Drawing_SetTypographyTextAlign(m_fontTypography, TEXT_ALIGN_LEFT);
    OH_Drawing_SetTextStyleBaseLine(m_fontTextStyle, TEXT_BASELINE_ALPHABETIC);

    //m_typographyCreate = OH_Drawing_CreateTypographyHandler(m_fontTypography, CanvasRenderingContext2DOHOS::m_fontCollection);

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];
    //m_bitmapData.m_pImageData.length
    // Gdiplus::Matrix matrix;
    // matrix.Translate(0.0f, height);
    // matrix.Scale(1.0f, -1.0f);
    // m_gdiGraphics->SetTransform(&matrix);
    //m_stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);       // 水平
    //m_stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear); // 垂直
    setDefault();
}
CanvasRenderingContext2DOHOS::~CanvasRenderingContext2DOHOS()
{
}
void CanvasRenderingContext2DOHOS::setLineWidth(double lineWidth)
{
    m_lineWidth = lineWidth;
}
void CanvasRenderingContext2DOHOS::setLineJoin(const char *lineJoin)
{
}
void CanvasRenderingContext2DOHOS::fillText(const std::string &text, double x, double y, std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    //这个由于windows自己的排版导致左边空隙过大
    //m_gdiGraphics->DrawString(
    //    pwszBuffer, bufferLen, m_font, Gdiplus::PointF(outX, outY), &m_stringFormat,
    //    &Gdiplus::SolidBrush(Gdiplus::Color(m_fillColorA, m_fillColorR, m_fillColorG, m_fillColorB)));
    //OH_Drawing_Typography *typography = realDraw(text.c_str());
    //OH_Drawing_TypographyPaint(typography, m_canvas, outX, outY);
    // m_gdiGraphics->DrawString(
    //     pwszBuffer, bufferLen, m_font, Gdiplus::PointF(outX, outY), Gdiplus::StringFormat::GenericTypographic(),
    //     &Gdiplus::SolidBrush(Gdiplus::Color(m_fillColorA, m_fillColorR, m_fillColorG, m_fillColorB)));

}

void CanvasRenderingContext2DOHOS::strokeText(const std::string &text, double x, double y,
                                             std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    //m_gdiGraphics->DrawString(pwszBuffer, bufferLen, m_font, Gdiplus::PointF(outX, outY), &m_stringFormat,
    //                          &Gdiplus::SolidBrush(Gdiplus::SolidBrush(
    //                              Gdiplus::Color(m_strokeColorA, m_strokeColorR, m_strokeColorG, m_strokeColorB))));

    // FontFamily fontFamily;
    // m_font->GetFamily(&fontFamily);
    
    // int size = m_font->GetSize();

    // Gdiplus::GraphicsPath path;
    // path.AddString(pwszBuffer, -1, &fontFamily, Gdiplus::FontStyleRegular, size, PointF(outX, outY), Gdiplus::StringFormat::GenericTypographic());
    // Pen pen(Color(m_strokeColorR, m_strokeColorG, m_strokeColorB), m_lineWidth);
    // //SolidBrush brush(Color(255, 255, 255, 255)); // 白色填充
    // m_gdiGraphics->DrawPath(&pen, &path); // 绘制描边
    //graphics.FillPath(&brush, &path); // 填充内部
}

TextMetrics CanvasRenderingContext2DOHOS::measureText(const std::string &text)
{
    TextMetrics metrics;
    //OH_Drawing_Typography *typography = realDraw(text.c_str());
    metrics.m_width = 0;//OH_Drawing_TypographyGetMaxIntrinsicWidth(typography);
    metrics.m_height = 0;//OH_Drawing_TypographyGetHeight(typography);
    return metrics;
}
void CanvasRenderingContext2DOHOS::clearRect(double x, double y, double width, double height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    
    //m_gdiGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));
}
void CanvasRenderingContext2DOHOS::save()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DOHOS::restore()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}

ImageData CanvasRenderingContext2DOHOS::getImageData(double x, double y, double width, double height)
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
        //todo
        return data;
    }
    else
    {
        LOGE("");
        return ImageData();
    }
}
const BitmapData &CanvasRenderingContext2DOHOS::getBitmapData() const
{
    /*void *bitmapAddr = OH_Drawing_BitmapGetPixels(m_bitmap);
    memcpy(m_bitmapData.m_pImageData, bitmapAddr, _bufferSize);
    return m_bitmapData;*/

    return m_bitmapData;
    // return getImageData(0, 0, m_width, m_height);
}
void CanvasRenderingContext2DOHOS::setTransform(double a, double b, double c, double d, double e, double f)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DOHOS::scale(double x, double y)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DOHOS::setTextAlign(const char* textAlign)
{
    if (strcmp(textAlign, "left") == 0)
    {
        m_textAlign = TextAlign::Left;
    }
    else if (strcmp(textAlign, "right") == 0)
    {
        m_textAlign = TextAlign::Right;
    }
    else if (strcmp(textAlign, "center") == 0)
    {
        m_textAlign = TextAlign::Center;
    }
    else
    {
        LOGE("textAlign invalid");
    }
}
void CanvasRenderingContext2DOHOS::setTextBaseline(const char *textBaseline)
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
}

void CanvasRenderingContext2DOHOS::getTextPosition(const std::string &text, double x, double y, double &outX,
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
void CanvasRenderingContext2DOHOS::setFont(const char *font)
{
    if (strcmp(font, getFont())==0)
        return;
    CanvasRenderingContext2D::setFont(font);
    bool isBold = m_fontDescription.isBold();
    bool isItalic = m_fontDescription.isItalic();
    
    if(isBold){
        OH_Drawing_SetTextStyleFontWeight(m_fontTextStyle, FONT_WEIGHT_700);
    }
    else{
        OH_Drawing_SetTextStyleFontWeight(m_fontTextStyle, FONT_WEIGHT_400);
    }

    if(isItalic){
        OH_Drawing_SetTextStyleFontStyle(m_fontTextStyle,FONT_STYLE_ITALIC);
    }
    else{
        OH_Drawing_SetTextStyleFontStyle(m_fontTextStyle,FONT_STYLE_NORMAL);
    }
    if(!m_fontDescription.m_family.empty()){
         const char* fontFamilies[1];
        fontFamilies[0] = m_fontDescription.m_family.c_str();
        OH_Drawing_SetTextStyleFontFamilies(m_fontTextStyle, 1, fontFamilies);
        OH_Drawing_SetTextStyleLocale(m_fontTextStyle, "en");
    }
    OH_Drawing_SetTextStyleFontSize(m_fontTextStyle,  m_fontDescription.m_size);
    // LOGI("setFont %s %f", font, m_fontDescription.m_size);
}
bool CanvasRenderingContext2DOHOS::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    //todo
    /*// 获取rawfile指针对象
    RawFile *rawFile = OH_ResourceManager_OpenRawFile(g_pAssetManager, path.c_str());
    if (rawFile != nullptr) {
        //OH_LOG_Print(LOG_APP, LOG_ERROR, GLOBAL_RESMGR, tag, "OH_ResourceManager_OpenRawFile success");
    }
    // 获取rawfile大小并申请内存
    size_t  len = OH_ResourceManager_GetRawFileSize(rawFile);
    std::unique_ptr<uint8_t[]> data= std::make_unique<uint8_t[]>(len);
    uint8_t *outdata =data.get();
    // 一次性读取rawfile全部内容
    int res = OH_ResourceManager_ReadRawFile(rawFile, outdata, len);

    OH_Drawing_RegisterFontBuffer(CanvasRenderingContext2DOHOS::m_fontCollection, fontName, outdata, len);
    // 关闭打开的指针对象
    OH_ResourceManager_CloseRawFile(rawFile);*/
    return true;
}


 OH_Drawing_Typography * CanvasRenderingContext2DOHOS::realDraw(const char *text) {
    OH_Drawing_TypographyHandlerPushTextStyle(m_typographyCreate, m_fontTextStyle);
    OH_Drawing_TypographyHandlerAddText(m_typographyCreate, text);
    OH_Drawing_TypographyHandlerPopTextStyle(m_typographyCreate);
    OH_Drawing_Typography *typography = OH_Drawing_CreateTypography(m_typographyCreate);
    OH_Drawing_TypographyLayout(typography, m_width);
    return typography;
}
} // namespace laya
