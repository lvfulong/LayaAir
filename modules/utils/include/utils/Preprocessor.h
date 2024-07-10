#ifndef __LAYA_PREPROCESSOR_H__
#define __LAYA_PREPROCESSOR_H__
#ifdef OS_IOS
#define GLES_SILENCE_DEPRECATION
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif OS_OHOS
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#elif OS_LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#elif OS_ANDROID
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#elif OS_WINDOWS
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef OS_WINDOWS
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif
namespace laya
{
template <class T> inline void UNUSED(T const &)
{
}


} // namespace laya
#endif
