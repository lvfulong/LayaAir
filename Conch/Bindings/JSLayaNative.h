#ifndef __JSLayaNative_H__
#define __JSLayaNative_H__

#include <Binder/JSInterface.h>
#include <set>
class MediaRecorder;
namespace laya
{
	

    class JSLayaNative
	{
		enum
		{
			onsuccessid,
			oncompleteid,
			onfailid,
			onsavealbumsuccessid,
			onsavealbumcompleteid,
			onsavealbumfailid,
		};

    public:

        static JSLayaNative* getInstance();


		void exportJS(Module& module);

        JSLayaNative();

		~JSLayaNative();

		void postMessage(JSValueAsParam param);

		std::string getSystemInfoSync();

		void exitMiniProgram();

		void finish();

		void vibrateShort(JSValueAsParam param);

		void vibrateLong(JSValueAsParam param);

		void chooseImage(JSValueAsParam param);

		void saveImageToPhotosAlbum(JSValueAsParam param);

		void startRecord(int recorderType, const char *outUrl, int frameWidth, int frameHeight, long videoBitRate, int fps);
		void onAudioData(uint8_t *pData, int size);
		void onRenderData(JSValueAsParam param);
		void stopRecord();
		void showKeyboard(JSValueAsParam param);

		void updateKeyboard(JSValueAsParam param);

		void onKeyboardInput(JSValueAsParam param);

		void offKeyboardInput();


		void onKeyboardConfirm(JSValueAsParam param);

		void offKeyboardConfirm();


		void onKeyboardComplete(JSValueAsParam param);

		void offKeyboardComplete();

		void hideKeyboard(JSValueAsParam param);

		void handleKeyboardInput(const char* strValue);

		void handleKeyboardConfirm(const char* strValue);

		void handleKeyboardComplete(const char* strValue);

		std::string createBufferURL(JSValueAsParam param);

		void revokeBufferURL(const char* url);

	protected:

		Persistent m_pOnFail;
		Persistent m_pOnSuccess;
		Persistent m_pOnComplete;

		Persistent m_pOnSaveAlbumFail;
		Persistent m_pOnSaveAlbumSuccess;
		Persistent m_pOnSaveAlbumComplete;

		Persistent m_pOnKeyboardInput;
		Persistent m_pOnKeyboardConfirm;
		Persistent m_pOnKeyboardComplete;

    public:

        static JSLayaNative*  ms_pLayaNative;
		void onComplete(std::weak_ptr<int> callbackRef, int resultCode, std::string jsonParam);
		void onCompleteCallJSFunction(int resultCode, std::string jsonParam);
		void onSaveImageComplete(int resultCode);
		void saveImageComplete(std::weak_ptr<int> callbackRef, int resultCode);
		

    private:

		std::shared_ptr<int>	m_CallbackRef;
		MediaRecorder *m_pAVRecorder = nullptr;
	};
}
#endif