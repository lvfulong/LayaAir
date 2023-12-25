#ifndef __JSFileSystemManager_H__
#define __JSFileSystemManager_H__

#include <binder/JSInterface.h>
#include <set>
namespace laya
{
    class JSFileSystemManager 
	{
    public:

        static JSFileSystemManager* getInstance();



		void exportJS(Module& module);

        JSFileSystemManager();

		~JSFileSystemManager();

        JsValue writeFileSync(const char *filePath, JSValueAsParam data, const char *encoding);
     
        void writeFile(JSValueAsParam param);
		void writeFileImpl(std::weak_ptr<int> callbackref, JSValueAsParam param);

        void unlink(JSValueAsParam param);
		void unlinkImpl(std::weak_ptr<int> callbackref, JSValueAsParam param);

        JsValue unlinkSync(const char *filePath);

        void getFileInfo(JSValueAsParam param);
		void getFileInfoImpl(std::weak_ptr<int> callbackref, JSValueAsParam param);

        void mkdir(JSValueAsParam param);
		void mkdirSync(const char* dirPath, bool recursive);
		void mkdirImpl(std::weak_ptr<int> callbackref, JSValueAsParam param);

        void rmdir(JSValueAsParam param);
		void rmdirImpl(std::weak_ptr<int> callbackref, JSValueAsParam param);

        void copyFile(JSValueAsParam param);
		void copyFileImpl(std::weak_ptr<int> callbackref, JSValueAsParam param);

        //string | ArrayBuffer
        JsValue readFileSync(const char *filePath, const char *encoding/*, const char *position, const char *length*/);

        void readFile(JSValueAsParam param);
		void readFileImpl(std::weak_ptr<int> callbackref, JSValueAsParam param);

        void readDir(JSValueAsParam param);
		JsValue readdirSync(const char* dirPath);
		void readDirImpl(std::weak_ptr<int> callbackref, JSValueAsParam param);

		JsValue statSync(const char* path/*, bool recursive*/);
		void stat(JSValueAsParam param);
    public:

        static JSFileSystemManager*  ms_pFileSystemManager;	
    

    private:
        enum class ENCODING_TYPE
        {
            UTF8 = 0,
            BINARY,
			NONE,
            INVALID,
        };
        void handleOnFailed(JSValueAsParam failCallback, JSValueAsParam completeCallback, const char *errMsg);

        void handleOnSuccess(JSValueAsParam sucessCallback, JSValueAsParam completeCallback);

        void handleOnSuccess(JSValueAsParam sucessCallback, JSValueAsParam completeCallback, JSValueAsParam param);

		void handleOnFailed(Persistent* paramHandle, const char *errMsg);

		void handleOnSuccess(Persistent* paramHandle, JSValueAsParam param);

		void handleOnSuccess(Persistent* paramHandle);

		std::shared_ptr<int>	m_CallbackRef;
	};
}
#endif
