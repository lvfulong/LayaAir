#ifndef __CanvasRenderingContext2DOHOS_H__
#define __CanvasRenderingContext2DOHOS_H__

#include "2D/CanvasRenderingContext2D.h"
#include "napi/native_api.h"
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <algorithm>
#include <cmath>
#include <native_drawing/drawing_bitmap.h>
#include <native_drawing/drawing_brush.h>
#include <native_drawing/drawing_canvas.h>
#include <native_drawing/drawing_color.h>
#include <native_drawing/drawing_font_collection.h>
#include <native_drawing/drawing_matrix.h>
#include <native_drawing/drawing_path.h>
#include <native_drawing/drawing_pen.h>
#include <native_drawing/drawing_register_font.h>
#include <native_drawing/drawing_text_typography.h>
#include <native_window/external_window.h>
#include <optional>
#include <stdint.h>
#include <string>
#include <sys/mman.h>

namespace laya
{
class ScopedTypography;
class CanvasRenderingContext2DOHOS : public CanvasRenderingContext2D
{
  public:
    CanvasRenderingContext2DOHOS(int width, int height);
    ~CanvasRenderingContext2DOHOS();
    void fillText(const std::string &text, double x, double y, std::optional<double> maxWidth) override;
    static void init();
    static void destroy();
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
    static bool registerFontFromBuffer(const std::string& fontName, uint8_t* buff, int len);
    void setFillStyle(const char *color) override;
    void setStrokeStyle(const char *color) override;

  protected:
    std::unique_ptr<ScopedTypography> createTypography(const std::string &text);
    void getTextPosition(const std::string &text, double x, double y, double &outX, double &outY) override;
    TextMetrics measureTextUtf16(wchar_t *pwszBuffer, int bufferLen);
    OH_Drawing_Typography *realDraw(const char *text);
    OH_Drawing_Bitmap *m_bitmap;
    OH_Drawing_Canvas *m_canvas;
    OH_Drawing_TextStyle *m_fontTextStyle;
    OH_Drawing_TypographyStyle *m_fontTypography;
    static OH_Drawing_FontCollection *m_fontCollection;
    OH_Drawing_TypographyCreate *m_typographyCreate;
    OH_Drawing_Matrix *m_matrix = nullptr;
    BitmapData m_bitmapData;
};
} // namespace laya
#endif
