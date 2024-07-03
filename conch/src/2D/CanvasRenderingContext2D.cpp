#include "CanvasRenderingContext2D.h"
#ifdef __ANDROID__
#include "CanvasRenderingContext2DAndroid.h"
#elif WIN32
#include "CanvasRenderingContext2DWin.h"
#elif __APPLE__
#include "CanvasRenderingContext2DCG.h"
#elif OHOS
#include "CanvasRenderingContext2DOHOS.h"
#elif __LINUX__
#include "CanvasRenderingContext2DLinux.h"
#endif
#include <utils/ColorParser.h>
#include "JCConch.h"

namespace laya
{
static int32_t s_id = 0;
static std::unordered_map<int32_t, CanvasRenderingContext2D *> s_canvasRenderingContext2DMap;
CanvasRenderingContext2D *CanvasRenderingContext2D::create(int width, int height)
{
#ifdef __ANDROID__
    return new CanvasRenderingContext2DAndroid(width, height);
#elif WIN32
    return new CanvasRenderingContext2DWin(width, height);
#elif __APPLE__
    return new CanvasRenderingContext2DCG(width, height);
#elif __LINUX__
    return new CanvasRenderingContext2DLinux(width, height);
#elif OHOS
    return new CanvasRenderingContext2DOHOS(width, height);
#endif
}
CanvasRenderingContext2D *CanvasRenderingContext2D::get(int id)
{
    std::unordered_map<int32_t, CanvasRenderingContext2D *>::iterator it = s_canvasRenderingContext2DMap.find(id);
    if (it != s_canvasRenderingContext2DMap.end())
    {
        return it->second;
    }
    return nullptr;
}
CanvasRenderingContext2D::CanvasRenderingContext2D(int width, int height) : m_width(width), m_height(height)
{
    this->m_id = s_id;
    s_id++;
    int32_t id = this->m_id;
    JCConch::s_pConchRender->postTaskFromJSToRenderAsync(
        [id, this]() { s_canvasRenderingContext2DMap.insert(std::make_pair(id, this)); });
}
CanvasRenderingContext2D::~CanvasRenderingContext2D()
{
    int32_t id = this->m_id;
    JCConch::s_pConchRender->postTaskFromJSToRenderAsync([id]() { s_canvasRenderingContext2DMap.erase(id); });
}
void CanvasRenderingContext2D::setDefault()
{
    // setFont("10px sans-serif");
    setFont("10px Arial");
    setTextAlign("left");
    setTextBaseline("alphabetic");
    setFillStyle("#000000");
    setStrokeStyle("#000000");
    setLineWidth(1.0);
    setLineJoin("miter");
}
void CanvasRenderingContext2D::setTextAlign(const char *textAlign)
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
const char *CanvasRenderingContext2D::getTextAlign()
{
    if (m_textAlign == TextAlign::Left)
    {
        return "left";
    }
    else if (m_textAlign == TextAlign::Right)
    {
        return "right";
    }
    else if (m_textAlign == TextAlign::Center)
    {
        return "center";
    }
    return "left";
}
void CanvasRenderingContext2D::setFillStyle(const char *color)
{
    m_fillStyle = color;
    parseRGBAFromString(m_fillStyle.c_str(), m_fillColorR, m_fillColorG, m_fillColorB, m_fillColorA);
}
const char *CanvasRenderingContext2D::getFillStyle()
{
    return m_fillStyle.c_str();
}

void CanvasRenderingContext2D::setStrokeStyle(const char *color)
{
    m_strokeStyle = color;
    parseRGBAFromString(m_strokeStyle.c_str(), m_strokeColorR, m_strokeColorG, m_strokeColorB, m_strokeColorA);
}
const char *CanvasRenderingContext2D::getStrokeStyle()
{
    return m_strokeStyle.c_str();
}
void CanvasRenderingContext2D::setTextBaseline(const char *textBaseline)
{
    if (strcmp(textBaseline, "alphabetic") == 0)
    {
        m_textBaseline = TextBaseline::Alphabetic;
    }
    else if (strcmp(textBaseline, "top") == 0)
    {
        m_textBaseline = TextBaseline::Top;
    }
    else if (strcmp(textBaseline, "hanging") == 0)
    {
        m_textBaseline = TextBaseline::Hanging;
    }
    else if (strcmp(textBaseline, "middle") == 0)
    {
        m_textBaseline = TextBaseline::Middle;
    }
    else if (strcmp(textBaseline, "ideographic") == 0)
    {
        m_textBaseline = TextBaseline::Ideographic;
    }
    else if (strcmp(textBaseline, "bottom") == 0)
    {
        m_textBaseline = TextBaseline::Bottom;
    }
    else
    {
        LOGE("textBaseline invalid");
    }
}

const char *CanvasRenderingContext2D::getTextBaseline()
{
    if (m_textBaseline == TextBaseline::Alphabetic)
    {
        return "alphabetic";
    }
    else if (m_textBaseline == TextBaseline::Top)
    {
        return "top";
    }
    else if (m_textBaseline == TextBaseline::Hanging)
    {
        return "hanging";
    }
    else if (m_textBaseline == TextBaseline::Middle)
    {
        return "middle";
    }
    else if (m_textBaseline == TextBaseline::Ideographic)
    {
        return "ideographic";
    }
    else if (m_textBaseline == TextBaseline::Bottom)
    {
        return "bottom";
    }
    return "alphabetic";
}
void CanvasRenderingContext2D::setLineJoin(const char *lineJoin)
{
    if (strcmp(lineJoin, "round") == 0)
    {
        m_lineJoin = LineJoin::Round;
    }
    else if (strcmp(lineJoin, "bevel") == 0)
    {
        m_lineJoin = LineJoin::Bevel;
    }
    else if (strcmp(lineJoin, "miter") == 0)
    {
        m_lineJoin = LineJoin::Miter;
    }
    else
    {
        LOGE("lineJoin invalid");
    }
}
const char *CanvasRenderingContext2D::getLineJoin()
{
    if (m_lineJoin == LineJoin::Round)
    {
        return "round";
    }
    else if (m_lineJoin == LineJoin::Bevel)
    {
        return "bevel";
    }
    else if (m_lineJoin == LineJoin::Miter)
    {
        return "miter";
    }
    return "miter";
}
void CanvasRenderingContext2D::getTextPosition(const std::string &text, double x, double y, double &outX, double &outY)
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
        outY = y - textMetrics.m_height;
    }
    else if (m_textBaseline == TextBaseline::Alphabetic)
    {
        outY = y - textMetrics.m_ascender;
    }
}
} // namespace laya
