#include "CanvasRenderingContext2DOHOS.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <imageLib/JCImageRW.h>
#include <rawfile/raw_file_manager.h>
#include <regex>
#include <utils/JCCrypto.h>
#include <utils/Log.h>
extern NativeResourceManager *g_pAssetManager;
namespace laya
{
class ScopedTypography
{
  public:
    ScopedTypography(OH_Drawing_Typography *typography) : m_typegraphy(typography)
    {
    }
    ~ScopedTypography()
    {
        if (m_typegraphy)
        {
            OH_Drawing_DestroyTypography(m_typegraphy);
        }
    }
    OH_Drawing_Typography *get()
    {
        return m_typegraphy;
    }

  private:
    OH_Drawing_Typography *m_typegraphy{nullptr};
};
CanvasRenderingContext2DOHOS::CanvasRenderingContext2DOHOS(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }

    m_bitmap = OH_Drawing_BitmapCreate();
    OH_Drawing_BitmapFormat cFormat{COLOR_FORMAT_RGBA_8888, ALPHA_FORMAT_OPAQUE};
    OH_Drawing_BitmapBuild(m_bitmap, m_width, m_height, &cFormat);

    m_canvas = OH_Drawing_CanvasCreate();
    OH_Drawing_CanvasBind(m_canvas, m_bitmap);
    OH_Drawing_CanvasClear(m_canvas, OH_Drawing_ColorSetArgb(0x00, 0x00, 0x00, 0x00));

    m_fontTypography = OH_Drawing_CreateTypographyStyle();
    OH_Drawing_SetTypographyTextDirection(m_fontTypography, TEXT_DIRECTION_LTR);
    OH_Drawing_SetTypographyTextAlign(m_fontTypography, TEXT_ALIGN_LEFT);

    m_fontTextStyle = OH_Drawing_CreateTextStyle();
    // OH_Drawing_SetTextStyleBaseLine(m_fontTextStyle, TEXT_BASELINE_ALPHABETIC);
    // OH_Drawing_SetTextStyleBaseLine(m_fontTextStyle, TEXT_BASELINE_IDEOGRAPHIC);
    OH_Drawing_SetTextStyleFontHeight(m_fontTextStyle, 0);
    m_fontCollection = OH_Drawing_CreateFontCollection();

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];
    setDefault();
    m_matrix = OH_Drawing_MatrixCreate();
    m_typographyCreate = OH_Drawing_CreateTypographyHandler(m_fontTypography, m_fontCollection);
}
CanvasRenderingContext2DOHOS::~CanvasRenderingContext2DOHOS()
{
    if (m_matrix)
    {
        OH_Drawing_MatrixDestroy(m_matrix);
        m_canvas = nullptr;
    }
    if (m_canvas)
    {
        OH_Drawing_CanvasDestroy(m_canvas);
        m_canvas = nullptr;
    }

    if (m_bitmap)
    {
        OH_Drawing_BitmapDestroy(m_bitmap);
        m_bitmap = nullptr;
    }
}
std::unique_ptr<ScopedTypography> CanvasRenderingContext2DOHOS::createTypography(const std::string &text)
{
    OH_Drawing_TypographyHandlerPushTextStyle(m_typographyCreate, m_fontTextStyle);
    OH_Drawing_TypographyHandlerAddText(m_typographyCreate, text.c_str());
    OH_Drawing_TypographyHandlerPopTextStyle(m_typographyCreate);
    OH_Drawing_Typography *typography = OH_Drawing_CreateTypography(m_typographyCreate);
    OH_Drawing_TypographyLayout(typography, m_width);
    return std::make_unique<ScopedTypography>(typography);
}
void CanvasRenderingContext2DOHOS::setLineWidth(double lineWidth)
{
    m_lineWidth = lineWidth;
}
void CanvasRenderingContext2DOHOS::setLineJoin(const char *lineJoin)
{
    // todo
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

    auto typography = createTypography(text);

    OH_Drawing_TypographyPaint(typography->get(), m_canvas, outX, outY);
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
    // todo
}

TextMetrics CanvasRenderingContext2DOHOS::measureText(const std::string &text)
{
    TextMetrics metrics;
    auto typography = createTypography(text);
    metrics.m_width = OH_Drawing_TypographyGetMaxIntrinsicWidth(typography->get());
    metrics.m_height = OH_Drawing_TypographyGetHeight(typography->get());
    metrics.m_descender = OH_Drawing_TypographyGetAlphabeticBaseline(typography->get());
    return metrics;
}
void CanvasRenderingContext2DOHOS::clearRect(double x, double y, double width, double height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    OH_Drawing_CanvasClear(m_canvas, OH_Drawing_ColorSetArgb(0x00, 0x00, 0x00, 0x00));
}
void CanvasRenderingContext2DOHOS::save()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    OH_Drawing_CanvasSave(m_canvas);
}
void CanvasRenderingContext2DOHOS::restore()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    OH_Drawing_CanvasRestore(m_canvas);
}
ImageData CanvasRenderingContext2DOHOS::getImageData(double x, double y, double width, double height)
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
        uint32_t *pixel = (uint32_t *)(&data.m_data[0]);
        char *bitmapAddr = static_cast<char *>(OH_Drawing_BitmapGetPixels(m_bitmap));

        int start = 0;
        int bytePerLineDes = clampedW * 4;
        int bytePerLineSrc = m_width * 4;
        int startI = clampedH + clampedY - 1;
        for (int i = clampedY; i < clampedH + clampedY; i++)
        {
            std::memcpy(static_cast<void *>(&data.m_data[start]), &bitmapAddr[clampedX * 4 + i * bytePerLineSrc],
                        bytePerLineDes);
            start += bytePerLineDes;
        }
        return data;
    }
    else
    {
        return ImageData();
    }
}
const BitmapData &CanvasRenderingContext2DOHOS::getBitmapData() const
{
    void *bitmapAddr = OH_Drawing_BitmapGetPixels(m_bitmap);
    memcpy(m_bitmapData.m_pImageData, bitmapAddr, m_width * m_height * 4);
    return m_bitmapData;
}
void CanvasRenderingContext2DOHOS::setTransform(double a, double b, double c, double d, double e, double f)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    OH_Drawing_CanvasScale(m_canvas, a, d);
    OH_Drawing_CanvasTranslate(m_canvas, e, f);
    // OH_Drawing_MatrixSetMatrix(m_matrix, a, b, e, c, d, f, 0.0f, 0.0f, 0.0f);
}
void CanvasRenderingContext2DOHOS::scale(double x, double y)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    OH_Drawing_CanvasScale(m_canvas, x, x);
}
void CanvasRenderingContext2DOHOS::setTextAlign(const char *textAlign)
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
}
void CanvasRenderingContext2DOHOS::setFillStyle(const char *color)
{
    CanvasRenderingContext2D::setFillStyle(color);
    OH_Drawing_SetTextStyleColor(m_fontTextStyle,
                                 OH_Drawing_ColorSetArgb(m_fillColorA, m_fillColorR, m_fillColorG, m_fillColorB));
}

void CanvasRenderingContext2DOHOS::setStrokeStyle(const char *color)
{
    CanvasRenderingContext2D::setStrokeStyle(color);
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
        outY = y + textMetrics.m_height;
    }
    else if (m_textBaseline == TextBaseline::Alphabetic)
    {
        outY = y - textMetrics.m_ascender;
    }
}
void CanvasRenderingContext2DOHOS::setFont(const char *font)
{
    if (strcmp(font, getFont()) == 0)
        return;
    CanvasRenderingContext2D::setFont(font);
    bool isBold = m_fontDescription.isBold();
    bool isItalic = m_fontDescription.isItalic();
    if (!m_fontDescription.m_family.empty())
    {
        const char *fontFamilies[1];
        fontFamilies[0] = m_fontDescription.m_family.c_str();
        OH_Drawing_SetTextStyleFontFamilies(m_fontTextStyle, 1, fontFamilies);
        OH_Drawing_SetTextStyleLocale(m_fontTextStyle, "en");
    }

    if (isBold)
    {
        OH_Drawing_SetTextStyleFontWeight(m_fontTextStyle, FONT_WEIGHT_700);
    }
    else
    {
        OH_Drawing_SetTextStyleFontWeight(m_fontTextStyle, FONT_WEIGHT_400);
    }

    if (isItalic)
    {
        OH_Drawing_SetTextStyleFontStyle(m_fontTextStyle, FONT_STYLE_ITALIC);
    }
    else
    {
        OH_Drawing_SetTextStyleFontStyle(m_fontTextStyle, FONT_STYLE_NORMAL);
    }
    OH_Drawing_SetTextStyleFontSize(m_fontTextStyle, m_fontDescription.m_size);
}
bool CanvasRenderingContext2DOHOS::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    RawFile *rawFile = OH_ResourceManager_OpenRawFile(g_pAssetManager, path.c_str());
    if (rawFile != nullptr)
    {
        LOGE("OH_ResourceManager_OpenRawFile success");
    }
    size_t len = OH_ResourceManager_GetRawFileSize(rawFile);
    std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(len);
    uint8_t *outdata = data.get();
    int res = OH_ResourceManager_ReadRawFile(rawFile, outdata, len);

    // OH_Drawing_RegisterFontBuffer(m_fontCollection, fontName.c_str(), outdata, len);
    OH_ResourceManager_CloseRawFile(rawFile);
    return true;
}
} // namespace laya
