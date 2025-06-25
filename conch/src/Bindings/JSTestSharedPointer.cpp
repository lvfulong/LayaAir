#include "JSTestSharedPointer.h"
#include <jsbind/JSBind.h>
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include <downloadMgr/JCDownloadMgr.h>
#include <utils/JCMemorySurvey.h>
#include <utils/JCFileSystem.h>
#include <resource/JCFileResManager.h>
#include <utils/JCLayaUrl.h>
#include <utils/JCCommonMethod.h>
#include "JSFile.h"
#include "../../JCScriptRuntime.h"
#include <functional>
#include "../../JCSystemConfig.h"
#include "JCConch.h"
#include <audio/StaticDecoderCache.h>
#include <audio/StreamDecoder.h>
#include <profiler/Profiler.h>
namespace laya
{
	std::shared_ptr<JSTestSharedPointer> s_testSharedPointer;
    JSTestSharedPointer::JSTestSharedPointer()
    {
		
		LOGI("JSTestSharedPointer::JSTestSharedPointer");
    }
    JSTestSharedPointer::~JSTestSharedPointer()
    {
		LOGI("JSTestSharedPointer::~JSTestSharedPointer()");
    }
	void JSTestSharedPointer::test(JSTestSharedPointer* dd)
	{
		s_testSharedPointer = shared_from_this();
		LOGI("s_testSharedPointer:%p",s_testSharedPointer.get());
		LOGI("JSTestSharedPointer::test");


	}
	void JSTestSharedPointer::test2(std::shared_ptr<JSTestSharedPointer> dd)
	{
		LOGI("s_testSharedPointer:%p", s_testSharedPointer.get());
		LOGI("JSTestSharedPointer::test");


	}
    void JSTestSharedPointer::exportJS(jsbind::Object& context)
    {
		jsbind::class_<JSTestSharedPointer, jsbind::shared_ptr_traits> class_binding;
		class_binding.function("test", &JSTestSharedPointer::test);
		class_binding.function("test2", &JSTestSharedPointer::test2);
		class_binding.constructor<>();
		context.class_("JSTestSharedPointer", class_binding);
    }
}	