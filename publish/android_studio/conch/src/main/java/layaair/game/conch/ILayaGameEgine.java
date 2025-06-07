package layaair.game.conch;

import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Rect;
import android.webkit.ValueCallback;
import android.widget.AbsoluteLayout;

 public abstract interface ILayaGameEgine {
	//	设置游戏缓存目录
	 void setAppCacheDir(String _cacheDir);
    //  设置扩展包路径
	 void setExpansionZipDir( final String mainPath,final String patchPath );

	//	设置so路径
	 void setSoPath(String _soPath);
	//	设置jar文件名
	 void setJarFile(String _jarFile);
    //  设置jar路径asset
    void setAssetInfo(AssetManager am);
	//	设置so文件名
	 void setSoFile(String _soFile);
	//	引擎销毁
	 void onDestroy();
	//	获得游戏显示的view
	 AbsoluteLayout getAbsLayout();
	//	监听游戏事件（退出）
	 void setLayaEventListener(ILayaEventListener _lis);

	//	设置游戏activity的context
	 void setContext(Context ctx);
	//	设置游戏地址
	 void setGameUrl(String _url);
	//	游戏初始化
	 void onCreate();
	//	游戏切到后台
	 void onPause();
	//	游戏恢复
	 void onResume();
	//	游戏停止
	 void onStop();
	 // 按键监听
	 void onKeyEvent(String keyEvent,int keyCode);

	 void onNewIntent(Intent intent);

	 void onRestart();

	 void onActivityResult(int requestCode, int resultCode,Intent intent);

	 void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults);

	//	设置是否拦截返回按键 默认不拦截
	 void setInterceptKey(boolean _intercept);

	 void setAlertTitle(String title);

	 void setStringOnBackPressed(String str);

	 void onTrimMemory(int level);
}
