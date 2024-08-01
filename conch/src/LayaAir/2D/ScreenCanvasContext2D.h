#ifndef __ScreenCanvasContext2D_H__
#define __ScreenCanvasContext2D_H__

#include <atomic>
#include <memory>
#include <stack>
#include <stdio.h>
#include <vector>

namespace laya
{
class GLESEngine;
class GLESInternalTex;
class RenderTexture2D;
class GLESInternalRT;
class ScreenCanvasContext2D
{
  public:
    ScreenCanvasContext2D(GLESEngine *pWebglEngine);
    ~ScreenCanvasContext2D();
    void startForMainCanvas();
    void endForMainCanvas();
    void size(int w, int h);
    void captureScreen();
    void requestCaptureScreen();

  public:
    int m_width = 99999;
    int m_height = 99999;
    GLESEngine *m_pGLESEngine;
    GLESInternalTex *m_texture = nullptr;
    GLESInternalRT *m_target = nullptr;
    std::atomic_bool m_requestCaptureScreen{false};
};
} // namespace laya
#endif //__ScreenCanvasContext2D_H__
