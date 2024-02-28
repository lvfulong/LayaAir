#ifndef __CanvasRenderingContext2DWin_H__
#define __CanvasRenderingContext2DWin_H__

#include <windows.h>
#include <gdiplus.h>
#include "CanvasRenderingContext2D.h"
#include <optional>
#include <string>


namespace laya
{
class CanvasRenderingContext2DWin : public CanvasRenderingContext2D
{
  public:
    CanvasRenderingContext2DWin(int width, int height);
    ~CanvasRenderingContext2DWin();
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
    void setTextAlign(const char *textAlign) override;
    void setTextBaseline(const char *textBaseline) override;
    void setLineWidth(double lineWidth) override;
    void setLineJoin(const char *lineJoin) override;
     static bool registerFontFromPath(const std::string &fontName, const std::string &path);
  protected:
    TextMetrics measureTextUtf16(wchar_t* pwszBuffer, int bufferLen);
    BitmapData m_bitmapData;
    Gdiplus::Bitmap* m_gdiBitmap;
    Gdiplus::Graphics* m_gdiGraphics;
    Gdiplus::Font* m_font = nullptr;
    Gdiplus::StringFormat m_stringFormat;
    Gdiplus::FontStyle  m_fontStyle;
};
} // namespace laya
#endif
