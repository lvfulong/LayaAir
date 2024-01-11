#ifndef __CanvasRenderingContext2D_H__
#define __CanvasRenderingContext2D_H__
#include "FontDescription.h"
#include "FontDescriptionParser.h"
#include "ImageData.h"
#include "TextMetrics.h"
#include "imageLib/JCImageRW.h"
#include <optional>
#include <string>
namespace laya
{
enum class TextAlign
{
    Left,
    Right,
    Center
};

enum class TextBaseline
{
    Alphabetic,
    Top,
    Hanging,
    Middle,
    Ideographic,
    Bottom
};

enum class LineJoin
{
    Round,
    Bevel,
    Miter
};

class CanvasRenderingContext2D
{
  public:
    static CanvasRenderingContext2D *create(int width, int height);
    static CanvasRenderingContext2D *get(int id);
    CanvasRenderingContext2D(int width, int height);
    virtual ~CanvasRenderingContext2D();
    virtual void fillText(const std::string &text, double x, double y, std::optional<double> maxWidth) = 0;

    virtual void strokeText(const std::string &text, double x, double y, std::optional<double> maxWidth) = 0;
    virtual TextMetrics measureText(const std::string &text) = 0;
    virtual void clearRect(double x, double y, double width, double height) = 0;
    virtual void save() = 0;
    virtual void restore() = 0;
    virtual ImageData getImageData(double x, double y, double width, double height) = 0;
    virtual void setTransform(double a, double b, double c, double d, double e, double f) = 0;
    virtual void scale(double x, double y) = 0;
    const char *getFont()
    {
        return m_font.c_str();
    }
    virtual void setFont(const char *font)
    {
        m_font = font;
        FontDescriptionParser::parse(font, m_fontDescription);
    }
    virtual const BitmapData &getBitmapData() const = 0;
    int getID()
    {
        return m_id;
    }
    virtual void setTextAlign(const char *textAlign);
    const char *getTextAlign();

    virtual void setTextBaseline(const char *textBaseline);
    const char *getTextBaseline();

    virtual void setFillStyle(const char *color);
    const char *getFillStyle();

    virtual void setStrokeStyle(const char *color);
    const char *getStrokeStyle();

    virtual void setLineWidth(double lineWidth)
    {
        m_lineWidth = lineWidth;
    }
    virtual double getLineWidth()
    {
        return m_lineWidth;
    }

    virtual void setLineJoin(const char *lineJoin);
    const char *getLineJoin();

  protected:
    void getTextPosition(const std::string &text, double x, double y, double &outX, double &outY);
    void setDefault();
  protected:
    int m_width;
    int m_height;
    int32_t m_id{0};
    std::string m_font;
    FontDescription m_fontDescription;
    TextAlign m_textAlign{TextAlign::Left};
    TextBaseline m_textBaseline{TextBaseline::Alphabetic};
    LineJoin m_lineJoin{LineJoin::Miter};
    std::string m_fillStyle{"#000000"};
    std::string m_strokeStyle{"#000000"};
    uint32_t m_fillColorR{0};
    uint32_t m_fillColorG{0};
    uint32_t m_fillColorB{0};
    uint32_t m_fillColorA{0};

    uint32_t m_strokeColorR{0};
    uint32_t m_strokeColorG{0};
    uint32_t m_strokeColorB{0};
    uint32_t m_strokeColorA{0};

    double m_lineWidth{1.0};
};
} // namespace laya
#endif
