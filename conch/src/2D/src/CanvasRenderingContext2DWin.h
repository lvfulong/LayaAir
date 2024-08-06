#ifndef __CanvasRenderingContext2DWin_H__
#define __CanvasRenderingContext2DWin_H__

#include <windows.h>
#include "2D/CanvasRenderingContext2D.h"
#include <gdiplus.h>
#include <optional>
#include <string>
#include <gdiplusheaders.h>
#include <vector>

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
    // void setTextAlign(const char *textAlign) override;
    // void setTextBaseline(const char *textBaseline) override;
    void setLineWidth(double lineWidth) override;
    void setLineJoin(const char *lineJoin) override;
    static bool registerFontFromPath(const std::string &fontName, const std::string &path);
    static bool registerFontFromBuffer(const std::string& fontName, const uint8_t* buff, int len);
    static void init();
    static void clearAllBuffer();

  protected:
    //为了释放用
    static std::vector<char*> fontBuffers;
    void getTextPosition(const std::string &text, double x, double y, double &outX, double &outY);
    TextMetrics measureTextUtf16(wchar_t *pwszBuffer, int bufferLen);
    BitmapData m_bitmapData;
    Gdiplus::Bitmap *m_gdiBitmap;
    Gdiplus::Graphics *m_gdiGraphics;
    Gdiplus::Font *m_font = nullptr;
    //如果是自定义字体，用记录的family，而不要调用 GetFamily ，因为GetFamily后会在删除的时候真的删除字体（感觉没有加引用）
    //正常字体没有问题，具体怎么回事文档太少搞不清楚，崩溃。
    Gdiplus::FontFamily* m_pCustomFamily = nullptr;
    const Gdiplus::FontFamily* m_pLastFontFamily=nullptr;
    Gdiplus::StringFormat m_stringFormat;
    Gdiplus::FontStyle m_fontStyle;
};
} // namespace laya
#endif
