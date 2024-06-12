#ifndef __LAYA_PREPROCESSOR_H__
#define __LAYA_PREPROCESSOR_H__
#ifdef __APPLE__
#define GLES_SILENCE_DEPRECATION
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif OHOS
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#elif __LINUX__
#include <glad/egl.h>
#include <glad/gles2.h>
#elif __ANDROID__
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#elif WIN32
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef WIN32
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
