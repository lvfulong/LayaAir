#ifndef __LAYA_PREPROCESSOR_H__
#define __LAYA_PREPROCESSOR_H__
#ifdef __APPLE__
#define GLES_SILENCE_DEPRECATION
#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#elif ANDROID
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#elif _WIN32
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef _WIN32
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