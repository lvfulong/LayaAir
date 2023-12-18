#ifndef __CanvasRenderingContext2DCG_H__
#define __CanvasRenderingContext2DCG_H__
#include "CanvasRenderingContext2D.h"
#include <optional>
#include <string>

namespace laya
{
class CanvasRenderingContext2DCGImpl;
class CanvasRenderingContext2DCG : public CanvasRenderingContext2D
{
  public:
    CanvasRenderingContext2DCG(int width, int height);
    ~CanvasRenderingContext2DCG();
    void fillText(const std::string &text, double x, double y, std::optional<double> maxWidth) override;

    void strokeText(const std::string &text, double x, double y, std::optional<double> maxWidth) override;
    TextMetrics measureText(const std::string &text) override;
    void clearRect(double x, double y, double width, double height) override;
    void save() override;
    void restore() override;
    ImageData getImageData(double x, double y, double width, double height) override;
    void setTransform(double a, double b, double c, double d, double e, double f) override;
    void scale(double x, double y) override;
    void setFont(const char *font) override;
    const BitmapData &getBitmapData() const override;

  protected:
    void chooseFont(const std::string& strFontName);
    CanvasRenderingContext2DCGImpl *m_impl = {nullptr};
};
} // namespace laya
#endif
