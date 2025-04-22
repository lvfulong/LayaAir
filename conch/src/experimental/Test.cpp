#include "Test.h"
#include <vector>
#include <cstring>
#include <sstream>
#include <string>
#include "../JCCmdStruct.h"

namespace laya
{
    JCCommandEncoderBuffer Test::m_renderCmd(128, 128);
    char* Test::m_pBuffer = nullptr;
    Test::Test()
    {
    }

    Test::~Test()
    {
    }   

    int64_t Test::createObject(const std::string& className)       
    {
        if (className == "TestCommandBuffer")
        {
            return (int64_t)new TestCommandBuffer();
        }
        return 0;
    }

    void Test::destroyObject(const std::string& className, int64_t ptr)      
    {
        if (className == "TestCommandBuffer")
        {
            delete (TestCommandBuffer*)ptr;
        }       
    }
    void Test::setBuffer(jsbind::ArrayBuffer value)
    {
        Test::m_pBuffer = (char*)value.getData();
    }
    void Test::flush()
    {
        char* pBuffer = Test::m_pBuffer;
        int nLen = (*(int*)pBuffer - 1) * 4;
        m_renderCmd.setShareBuffer(pBuffer + 4, nLen);
        ((int*)pBuffer)[0] = 1;
        Test::dispatchAllCmds(&m_renderCmd);
        m_renderCmd.clearData();
    }   
    void Test::dispatchAllCmds(JCCommandEncoderBuffer* pTemplateMem)
    {
            //循环模板指令流
		pTemplateMem->setReadPos(0);
		int nLen = pTemplateMem->getDataSize();
		while (pTemplateMem->getReadPos() < (size_t)nLen)
		{
			dispatchScriptCmd(*pTemplateMem);
		}
    }
    typedef void(*ProcFunction)(JCCommandEncoderBuffer& layaGLCmd);
    bool Test::dispatchScriptCmd(JCCommandEncoderBuffer& pRenderCmd)
    {
		static int nLastPos = 0;
		static int nFuncID = 0;
        static const ProcFunction g_svProcFunctions[] =
        {
            &Test::_test_testInt32,		
            &Test::_test_testFloat,
            &Test::_test_testString,
            &Test::_test_testBoolean,
            &Test::_test_testArrayBuffer,
            &Test::_test_testArrayBufferView,       
        };
        static const int nFuncs = sizeof(g_svProcFunctions) / sizeof(g_svProcFunctions[0]);
        char* pCmdBuffer = pRenderCmd.getReadPtr();
        if (pCmdBuffer)
        {
#ifdef DEBUG_PARSE_STREAM
			int nCurrentPos = pRenderCmd.getReadPos();
			if (nCurrentPos == 0) {
				nLastPos = 0;
			}
			nLastPos = pRenderCmd.getReadPos();
#endif 
            nFuncID = *(pRenderCmd.popp<int>());
            if (nFuncID >= 0 && nFuncID < nFuncs)
            {
                (g_svProcFunctions[nFuncID])(pRenderCmd);
            }
            else
            {
                LOGE("JCLayaGLDispatch::dispatchScriptCmd Wrong command number!,cmd=%d", nFuncID);
            }
        }
        return true;
    }
    void Test::_test_testInt32(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_i64i* cmd = layaGLCmd.popp<CMD_i64i>();
        TestCommandBuffer* pTestCommandBuffer = (TestCommandBuffer*)cmd->a;
        pTestCommandBuffer->testInt32(cmd->b);
    }
    void Test::_test_testFloat(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_i64f* cmd = layaGLCmd.popp<CMD_i64f>();
        TestCommandBuffer* pTestCommandBuffer = (TestCommandBuffer*)cmd->a;
        pTestCommandBuffer->testFloat(cmd->b);
    }   
    void Test::_test_testString(JCCommandEncoderBuffer& layaGLCmd)
    {
    }
    void Test::_test_testBoolean(JCCommandEncoderBuffer& layaGLCmd)
    {
        CMD_i64i* cmd = layaGLCmd.popp<CMD_i64i>();
        TestCommandBuffer* pTestCommandBuffer = (TestCommandBuffer*)cmd->a;
        pTestCommandBuffer->testBoolean((bool)cmd->b);
    }
    void Test::_test_testArrayBuffer(JCCommandEncoderBuffer& layaGLCmd)
    {
    }
    void Test::_test_testArrayBufferView(JCCommandEncoderBuffer& layaGLCmd)
    {
    }   

    TestNormal::TestNormal()
    {
    }

    TestNormal::~TestNormal()
    {
    }       

    void TestNormal::testInt32(int32_t value)
    {       
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testInt32"] = str;   
    }

    void TestNormal::testFloat(float value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testFloat"] = str;
    }

    void TestNormal::testString(const std::string& value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testString"] = str;
    }

    void TestNormal::testBoolean(bool value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str(); 
        m_map["testBoolean"] = str;
    }   

    void TestNormal::testArrayBuffer(jsbind::ArrayBuffer value)
    {
        static std::vector<uint8_t> buffer;
        buffer.resize(value.getByteLength());
        memcpy(buffer.data(), value.getData(), value.getByteLength());
    }

    void TestNormal::testArrayBufferView(jsbind::ArrayBuffer value)
    {
        static std::vector<uint8_t> buffer;
        buffer.resize(value.getByteLength());
        memcpy(buffer.data(), value.getData(), value.getByteLength());
    }

    void TestNormal::destroy()      
    {

        m_map.clear();
    }


    TestCommandBuffer::TestCommandBuffer()
    {
    }

    TestCommandBuffer::~TestCommandBuffer()
    {
    }       

    void TestCommandBuffer::testInt32(int32_t value)
    {       
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testInt32"] = str;   
    }

    void TestCommandBuffer::testFloat(float value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testFloat"] = str;
    }

    void TestCommandBuffer::testString(const std::string& value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str();
        m_map["testString"] = str;
    }

    void TestCommandBuffer::testBoolean(bool value)
    {
        std::stringstream ss;
        ss << value;
        std::string str = ss.str(); 
        m_map["testBoolean"] = str;
    }   

    void TestCommandBuffer::testArrayBuffer(jsbind::ArrayBuffer value)
    {
        static std::vector<uint8_t> buffer;
        buffer.resize(value.getByteLength());
        memcpy(buffer.data(), value.getData(), value.getByteLength());
    }

    void TestCommandBuffer::testArrayBufferView(jsbind::ArrayBuffer value)
    {
        static std::vector<uint8_t> buffer;
        buffer.resize(value.getByteLength());
        memcpy(buffer.data(), value.getData(), value.getByteLength());
    }

    void TestCommandBuffer::destroy()      
    {

        m_map.clear();
    }
} // namespace laya
