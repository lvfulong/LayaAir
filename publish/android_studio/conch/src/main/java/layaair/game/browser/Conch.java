package layaair.game.browser;
import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Build;
import android.view.Surface;

import layaair.game.conch.LayaConch5;

public class Conch
{
	public interface RunCallback {
		void runInGLThread(Runnable f);

		void runInUIThread(Runnable f);
	}
	public Conch(Activity activity, ConchJNI.ConchOptions options, RunCallback runCallbacks, Surface surface) {
		mRunCallback = runCallbacks;
		mRunCallback.runInGLThread(()->{
			ConchJNI.init(activity, options, surface);
		});
	}
	private RunCallback mRunCallback;
	public void onSurfaceResize(int width, int height) {
		mRunCallback.runInGLThread(() -> ConchJNI.onSurfaceResize(width, height));
	}
	public void onSurfaceDestroy() {
		mRunCallback.runInGLThread(() -> ConchJNI.onSurfaceDestroy());
	}
	public void onSurfaceCreated(Surface surface) {
		mRunCallback.runInGLThread(() -> ConchJNI.onSurfaceCreated(surface));
	}
	public void onPause() {
		mRunCallback.runInGLThread(() -> ConchJNI.onAppPause());
	}
	public void onResume() {
		mRunCallback.runInGLThread(() -> ConchJNI.onAppResume());
	}
	public void uninit() {
		mRunCallback.runInGLThread(() -> ConchJNI.uninit());
	}
}
