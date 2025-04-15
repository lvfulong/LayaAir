#ifndef __JSCanvasRenderingContext2D_H__
#define __JSCanvasRenderingContext2D_H__

// 包含头文件
#include <jsbind/JSBind.h>
#include <stdio.h>

namespace laya
{
class CanvasRenderingContext2D;
class JSCanvasRenderingContext2D
{
  public:
    JSCanvasRenderingContext2D();
    JSCanvasRenderingContext2D(int width, int height);
    ~JSCanvasRenderingContext2D();

    void fillText(const char *text, double x, double y, double maxWidth);

    void strokeText(const char *text, double x, double y, double maxWidth);
    jsvm_value measureText(const std::string &text);
    void clearRect(double x, double y, double width, double height);
    void save();
    void restore();
    jsvm_value getImageData(double x, double y, double width, double height);
    void scale(double x, double y);
    void setTransform(double a, double b, double c, double d, double e, double f);
    const char *getFont();
    void setFont(const char *font);
    int getID();
    void setTextAlign(const char *textAlign);
    const char *getTextAlign();

    void setTextBaseline(const char *textBaseline);
    const char *getTextBaseline();

    void setFillStyle(const char *color);
    const char *getFillStyle();

    void setStrokeStyle(const char *color);
    const char *getStrokeStyle();
    
    void setLineWidth(double lineWidth);
    double getLineWidth();
    
    
    void setLineJoin(const char *lineJoin);
    const char *getLineJoin();

    static void exportJS(jsbind::Object &context);

  public:
    CanvasRenderingContext2D *m_context;
};
} // namespace laya

#endif //__JSCanvasRenderingContext2D_H__
