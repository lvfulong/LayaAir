#ifndef __CanvasRenderingContext2DAndroid_H__
#define __CanvasRenderingContext2DAndroid_H__
#include "CToJavaBridge.h"
#include "CanvasRenderingContext2D.h"
#include <optional>
#include <string>
namespace laya
{
class CanvasRenderingContext2DCGImpl;
class CanvasRenderingContext2DAndroid : public CanvasRenderingContext2D
{
  public:
    CanvasRenderingContext2DAndroid(int width, int height);
    ~CanvasRenderingContext2DAndroid();
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
    void setFillStyle(const char *color) override;
    void setStrokeStyle(const char *color) override;
    void setLineWidth(double lineWidth) override;
    void setLineJoin(const char *lineJoin) override;

    static void addStaticMethod(JNIEnv *env, const char *className);
    static bool registerFontFromPath(const std::string &fontName, const std::string &path);

  protected:
    void setTextSize(JNIEnv *env, float size);
    void setTypeface(JNIEnv *env, const std::string &fontName, int style);

  protected:
    jobject m_object = nullptr;

    static jclass s_cls;
    static jmethodID s_createMethodID;
    static jmethodID s_fillTextMethodID;
    static jmethodID s_strokeTextMethodID;
    static jmethodID s_measureTextMethodID;
    static jmethodID s_getImageDataMethodID;
    static jmethodID s_saveMethodID;
    static jmethodID s_restoreMethodID;
    static jmethodID s_setTransformMethodID;
    static jmethodID s_clearRectMethodID;
    static jmethodID s_setStrokeColorMethodID;
    static jmethodID s_setFillColorMethodID;
    static jmethodID s_setTextSizeMethodID;
    static jmethodID s_setTypefaceMethodID;
    static jmethodID s_setTextAlignMethodID;
    static jmethodID s_setTextBaselineMethodID;
    static jmethodID s_scaleMethodID;
    static jmethodID s_setLineWidthMethodID;
    static jmethodID s_setLineJoinMethodID;
    static jmethodID s_registerFontFromPathMethodID;

    BitmapData m_bitmapData;
};
} // namespace laya
#endif
