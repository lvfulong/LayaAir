#ifndef __CanvasRenderingContext2DLinux_H__
#define __CanvasRenderingContext2DLinux_H__

#include "2D/CanvasRenderingContext2D.h"
#include <optional>
#include <string>

namespace laya
{
class xftTextRender;
class CanvasRenderingContext2DLinux : public CanvasRenderingContext2D
{
  public:
    CanvasRenderingContext2DLinux(int width, int height);
    ~CanvasRenderingContext2DLinux();
    void fillText(const std::string &text, double x, double y, std::optional<double> maxWidth) override;

    void strokeText(const std::string &text, double x, double y, std::optional<double> maxWidth) override;
    TextMetrics measureText(const std::string &text) override;
    void clearRect(double x, double y, double width, double height) override;
    void save() override;
    void restore() override;
    ImageData getImageData(double x, double y, double width, double height) override;
    void setTransform(double a, double b, double c, double d, double e, double f) override;
    void scale(double x, double y) override;
    const BitmapData &getBitmapData() const override;
    void setFont(const char *font) override;
    void setLineWidth(double lineWidth) override;
    void setLineJoin(const char *lineJoin) override;
    static bool registerFontFromPath(const std::string &fontName, const std::string &path);
    static bool registerFontFromBuffer(const std::string& fontName, const uint8_t* buff, int len);
    static void destroy();

  protected:
    void getTextPosition(const std::string &text, double x, double y, double &outX, double &outY) override;
    BitmapData m_bitmapData;
    xftTextRender* m_pTextRender;
};
} // namespace laya
#endif
