#ifndef __CanvasRenderingContext2DCG_H__
#define __CanvasRenderingContext2DCG_H__
#include "CanvasRenderingContext2D.h"
#include <optional>
#include <string>

namespace laya
{
class CanvasRenderingContext2DCGImpl;
class NativeInfoImpl;
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
    static bool registerFontFromPath(const std::string &fontName, const std::string &path);
    static bool registerFontFromBuffer(const std::string& fontName, uint8_t* buff, int len);
  protected:
    void getTextPosition(const std::string &text, double x, double y, double &outX, double &outY) override;
    void chooseFont(const std::string& strFontName);
    CanvasRenderingContext2DCGImpl *m_impl = {nullptr};


    
  private:
    static std::unordered_map<std::string, std::string> m_fontName2RealName;
    static std::unordered_map<std::string, NativeInfoImpl*> m_fontName2NativeInfoImpl;
    static std::pair<bool, std::string> getRealFontName(const std::string &family);
};
} // namespace laya
#endif
