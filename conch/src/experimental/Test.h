#ifndef __Test_H__    
#define __Test_H__

#include <jsbind/JSBind.h>
#include "utils/JCBuffer.h"
#include <core/math/Vector4.h>

namespace laya
{
  enum FunctionType
  {
    testInt32 = 0,
    testFloat32 = 1,
    testString = 2,
    testBoolean = 3,
    testArrayBuffer = 4,
    testArrayBufferView = 5,
    testVector4 = 6,
    count = 7,
  };    
  class Test
  {
    public:
      Test();
      ~Test();
      static int createObject(const std::string& className);
      static void destroyObject(const std::string& className, int ptr);
      static void setBuffer(jsbind::ArrayBuffer value);
      static void flush();
      static void dispatchAllCmds(JCCommandEncoderBuffer* pTemplateMem);
      static bool dispatchScriptCmd(JCCommandEncoderBuffer& pRenderCmd);
      static void _test_testInt32(JCCommandEncoderBuffer& layaGLCmd);
      static void _test_testFloat(JCCommandEncoderBuffer& layaGLCmd);
      static void _test_testString(JCCommandEncoderBuffer& layaGLCmd);
      static void _test_testBoolean(JCCommandEncoderBuffer& layaGLCmd);
      static void _test_testArrayBuffer(JCCommandEncoderBuffer& layaGLCmd);
      static void _test_testArrayBufferView(JCCommandEncoderBuffer& layaGLCmd); 
      static void _test_testVector4(JCCommandEncoderBuffer& layaGLCmd);
      static char* m_pBuffer;
      static JCCommandEncoderBuffer m_renderCmd;
  };
  class TestCommandBuffer
  {
    public:
      TestCommandBuffer();
      ~TestCommandBuffer();
      void testInt32(int32_t value);
      void testFloat(float value);
      void testString(const std::string& value);
      void testBoolean(bool value);
      void testArrayBuffer(char* value, int bytes);
      void testArrayBufferView(char* value, int bytes);
      void testVector4(const Vector4& v4);
      void destroy();
      std::unordered_map<std::string, std::string> m_map; 
      Vector4 m_v4;
  };  
  class TestNormal
  {
    public:
      TestNormal();
      ~TestNormal();
      void testInt32(int32_t value);
      void testFloat(float value);
      void testString(const std::string& value);
      void testBoolean(bool value);
      void testArrayBuffer(jsbind::ArrayBuffer value);
      void testArrayBufferView(jsbind::ArrayBuffer value);
      void testVector4(const Vector4& v4);
      void destroy();
      std::unordered_map<std::string, std::string> m_map; 
      Vector4 m_v4;
  };  
} // namespace laya
#endif  