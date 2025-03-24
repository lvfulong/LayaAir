#include "CanvasRenderingContext2DAndroid.h"
#include <utils/Log.h>
#include <cassert>
#include <utils/JCBuffer.h>
#include <utils/JCFileSystem.h>
#include <platform/OS.h>
#include "FontDescription.h"

namespace laya
{
jclass CanvasRenderingContext2DAndroid::s_cls = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_createMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_fillTextMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_strokeTextMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_measureTextMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_getImageDataMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_saveMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_restoreMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setTransformMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_clearRectMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setStrokeColorMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setFillColorMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setTextSizeMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setTypefaceMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setTextAlignMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setTextBaselineMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_scaleMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setLineWidthMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_setLineJoinMethodID = nullptr;
jmethodID CanvasRenderingContext2DAndroid::s_registerFontFromPathMethodID = nullptr;

void CanvasRenderingContext2DAndroid::addStaticMethod(JNIEnv *env, const char *className)
{
    jclass cls = env->FindClass(className);
    s_cls = (jclass)env->NewGlobalRef(cls);
    s_createMethodID = env->GetStaticMethodID(s_cls, "create",
                                              "(JII)Llayaair/"
                                              "game/browser/LayaCanvasRenderingContext2D;");

    s_fillTextMethodID = env->GetStaticMethodID(s_cls, "fillText",
                                                "(Llayaair/"
                                                "game/browser/LayaCanvasRenderingContext2D;Ljava/lang/String;FF)V");

    s_strokeTextMethodID = env->GetStaticMethodID(s_cls, "strokeText",
                                                  "(Llayaair/"
                                                  "game/browser/LayaCanvasRenderingContext2D;Ljava/lang/String;FF)V");

    s_measureTextMethodID = env->GetStaticMethodID(s_cls, "measureText",
                                                   "(Llayaair/"
                                                   "game/browser/LayaCanvasRenderingContext2D;Ljava/lang/String;)[F");

    s_getImageDataMethodID = env->GetStaticMethodID(s_cls, "getImageData",
                                                    "(Llayaair/"
                                                    "game/browser/LayaCanvasRenderingContext2D;IIII)[B");

    s_saveMethodID = env->GetStaticMethodID(s_cls, "save",
                                            "(Llayaair/"
                                            "game/browser/LayaCanvasRenderingContext2D;)V");
    s_restoreMethodID = env->GetStaticMethodID(s_cls, "restore",
                                               "(Llayaair/"
                                               "game/browser/LayaCanvasRenderingContext2D;)V");

    s_setTransformMethodID = env->GetStaticMethodID(s_cls, "setTransform",
                                                    "(Llayaair/"
                                                    "game/browser/LayaCanvasRenderingContext2D;FFFFFF)V");
    s_clearRectMethodID = env->GetStaticMethodID(s_cls, "clearRect",
                                                 "(Llayaair/"
                                                 "game/browser/LayaCanvasRenderingContext2D;FFFF)V");

    s_setStrokeColorMethodID = env->GetStaticMethodID(s_cls, "setStrokeColor",
                                                      "(Llayaair/"
                                                      "game/browser/LayaCanvasRenderingContext2D;IIII)V");

    s_setFillColorMethodID = env->GetStaticMethodID(s_cls, "setFillColor",
                                                    "(Llayaair/"
                                                    "game/browser/LayaCanvasRenderingContext2D;IIII)V");

    s_setTextSizeMethodID = env->GetStaticMethodID(s_cls, "setTextSize",
                                                   "(Llayaair/"
                                                   "game/browser/LayaCanvasRenderingContext2D;F)V");

    s_setTypefaceMethodID = env->GetStaticMethodID(s_cls, "setTypeface",
                                                   "(Llayaair/"
                                                   "game/browser/LayaCanvasRenderingContext2D;Ljava/lang/String;I)V");

    s_setTextAlignMethodID = env->GetStaticMethodID(s_cls, "setTextAlign",
                                                    "(Llayaair/"
                                                    "game/browser/LayaCanvasRenderingContext2D;I)V");
    s_setTextBaselineMethodID = env->GetStaticMethodID(s_cls, "setTextBaseline",
                                                       "(Llayaair/"
                                                       "game/browser/LayaCanvasRenderingContext2D;I)V");

    s_scaleMethodID = env->GetStaticMethodID(s_cls, "scale",
                                             "(Llayaair/"
                                             "game/browser/LayaCanvasRenderingContext2D;FF)V");

    s_setLineWidthMethodID = env->GetStaticMethodID(s_cls, "setLineWidth",
                                                    "(Llayaair/"
                                                    "game/browser/LayaCanvasRenderingContext2D;F)V");

    s_setLineJoinMethodID = env->GetStaticMethodID(s_cls, "setLineJoin",
                                                   "(Llayaair/"
                                                   "game/browser/LayaCanvasRenderingContext2D;I)V");
    s_registerFontFromPathMethodID =
        env->GetStaticMethodID(s_cls, "registerFontFromPath", "(Ljava/lang/String;Ljava/lang/String;)Z");
}
CanvasRenderingContext2DAndroid::CanvasRenderingContext2DAndroid(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    jobject jobject = env->CallStaticObjectMethod(s_cls, s_createMethodID, (jlong)this, width, height);
    m_object = env->NewGlobalRef(jobject);
    env->DeleteLocalRef(jobject);

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];

    setDefault();
}
CanvasRenderingContext2DAndroid::~CanvasRenderingContext2DAndroid()
{
    CToJavaBridge::ThreadJNIData *pThreadData = CToJavaBridge::GetInstance()->checkThreadJNI();
    if (!pThreadData)
        return;
    JNIEnv *pJNI = pThreadData->pThreadJNI;
    if (m_object != nullptr)
    {
        pJNI->DeleteGlobalRef(m_object);
    }
}
void CanvasRenderingContext2DAndroid::setLineWidth(double lineWidth)
{
    CanvasRenderingContext2D::setLineWidth(lineWidth);

    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_setLineWidthMethodID, m_object, lineWidth);
}
void CanvasRenderingContext2DAndroid::setLineJoin(const char *lineJoin)
{
    CanvasRenderingContext2D::setLineJoin(lineJoin);

    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_setLineJoinMethodID, m_object, m_lineJoin);
}
void CanvasRenderingContext2DAndroid::fillText(const std::string &text, double x, double y,
                                               std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }

    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    jstring jText = env->NewStringUTF(text.c_str());
    env->CallStaticVoidMethod(s_cls, s_fillTextMethodID, m_object, jText, x, y);
    env->DeleteLocalRef(jText);
}
void CanvasRenderingContext2DAndroid::strokeText(const std::string &text, double x, double y,
                                                 std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    jstring jText = env->NewStringUTF(text.c_str());
    env->CallStaticVoidMethod(s_cls, s_strokeTextMethodID, m_object, jText, x, y);
    env->DeleteLocalRef(jText);
}
TextMetrics CanvasRenderingContext2DAndroid::measureText(const std::string &text)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    jstring jText = env->NewStringUTF(text.c_str());
    jfloatArray returnArray = (jfloatArray)env->CallStaticObjectMethod(s_cls, s_measureTextMethodID, m_object, jText);
    if (!returnArray)
    { // android 8 crash unknow reseaon
        env->DeleteLocalRef(jText);
        return TextMetrics();
    }

    jfloat *arrayBody = env->GetFloatArrayElements(returnArray, JNI_FALSE);
    float width = arrayBody[0];
    float height = arrayBody[1];
    float ascent = arrayBody[2];
    env->ReleaseFloatArrayElements(returnArray, arrayBody, 0);
    env->DeleteLocalRef(returnArray);
    env->DeleteLocalRef(jText);

    TextMetrics metrics;

    metrics.m_width = ceilf(width);
    metrics.m_height = ceilf(height);
    metrics.m_ascender = ceilf(ascent);
    return metrics;
}
void CanvasRenderingContext2DAndroid::clearRect(double x, double y, double width, double height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_clearRectMethodID, m_object, x, y, width, height);
}
void CanvasRenderingContext2DAndroid::save()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }

    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_saveMethodID, m_object);
}
void CanvasRenderingContext2DAndroid::restore()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_restoreMethodID, m_object);
}
ImageData CanvasRenderingContext2DAndroid::getImageData(double x, double y, double width, double height)
{
    int clampedX = std::clamp(x, 0.0, static_cast<double>(m_width));
    int clampedY = std::clamp(y, 0.0, static_cast<double>(m_height));
    int clampedW = std::clamp(width, 0.0, static_cast<double>(m_width) - x);
    int clampedH = std::clamp(height, 0.0, static_cast<double>(m_height) - y);

    if (clampedW > 0 && clampedH > 0)
    {
        ImageData data;
        data.m_width = clampedW;
        data.m_height = clampedH;
        data.m_data.resize(clampedW * clampedH * 4);

        CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
        JNIEnv *env = threadJniData->pThreadJNI;

        jbyteArray returnArray = (jbyteArray)env->CallStaticObjectMethod(s_cls, s_getImageDataMethodID, m_object,
                                                                         clampedX, clampedY, clampedW, clampedH);

        jsize arrayLen = env->GetArrayLength(returnArray);
        assert(arrayLen == data.m_data.size());
        env->GetByteArrayRegion(returnArray, 0, arrayLen, (jbyte *)data.m_data.data());
        env->DeleteLocalRef(returnArray);
        return data;
    }

    return ImageData();
}
const BitmapData &CanvasRenderingContext2DAndroid::getBitmapData() const
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;
    assert(m_width > 0 && m_height > 0);
    jbyteArray returnArray =
        (jbyteArray)env->CallStaticObjectMethod(s_cls, s_getImageDataMethodID, m_object, 0, 0, m_width, m_height);

    jsize arrayLen = env->GetArrayLength(returnArray);
    assert(arrayLen == m_width * m_height * 4);
    env->GetByteArrayRegion(returnArray, 0, arrayLen, (jbyte *)m_bitmapData.m_pImageData);
    env->DeleteLocalRef(returnArray);

    return m_bitmapData;
}
void CanvasRenderingContext2DAndroid::setTransform(double a, double b, double c, double d, double e, double f)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_setTransformMethodID, m_object, a, b, c, d, e, f);
}
void CanvasRenderingContext2DAndroid::scale(double x, double y)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    env->CallStaticVoidMethod(s_cls, s_scaleMethodID, m_object, x, y);
}
void CanvasRenderingContext2DAndroid::setTextAlign(const char *textAlign)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;
    CanvasRenderingContext2D::setTextAlign(textAlign);

    env->CallStaticVoidMethod(s_cls, s_setTextAlignMethodID, m_object, m_textAlign);
}
void CanvasRenderingContext2DAndroid::setTextBaseline(const char *textBaseline)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;
    CanvasRenderingContext2D::setTextBaseline(textBaseline);

    env->CallStaticVoidMethod(s_cls, s_setTextBaselineMethodID, m_object, m_textBaseline);
}
void CanvasRenderingContext2DAndroid::setFillStyle(const char *color)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;
    CanvasRenderingContext2D::setFillStyle(color);

    env->CallStaticVoidMethod(s_cls, s_setFillColorMethodID, m_object, m_fillColorR, m_fillColorG, m_fillColorB,
                              m_fillColorA);
}
void CanvasRenderingContext2DAndroid::setStrokeStyle(const char *color)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;
    CanvasRenderingContext2D::setStrokeStyle(color);

    env->CallStaticVoidMethod(s_cls, s_setStrokeColorMethodID, m_object, m_strokeColorR, m_strokeColorG, m_strokeColorB,
                              m_strokeColorA);
}
void CanvasRenderingContext2DAndroid::setFont(const char *font)
{
    CanvasRenderingContext2D::setFont(font);

    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;
    bool isBold = m_fontDescription->isBold();
    bool isItalic = m_fontDescription->isItalic();
    int style = 0;
    if (isBold)
    {
        style |= 1;
    }
    if (isItalic)
    {
        style |= 2;
    }
    setTypeface(env, m_fontDescription->m_family, style);
    setTextSize(env, m_fontDescription->m_size);
}

void CanvasRenderingContext2DAndroid::setTextSize(JNIEnv *env, float size)
{
    env->CallStaticVoidMethod(s_cls, s_setTextSizeMethodID, m_object, size);
}
void CanvasRenderingContext2DAndroid::setTypeface(JNIEnv *env, const std::string &fontName, int style)
{
    jstring jFontName = env->NewStringUTF(fontName.c_str());
    env->CallStaticVoidMethod(s_cls, s_setTypefaceMethodID, m_object, jFontName, style);
    env->DeleteLocalRef(jFontName);
}
bool CanvasRenderingContext2DAndroid::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    CToJavaBridge::ThreadJNIData *threadJniData = CToJavaBridge::GetInstance()->checkThreadJNI();
    JNIEnv *env = threadJniData->pThreadJNI;

    jstring jFontName = env->NewStringUTF(fontName.c_str());
    jstring jPath = env->NewStringUTF(path.c_str());
    bool ok = (bool)env->CallStaticBooleanMethod(s_cls, s_registerFontFromPathMethodID, jFontName, jPath);
    env->DeleteLocalRef(jFontName);
    env->DeleteLocalRef(jPath);
    return ok;
}
bool CanvasRenderingContext2DAndroid::registerFontFromBuffer(const std::string& fontName, uint8_t* buff, int len) 
{
    JCBuffer buf((char *)buff, len, false, false);
    std::string tempFilePath = OS::getTemporaryCachePath() + std::string("/tmp_") + fontName;
    writeFileSync(tempFilePath.c_str(), buf);
    return CanvasRenderingContext2DAndroid::registerFontFromPath(fontName, tempFilePath);
}
} // namespace laya
