#include "JSCanvasRenderingContext2D.h"
#include <binder/JSInterface.h>
#include "2D/CanvasRenderingContext2D.h"
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>

#include "../../JCConch.h"
#include "../../JCSystemConfig.h"

namespace laya
{

JSCanvasRenderingContext2D::JSCanvasRenderingContext2D(int width, int height)
{
    m_context = CanvasRenderingContext2D::create(width, height);
    AdjustAmountOfExternalAllocatedMemory(4);
    JCMemorySurvey::GetInstance()->newClass("conchCanvasRenderingContext2D", 4, this);
}
JSCanvasRenderingContext2D::JSCanvasRenderingContext2D()
{
    AdjustAmountOfExternalAllocatedMemory(4);
    JCMemorySurvey::GetInstance()->newClass("conchCanvasRenderingContext2D", 4, this);
}
JSCanvasRenderingContext2D::~JSCanvasRenderingContext2D()
{
    JCMemorySurvey::GetInstance()->releaseClass("conchCanvasRenderingContext2D", this);
}

void JSCanvasRenderingContext2D::fillText(const char *text, double x, double y, double maxWidth)
{
    m_context->fillText(text, x, y, maxWidth);
}

void JSCanvasRenderingContext2D::strokeText(const char *text, double x, double y, double maxWidth)
{
    m_context->strokeText(text, x, y, maxWidth);
}
JsValue JSCanvasRenderingContext2D::measureText(const std::string &text)
{
    TextMetrics metrics = m_context->measureText(text);
    TextMetrics *copy = new TextMetrics(metrics);
    return Converter<TextMetrics *>::ToJs(copy);
}
int JSCanvasRenderingContext2D::getID()
{
    return m_context->getID();
}
void JSCanvasRenderingContext2D::clearRect(double x, double y, double width, double height)
{
    m_context->clearRect(x, y, width, height);
}
void JSCanvasRenderingContext2D::save()
{
    m_context->save();
}
void JSCanvasRenderingContext2D::restore()
{
    m_context->restore();
}
JsValue JSCanvasRenderingContext2D::getImageData(double x, double y, double width, double height)
{
    ImageData data = m_context->getImageData(x, y, width, height);
    ImageData *copy = new ImageData(std::move(data));
    return Converter<ImageData *>::ToJs(copy);
}
void JSCanvasRenderingContext2D::setTransform(double a, double b, double c, double d, double e, double f)
{
    m_context->setTransform(a, b, c, d, e, f);
}
void JSCanvasRenderingContext2D::scale(double x, double y)
{
    m_context->scale(x, y);
}
const char *JSCanvasRenderingContext2D::getFont()
{
    return m_context->getFont();
}
void JSCanvasRenderingContext2D::setFont(const char *font)
{
    m_context->setFont(font);
}
void JSCanvasRenderingContext2D::setTextAlign(const char *textAlign)
{
    m_context->setTextAlign(textAlign);
}
const char *JSCanvasRenderingContext2D::getTextAlign()
{
    return m_context->getTextAlign();
}
void JSCanvasRenderingContext2D::setTextBaseline(const char *textBaseline)
{
    m_context->setTextBaseline(textBaseline);
}
const char *JSCanvasRenderingContext2D::getTextBaseline()
{
    return m_context->getTextBaseline();
}
void JSCanvasRenderingContext2D::setFillStyle(const char *color)
{
    m_context->setFillStyle(color);
}
const char *JSCanvasRenderingContext2D::getFillStyle()
{
    return m_context->getFillStyle();
}

void JSCanvasRenderingContext2D::setStrokeStyle(const char *color)
{
    m_context->setStrokeStyle(color);
}
const char *JSCanvasRenderingContext2D::getStrokeStyle()
{
    return m_context->getStrokeStyle();
}
void JSCanvasRenderingContext2D::setLineWidth(double lineWidth)
{
    m_context->setLineWidth(lineWidth);
}
double JSCanvasRenderingContext2D::getLineWidth()
{
    return m_context->getLineWidth();
}
void JSCanvasRenderingContext2D::setLineJoin(const char *lineJoin)
{
    m_context->setLineJoin(lineJoin);
}
const char *JSCanvasRenderingContext2D::getLineJoin()
{
    return m_context->getLineJoin();
}
void JSCanvasRenderingContext2D::exportJS(jsbind::Object &context)
{
    jsbind::class_<ImageData> class_binding_image_data;
    class_binding_image_data.constructor<>();
    class_binding_image_data.property_field("width", &ImageData::m_width);
    class_binding_image_data.property_field("height", &ImageData::m_height);
    class_binding_image_data.property_optional_override(
        "data", optional_override([](ImageData &imageData) {
            auto ab = jsbind::ArrayBuffer::MakeTypedArray(imageData.m_data.data(), imageData.m_data.size(), 0, jsbind::ArrayBuffer::UINT8_CLAMPED_ARRAY);
            return ab.getHandle();
        }));
    context.class_("ImageData", class_binding_image_data);

    jsbind::class_<TextMetrics> class_binding_text_metrics;
    class_binding_text_metrics.constructor<>();
    class_binding_text_metrics.property_field("width", &TextMetrics::m_width);
    context.class_("TextMetrics", class_binding_text_metrics);

    jsbind::class_<JSCanvasRenderingContext2D> class_binding;
    class_binding.constructor<>();
    class_binding.constructor<int, int>();
    class_binding.property("fillStyle", &JSCanvasRenderingContext2D::getFillStyle,
                           &JSCanvasRenderingContext2D::setFillStyle);
    class_binding.property("strokeStyle", &JSCanvasRenderingContext2D::getStrokeStyle,
                           &JSCanvasRenderingContext2D::setStrokeStyle);
    class_binding.property("textAlign", &JSCanvasRenderingContext2D::getTextAlign,
                           &JSCanvasRenderingContext2D::setTextAlign);
    class_binding.property("textBaseline", &JSCanvasRenderingContext2D::getTextBaseline,
                           &JSCanvasRenderingContext2D::setTextBaseline);
    class_binding.property("lineWidth", &JSCanvasRenderingContext2D::getLineWidth,
                           &JSCanvasRenderingContext2D::setLineWidth);
    class_binding.property("lineJoin", &JSCanvasRenderingContext2D::getLineJoin,
                           &JSCanvasRenderingContext2D::setLineJoin);
    class_binding.property("conchImgId", &JSCanvasRenderingContext2D::getID);
    class_binding.property("font", &JSCanvasRenderingContext2D::getFont, &JSCanvasRenderingContext2D::setFont);
    class_binding.function("strokeText", &JSCanvasRenderingContext2D::strokeText);
    class_binding.function("fillText", &JSCanvasRenderingContext2D::fillText);
    class_binding.function("measureText", &JSCanvasRenderingContext2D::measureText);
    class_binding.function("clearRect", &JSCanvasRenderingContext2D::clearRect);
    class_binding.function("save", &JSCanvasRenderingContext2D::save);
    class_binding.function("restore", &JSCanvasRenderingContext2D::restore);
    class_binding.function("getImageData", &JSCanvasRenderingContext2D::getImageData);
    class_binding.function("setTransform", &JSCanvasRenderingContext2D::setTransform);
    class_binding.function("scale", &JSCanvasRenderingContext2D::scale);
    context.class_("conchCanvasRenderingContext2D", class_binding);

}
} // namespace laya
