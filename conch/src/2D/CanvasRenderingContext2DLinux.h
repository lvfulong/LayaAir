#ifndef __CanvasRenderingContext2DLinux_H__
#define __CanvasRenderingContext2DLinux_H__

#include "CanvasRenderingContext2D.h"
#include <optional>
#include <string>

namespace laya
{
class CanvasRenderingContext2DLinuxImpl;
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

  protected:
    void getTextPosition(const std::string &text, double x, double y, double &outX, double &outY) override;
    CanvasRenderingContext2DLinuxImpl *m_impl = {nullptr};
    BitmapData m_bitmapData;
};
} // namespace laya
#endif
