package layaair.game.browser;
import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Build;
import android.view.Surface;

import layaair.game.conch.LayaConch5;

public class ConchJNI {
	public static boolean g_bInitialized=false;
	public static boolean initNativeLibrary(String library, boolean plugin) {
		try {
			if(plugin) {
				System.load(library);
			}
			else {
				System.loadLibrary(library);
			}
			g_bInitialized = true;
		}
		catch(Exception e) {
			e.printStackTrace();
			g_bInitialized = false;
		}
		return g_bInitialized;
	}

	public static class ConchOptions {
		public String url;
		public Object assetManager;
		public String filesDir;
        public String cacheDir;
		public String apkExpansionMainPath;
		public String apkExpansionPatchPath;
	}
	
	public static native void init(Activity activity, ConchOptions options, Surface surface);
	public static native void uninit();
	public static native void handleTouch( int type,int id,int x,int y );
	public static native void handleKeyEvent(int keyCode, int actionType);
	public static native void handleJoystickEvent(float THUMBL_xOffset,float THUMBL_yOffset,float THUMBR_xOffset,float THUMBR_yOffset,float LT_Offset,float RT_Offset);
	public static native void handleDeviceMotionEvent(float ax, float ay, float az, float agx, float agy, float agz, float ra, float rb, float rg, float interval);
	public static native void handleDeviceOrientationEvent(float ra,float rb,float rg);
	public static native void onAppPause();
	public static native void onSurfaceResize(int width,int height);
	public static native void onSurfaceCreated(Object surface);
	public static native void onSurfaceDestroy();
	public static native void onAppResume();
	public static native void performUpdates();
	public static native void exportStaticMethodToC( String packcls);
	public static native void alertCallback();
	public static native void RunJS(String jsstring);
	public static native void networkChanged( int p_nNetworkType );
	public static native void inputChange(int keyCode);
	public static native void handleKeyboardInput(String strValue);
	public static native void handleKeyboardConfirm(String strValue);
	public static native void handleKeyboardComplete(String strValue);
	public static native void reloadJS();
	public static native void urlBack();
	public static native void handleAsyncMessageMethodNative(long nativeHandle, String result);
	public static native void onTrimMemory(int level);
}
