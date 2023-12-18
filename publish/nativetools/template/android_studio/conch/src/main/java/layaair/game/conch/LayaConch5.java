package layaair.game.conch;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Locale;
import java.util.Vector;

import layaair.game.browser.CameraActivity;
import layaair.game.browser.ConchSurfaceView;
import layaair.game.browser.LayaVideoPlayer;
import layaair.game.browser.Picture.ImageUtils;
import layaair.game.browser.Picture.MultiImageSelectorActivity;
import layaair.game.browser.Picture.bean.CallbackRes;
import layaair.game.network.NetworkReceiver;
import layaair.game.browser.ExportJavaFunction;
import layaair.game.browser.ConchJNI;
import layaair.game.browser.LayaEditBox;
import layaair.game.browser.LayaWebView;
import layaair.game.config.config;
import layaair.game.device.DevID;
import layaair.game.utility.Constants;
import layaair.game.utility.LayaAudioMusic;
import layaair.game.utility.ProcessInfo;
import layaair.game.utility.Utils;

import android.Manifest;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.ContentValues;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Rect;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import androidx.core.app.ActivityCompat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Choreographer;
import android.view.Display;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.view.View.OnGenericMotionListener;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.danikula.videocache.HttpProxyCacheServer;
import com.google.gson.Gson;

import javax.microedition.khronos.opengles.GL10;

import static android.content.Context.SENSOR_SERVICE;
import static android.content.Context.WINDOW_SERVICE;
public class LayaConch5 implements ILayaGameEgine,OnKeyListener, Choreographer.FrameCallback {
	// view
	private Choreographer mChoreographer;
	private static final String TAG = "LayaConch5";
	public boolean localizable=false;
	public AbsoluteLayout m_pAbsLayout = null;
	public LayaWebView m_pLayaWebView = null;
	public LayaEditBox m_pEditBox = null;
	private AbsoluteLayout m_pEditBoxLayout = null;
	private ImageView mImageView = null;
	public ConchSurfaceView m_pCavans = null;
	public DevID m_pDevID = null;
	private ILayaEventListener m_layaEventListener = null;
	private boolean m_interceptKey = false;
	public AssetManager m_AM = null;
	public Context mCtx = null;
	public boolean m_bIsPlug = true;
	public String m_strUrl = "";
	public String m_strExt = "";
	public boolean m_bHorizontalScreen ; // 是否横屏
	private NetworkReceiver m_pNetWorkReveiver;
	private long m_nBackPressTime = 0;
	protected int m_nDownloadThreadNum = 3;
	protected String m_strCachePath = "";
	protected String m_strExpansionMainPath = "";
	protected String m_strExpansionPatchPath = "";
	protected String m_strConfigJS = "";
	static public String m_strSoPath = "";
	static public String m_strJarFile = "";
	static public String m_strSoFile = "/libconch.so";
	static Bundle m_marketBundle;
	static public final String MARKET_MARKETNAME = "marketName";
	static public final String MARKET_WAITSCREENBKCOLOR = "waitscreenbkcolor";
	static public final String MARKET_ENTERPLATFORMTYPE = "enterplatformtype";
	static public final String MARKET_EXITSHOWWEBURL = "exitshowweburle";
	static public final String MARKET_SERVERNAME = "servername";
	static public final String MARKET_PAYTYPE = "paytype";
	static public final String MARKET_LOGINTYPE = "logintype";
	static public final String MARKET_CHARGETYPE = "chargetype";
	private SensorManager mSensorManager = null;
	private Sensor mSensor = null;
	private Sensor orientationSensor=null;
	private float x, y, z;
	private boolean  mBIsSensor=false;
	public String m_appVersion = "";
	public String m_appLocalVersion = "";

	public static LayaConch5 ms_layaConche = null;

	private LayaVideoPlayer m_pLayaVideoPlayer;

	private Vector<LayaVideoPlayer> m_videoPlayers = new Vector<LayaVideoPlayer>();
    private boolean isToast = true;//是否弹吐司，为了保证for循环只弹一次
    private String saveImageToAlbumFilePath = "";

	public String 	m_strAlertTitle = "";
	public String  m_strOnBackPressed = "";
	public HttpProxyCacheServer httpProxyCacheServer;

	public static LayaConch5 GetInstance() {
		if (ms_layaConche == null) {
			ms_layaConche = new LayaConch5();
		}
		return ms_layaConche;
	}

	public LayaConch5() {
		ms_layaConche = this;
	}

	public void addVideoPlayer(LayaVideoPlayer player)
	{
		m_videoPlayers.add( player );
	}

	public void removeVideoPlayer(LayaVideoPlayer player)
	{
		m_videoPlayers.remove( player );
	}

	public  void setSensorAble(boolean a)
	{
		if(mBIsSensor!=a)
		{
			mBIsSensor=a;
			if(mBIsSensor){
				registerSensor();
			}
			else {
				unRegisterSensor();
			}
		}
	}

	private void registerSensor()
	{
		if(mSensorManager==null) {
			mSensorManager = (SensorManager)mCtx.getSystemService(SENSOR_SERVICE);
			mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
			orientationSensor=mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION);
		}
		mSensorManager.registerListener(lsn, orientationSensor, SensorManager.SENSOR_DELAY_NORMAL);
		mSensorManager.registerListener(lsn, mSensor, SensorManager.SENSOR_DELAY_NORMAL);
	}

	private void unRegisterSensor()
	{
		if(mSensorManager!=null) {
			mSensorManager.unregisterListener(lsn);
		}
	}

	public LayaConch5(Context ctx) {
		setContext(ctx);
		httpProxyCacheServer = new HttpProxyCacheServer.Builder(ctx).maxCacheFilesCount(25).build();

		Configuration configuration=ctx.getResources().getConfiguration();
		try {
			m_bHorizontalScreen = (configuration.screenWidthDp > configuration.screenHeightDp);
		}
		catch (NoSuchFieldError e)
		{
			WindowManager wm = (WindowManager) (mCtx
					.getSystemService(Context.WINDOW_SERVICE));
			DisplayMetrics dm = new DisplayMetrics();
			wm.getDefaultDisplay().getMetrics(dm);
			m_bHorizontalScreen= dm.widthPixels>dm.heightPixels;
		}
		m_pNetWorkReveiver = new NetworkReceiver();
		ms_layaConche = this;
		m_appVersion = getLocalVersionName(ctx);
		m_appLocalVersion = getLocalVersion(ctx);
	}

	public void setContext(Context ctx) {
		if (ctx == null)
			return;
		mCtx = ctx;
		m_pDevID = new DevID(ctx);
	}
	public float[] getRotatedValues(float values[]) {
		if (values == null || values.length != 3) {
			return values;
		}

		Display display =
				((WindowManager)mCtx.getSystemService(WINDOW_SERVICE)).getDefaultDisplay();
		int displayRotation = display.getRotation();

		float[] rotatedValues = new float[3];
		switch (displayRotation) {
			case Surface.ROTATION_0:
				rotatedValues[0] = -values[0];
				rotatedValues[1] = -values[2];
				rotatedValues[2] = values[1];
				break;
			case Surface.ROTATION_90:
				rotatedValues[0] = -values[0];
				rotatedValues[1] = -values[1];
				rotatedValues[2] = -values[2];
				break;
			case Surface.ROTATION_180:
				rotatedValues[0] = values[0];
				rotatedValues[1] = values[2];
				rotatedValues[2] = -values[1];
				break;
			case Surface.ROTATION_270:
				rotatedValues[0] = values[0];
				rotatedValues[1] = values[1];
				rotatedValues[2] = values[2];
				break;
		}

		return rotatedValues;
	}
	SensorEventListener lsn = new SensorEventListener() {
		@Override
		public void onSensorChanged(SensorEvent event) {
			float[] values = getRotatedValues(event.values);
		    int  a=	event.sensor.getType();
			x = values[0];
			y = values[1];
			z = values[2];
			if(a==Sensor.TYPE_ORIENTATION)
			{
				ConchJNI.handleDeviceOrientationEvent(x,y,z);
			}
			else {
				ConchJNI.handleDeviceMotionEvent(0f, 0f, 0f, x, y, z, 0f, 0f, 0f, 1f);
			}
		}
		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
			// TODO Auto-generated method stub
			int a=accuracy;
		}
	};

	public void onCreate() {
		// 监听网络
		Log.e(TAG, ">>>>>>>conchjar android-2.0.8");
		IntentFilter pFilter = new IntentFilter();
		pFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		try 
		{
			mCtx.registerReceiver(m_pNetWorkReveiver, pFilter);
		} 
		catch (Exception e) 
		{
			Log.i("0","registerReceiver error m_pNetWorkReveiver=" + m_pNetWorkReveiver );
		}
		Log.e(TAG, "plugin-----------------onCreate() ");
		ActivityManager am = (ActivityManager) mCtx.getSystemService(Context.ACTIVITY_SERVICE);
		ProcessInfo.init(am);
		InputStream in = getClass().getResourceAsStream("/assets/config.ini");
		config.GetInstance().init(in);

		boolean initedNative = false;
		if (!m_bIsPlug) {
			if (m_strSoPath.length() > 0) {
				String pluginPath = getSoPath() + m_strSoFile;// "libegret.so";
				initedNative = ConchJNI.initNativeLibrary(pluginPath, true);
				if (!initedNative) {
					throw new RuntimeException("Failed to load native runtime library");
				}
			} else {
				initedNative = ConchJNI.initNativeLibrary("conch", false);
				if (!initedNative) {
					throw new RuntimeException("Failed to load native runtime library");
				}
			}
		} else {
			String pluginPath = getSoPath() + m_strSoFile;// "libegret.so";
			initedNative = ConchJNI.initNativeLibrary(pluginPath, true);
			if (!initedNative) {
				throw new RuntimeException("Failed to load native runtime library");
			}
		}
		// 先配置一下
		ConchJNI.configSetIsPlug(m_bIsPlug);
		if (m_strUrl.length() > 0) {
			ConchJNI.configSetURL(m_strUrl);
		}
		ExportJavaFunction.m_nState = 0;
		ExportJavaFunction expjava = ExportJavaFunction.GetInstance();
		expjava.m_pEngine = this;
		expjava.Init(mCtx);
		String _marketName = getMarketBundle().getString(MARKET_MARKETNAME);

		PlatformInitOK(0);
		mChoreographer = Choreographer.getInstance();
		mChoreographer.postFrameCallback(this);
	}
	public void PlatformInitOK(int p_nFlag) {
		Log.e("0", "==============Java流程 InitMainCanvas()");
		EngineStart();
	}

	@Override
	public void doFrame(long frameTimeNanos) {
		mChoreographer.postFrameCallback(this);
		if (m_pCavans != null && m_pCavans.mIsReady) {
			ConchJNI.onDrawFrame();
		}
	}

	static class CacheInfo{
		int id;
		String path;
	}
	//cachePath 需要到 appCache 那一层。
	public static Vector<CacheInfo> getCachedApp(String cachePath){
		Vector<CacheInfo> vecFile = new Vector<CacheInfo>();
		File file = new File(cachePath);
		File[] subFile = file.listFiles();

		for (File aSubFile : subFile) {
			CacheInfo cinfo = new CacheInfo();
			// 判断是否为文件夹
			if (aSubFile.isDirectory()) {
				String filename = aSubFile.getName();
				//vecFile.add(filename);
				cinfo.path = cachePath + "/" + filename;
				if (filename.compareTo("sessionFiles") == 0) {
					//vecFile.add("-1");//临时缓存
					cinfo.id = -1;
				} else {
					//找有没有id
					String appidfile = cachePath + "/" + filename + "/sourceid/appid";
					File appidFile = new File(appidfile);
					if (appidFile.exists()) {
						try {
							BufferedReader br = new BufferedReader(new FileReader(appidfile));
							String data = br.readLine();
							//vecFile.add(data);
							cinfo.id = Integer.parseInt(data);
						} catch (Exception e) {
							//vecFile.add("-2");//无法获得
							cinfo.id = -2;
						}
					} else {
						//vecFile.add("-3");//无法获得
						cinfo.id = -3;
					}
				}
			}
			vecFile.add(cinfo);
		}
        return vecFile;		
	}
	/**
	 * 删除一个目录。
	 * @param file
	 */
	public static void deletePath(File file) {
		Log.e("2jni", "cacheMgr delete dir: " + file.toString());
		if (file.isFile()) {
			file.delete();
			return;
		}

		if (file.isDirectory()) {
			File[] childFiles = file.listFiles();
			if (childFiles == null || childFiles.length == 0) {
				file.delete();
				return;
			}

			for (int i = 0; i < childFiles.length; i++) {
				deletePath(childFiles[i]);
			}
			file.delete();
		}
	}

	//TEST
	public void EngineStart() {
		String strLayaCache = getAppCacheDir() + "/LayaCache";
		File cacheFolder = new File(strLayaCache);
		if (!cacheFolder.exists()) {
			cacheFolder.mkdir();
		}
		Log.e(TAG, "plugin-----------------EngineStart() = " + (strLayaCache + "/localstorage"));
		// 准备localStorage目录
		File localStoragePath = new File(strLayaCache + "/localstorage");
		if (!localStoragePath.exists()) {
			if (!localStoragePath.mkdirs()) {
				Log.e("", "创建localStorage目录失败！");
				ExportJavaFunction.alert("创建游戏目录失败，请清理空间或重启应用再试");
				game_plugin_exitGame();
				return;
			}
		}
		ConchJNI.SetLocalStoragePath(strLayaCache + "/localstorage");
		ConchJNI.SetTmpCacheSpaceThreshold(config.GetInstance().m_nTmpCacheSpaceThreshold);
		ConchJNI.SetTmpCacheTimeThreshold(config.GetInstance().m_nTmpCacheTimeThreshold);
		ConchJNI.SetConchWebgl(config.GetInstance().m_bConchWebGL);

		String cachePath = getAppCacheDir() + "/LayaCache";
		if (m_AM != null) {
			ConchJNI.InitDLib(m_AM,getDownloadThreadNum(), "cache", cachePath, m_strExpansionMainPath == null ? "" : m_strExpansionMainPath,m_strExpansionPatchPath == null ? "" : m_strExpansionPatchPath, config.GetInstance().m_nThreadMode,config.GetInstance().m_nDebugMode,config.GetInstance().m_nDebugPort,config.GetInstance().m_bUseDcc, m_strConfigJS);
		}
		else {
			ConchJNI.InitDLib(null,getDownloadThreadNum(), getJarFile(), cachePath, m_strExpansionMainPath == null ? "" : m_strExpansionMainPath,m_strExpansionPatchPath == null ? "" : m_strExpansionPatchPath, config.GetInstance().m_nThreadMode,config.GetInstance().m_nDebugMode,config.GetInstance().m_nDebugPort,config.GetInstance().m_bUseDcc, m_strConfigJS);
		}
		InitView();
	}

	public  boolean isOpenNetwork()
	{
		if (!config.GetInstance().m_bCheckNetwork)
			return true;
		ConnectivityManager connManager = (ConnectivityManager) mCtx.getSystemService(Context.CONNECTIVITY_SERVICE);
		return connManager.getActiveNetworkInfo() != null && (connManager.getActiveNetworkInfo().isAvailable() && connManager.getActiveNetworkInfo().isConnected());
	}

	// 设备控制相关
	public void setScreenOrientation(int ori) {
		// IGameApp.setScreenOrientation(ori);
	}

	public void setScreenWakeLock( boolean p_bWakeLock ) 
	{
		try
		{
			Log.i("0",">>>>>>screenWakeLock wake=" + p_bWakeLock );
			Activity activity = (Activity)(mCtx);
			if(activity==null)
			{
				return;
			}
			if( p_bWakeLock )
			{
				Log.i("0",">>>>>>screenWakeLock ok" + p_bWakeLock );
				activity.getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
			}
			else
			{
				Log.i("0",">>>>>>screenWakeLock ok" + p_bWakeLock );
				activity.getWindow().clearFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
			}
		}
		catch( Exception e )
		{
			Log.i("0",">>>>>>screenWakeLock error" );
		}
	}

	@Override
	public boolean onKey(View v, int keyCode, KeyEvent event)
	{
		Log.e("input", ">>>>>>>>>>>>>>>>" + keyCode);
		if(event.getAction() == KeyEvent.ACTION_DOWN)
		{
			ConchJNI.handleKeyEvent(keyCode, KeyEvent.ACTION_DOWN);
		}
		else if(event.getAction() == KeyEvent.ACTION_UP)
		{
			ConchJNI.handleKeyEvent(keyCode, KeyEvent.ACTION_UP);
		}
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp!=null&&exp.m_pEngine.getIsPlug() ){
			//此处方便js接管Back事件，鉴于qq浏览器在没有初始化完成的时候就调用了back造成非法现予以屏蔽
			return false;
		}
		else
		{
			Log.e("","exp is null");
		}
		/*if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN)
		{
			Log.e("", "onKey = " + keyCode);
			if (!ConchJNI.onBackPressed())
			{
				long curtm = System.currentTimeMillis();
				int MaxDelay = 3500; // Toast.LENGTH_LONG 对应的时间
				if (m_nBackPressTime == 0 || (m_nBackPressTime > 0 && curtm - m_nBackPressTime > MaxDelay))
				{
					m_nBackPressTime = System.currentTimeMillis();
					Activity context=(Activity)mCtx;
					if (isFinishing(context))return true;
					Toast.makeText(mCtx, m_strOnBackPressed, Toast.LENGTH_LONG).show();
				}
				else
				{
					// 退出
					game_plugin_exitGame();
				}
				return false;
			}
		}*/
		return keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_UP;
	}

	@Override
	public void onKeyEvent(String keyEvent,int keyCode) {
		if( "onKeyUp".equals(keyEvent) ){
			ConchJNI.handleKeyEvent(keyCode, KeyEvent.ACTION_UP);
		}else if("onKeyDown".equals(keyEvent)){
			ConchJNI.handleKeyEvent(keyCode, KeyEvent.ACTION_DOWN);
		}
	}
	@SuppressLint("NewApi") @TargetApi(Build.VERSION_CODES.HONEYCOMB)
	void InitView() {
		m_pCavans = new ConchSurfaceView(mCtx);
		if (m_pAbsLayout == null) {
			m_pAbsLayout = new AbsoluteLayout(this.mCtx);
			m_pAbsLayout.setBackgroundColor(0x00ffffff);
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB){
				m_pAbsLayout.setOnGenericMotionListener(new OnGenericMotionListener() {
					@Override
					public boolean onGenericMotion(View arg0, MotionEvent event) {
						// TODO Auto-generated method stub
						//if( MotionEvent.TOOL_TYPE_UNKNOWN == event.getToolType(0))
						int eventSource = event.getSource();
						if( (eventSource & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK
		                && event.getAction() == MotionEvent.ACTION_MOVE)
						{
							/* 获取左摇杆值 */
							float leftX = event.getAxisValue(MotionEvent.AXIS_X);
							float leftY = event.getAxisValue(MotionEvent.AXIS_Y);
							/* 获取右摇杆值 */
							float z = event.getAxisValue(MotionEvent.AXIS_Z);
							float rz = event.getAxisValue(MotionEvent.AXIS_RZ);

							float lt = event.getAxisValue(MotionEvent.AXIS_LTRIGGER);
							float rt = event.getAxisValue(MotionEvent.AXIS_RTRIGGER);
							ConchJNI.handleJoystickEvent(leftX, leftY, z, rz, lt, rt);
						}
						return false;
					}
				});
			}
			m_pAbsLayout.setOnKeyListener(this);
		}
		int uiParm=0;
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			uiParm=View.SYSTEM_UI_FLAG_FULLSCREEN;
		}
		if(uiParm!=0)m_pAbsLayout.setSystemUiVisibility(uiParm);

		RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);

		m_pCavans.setLayoutParams(lp);

		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics dm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(dm);
		if (m_bHorizontalScreen && dm.heightPixels > dm.widthPixels
				|| !m_bHorizontalScreen && dm.widthPixels > dm.heightPixels) {
			int tmp = dm.heightPixels;
			dm.heightPixels = dm.widthPixels;
			dm.widthPixels = tmp;
		}

		m_pAbsLayout.addView(m_pCavans);
		if (m_pLayaWebView == null) {
			try
			{
				m_pLayaWebView = new LayaWebView(mCtx, this);
				if( config.GetInstance().m_sWebviewUrl != null ) {
					m_pLayaWebView.showWebView( config.GetInstance().m_sWebviewUrl, 0, 0, GetScreenWidth(), GetScreenHeight());
				}
			}
			catch(Exception e)
			{
				Log.e("LayaBoxwebView", ">>>>>>>>>>>>>>Exception"+e.toString());
			}
			catch(Throwable e)
			{
				Log.e("LayaBoxwebView", ">>>>>>>>>>>>>>throwable"+e.toString());
			}
		}

		if (m_pEditBox == null) {

			m_pEditBoxLayout = new AbsoluteLayout(mCtx);
			m_pEditBoxLayout.setBackgroundColor(0x00000000);

			m_pEditBox = new LayaEditBox(mCtx);
			m_pEditBox.CreateEditBox();

			m_pEditBoxLayout.addView(m_pEditBox.m_pEditBox,
					new AbsoluteLayout.LayoutParams(GetScreenWidth(), GetScreenHeight(), 0, 0));
			m_pAbsLayout.addView(m_pEditBoxLayout, new ViewGroup.LayoutParams(
					ViewGroup.LayoutParams.WRAP_CONTENT,
					ViewGroup.LayoutParams.WRAP_CONTENT));
		}

		LayaVideoPlayer.SetDefaultVideoContext(mCtx, m_pAbsLayout);
		//IVideoPlayer videoPlayer = new IVideoPlayer(mCtx, m_pAbsLayout);
		//videoPlayer.setDataSource("http://10.10.20.31:7788/res/av/mov_bbb.mp4"); //test url

//		if(m_pLayaVideoPlayer == null)
//		{
//			m_pLayaVideoPlayer = new LayaVideoPlayer(mCtx, m_pAbsLayout);
//			m_pLayaVideoPlayer.setX(700);
//			m_pLayaVideoPlayer.setY(700);
//			m_pLayaVideoPlayer.setWidth(300);
//			m_pLayaVideoPlayer.setHeight(500);
//			m_pLayaVideoPlayer.Load("https://cdn3.maxuscloud.com/wgame2/jjcgame/shipin/sp01.mp4");
//			m_pLayaVideoPlayer.Play();
//		}

		if (m_interceptKey) {
			Log.i("layaConch5", "m_interceptKey OK!");
			m_pAbsLayout.setFocusable(true);
			m_pAbsLayout.setFocusableInTouchMode(true);
			m_pAbsLayout.requestFocus();
		}
	}

    //获取是否存在NavigationBar
    public static boolean checkDeviceHasNavigationBar(Context context) {
        boolean hasNavigationBar = false;
        try {
            Resources rs = context.getResources();
            int id = rs.getIdentifier("config_showNavigationBar", "bool", "android");
            if (id > 0) {
                hasNavigationBar = rs.getBoolean(id);
            }
            Class systemPropertiesClass = Class.forName("android.os.SystemProperties");
            Method m = systemPropertiesClass.getMethod("get", String.class);
            String navBarOverride = (String) m.invoke(systemPropertiesClass, "qemu.hw.mainkeys");
            if ("1".equals(navBarOverride)) {
                hasNavigationBar = false;
            } else if ("0".equals(navBarOverride)) {
                hasNavigationBar = true;
            }
        } catch (Exception e) {

        }

        return hasNavigationBar;
    }

    /*
    private ContentObserver mNavigationStatusObserver = new ContentObserver(null) {
        @Override
        public void onChange(boolean selfChange) {
            int navigationBarIsMin = Settings.System.getInt(getGameContext().getContentResolver(),
                    "navigationbar_is_min", 0);
            if (navigationBarIsMin == 1) {
                //导航键隐藏了
            } else {
                //导航键显示了
            }

        }
    };
    */

	/**
	 * 判断activity是否处于可用状态
	 * @param context
	 * @return
	 */
	private boolean isFinishing(Activity activity)
	{
		if(activity==null||activity.isFinishing())
			return true;
		return false;
	}

	public void showMessage(String pMessage)
	{
		Activity context= (Activity) (mCtx);
		if(isFinishing(context))return;
		Toast mToast = Toast.makeText(context, pMessage, Toast.LENGTH_LONG);
		mToast.show();
	}

	public void alertJS(String title, String msg,final int p_nCallbackType)
	{
		//TODO 不知道为什么，插件情况下传入ctx就行，但是直接执行必须用Activity对象。
		Activity context= (Activity) (mCtx);
		if(isFinishing(context))return;
		AlertDialog.Builder pBuilder = new AlertDialog.Builder(context);
		pBuilder.setTitle(title);
		pBuilder.setMessage(msg);
		pBuilder.setCancelable(false);
		// 退出按钮
		pBuilder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface p_pDialog, int arg1) {
				p_pDialog.cancel();
				if (p_nCallbackType == 1) {
					ConchJNI.alertCallback();
				} else if (p_nCallbackType == 2) {
					game_plugin_exitGame();
				}
			}
		});
		AlertDialog alertdlg = pBuilder.create();
		alertdlg.setCanceledOnTouchOutside(false);
		alertdlg.show();
	}

	int m_iScreenWidth = 0;
	int m_iScreenHeight = 0;

	// ------------------------------------------------------------------------------
	public int GetScreenWidth() {
		// if (m_iScreenWidth != 0)
		// return m_iScreenWidth;
		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics pDm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(pDm);
		m_iScreenWidth = pDm.widthPixels;
		return m_iScreenWidth;
	}

	// ------------------------------------------------------------------------------
	public int GetScreenHeight() {
		// if (m_iScreenHeight != 0)
		// return m_iScreenHeight;
		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics pDm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(pDm);
		m_iScreenHeight = pDm.heightPixels;
		return m_iScreenHeight;
	}

	// ------------------------------------------------------------------------------

	public void game_conch3_setAssetInfo(AssetManager am) {
		m_AM = am;
	}

	public void game_conch3_SetIsPlug(boolean b) {
		m_bIsPlug = b;
	}

	public void game_conch3_setAppWorkPath(String runpath) {// 在这里保存运行数据。
		m_strCachePath = runpath;
	}

	public AbsoluteLayout getAbsLayout() {
		return m_pAbsLayout;
	}

	public DevID getDevID() {
		return m_pDevID;
	}
	public String getIPAddress()
	{
		NetworkInfo info = ((ConnectivityManager) mCtx
				.getSystemService(Context.CONNECTIVITY_SERVICE)).getActiveNetworkInfo();
		if (info != null && info.isConnected()) {
			if (info.getType() == ConnectivityManager.TYPE_MOBILE) {    // 当前使用2G/3G/4G网络
				try {
					for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); ((Enumeration<?>) en).hasMoreElements(); ) {
						NetworkInterface intf = en.nextElement();
						for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements(); ) {
							InetAddress inetAddress = enumIpAddr.nextElement();
							if (!inetAddress.isLoopbackAddress() && inetAddress instanceof Inet4Address) {
								return inetAddress.getHostAddress();
							}
						}
					}
				} catch (SocketException e) {
					e.printStackTrace();
				}

			} else if (info.getType() == ConnectivityManager.TYPE_WIFI) {    // 当前使用无线网络
				WifiManager wifiManager = (WifiManager) mCtx.getSystemService(Context.WIFI_SERVICE);
				WifiInfo wifiInfo = wifiManager.getConnectionInfo();
				String ipAddress = intIP2StringIP(wifiInfo.getIpAddress());    // 得到IPV4地址
				return ipAddress;
			}
		} else {
			// 当前无网络连接,请在设置中打开网络
		}
		return "";
	}
	/**
	 * 将得到的int类型的IP转换为String类型
	 *
	 * @param ip
	 * @return
	 */
	public static String intIP2StringIP(int ip) {
		return (ip & 0xFF) + "." +
				((ip >> 8) & 0xFF) + "." +
				((ip >> 16) & 0xFF) + "." +
				(ip >> 24 & 0xFF);
	}
	public ConchSurfaceView getCanvas() {
		return m_pCavans;
	}

	public boolean getIsPlug() {
		return m_bIsPlug;
	}

	public LayaEditBox getEditBox() {
		return m_pEditBox;
	}

	public LayaWebView getWebView() {
		return m_pLayaWebView;
	}

	public Context getGameContext() {
		return mCtx;
	}

	public boolean getHorizontalScreen() {
		return m_bHorizontalScreen;
	}

	public void onPause() 
	{
		mChoreographer.removeFrameCallback(this);

		for (LayaVideoPlayer video :  m_videoPlayers) {
			video.onPause();
		}
		if(mBIsSensor)unRegisterSensor();
		ConchJNI.OnAppPause();//ui thread
	}

	public NetworkReceiver getNetworkReceiver() {
		return m_pNetWorkReveiver;
	}
	public String getLanguage() {
		Locale locale = mCtx.getResources().getConfiguration().locale;
		return locale.getLanguage();
	}
	public void setLanguage(String language) {
		Log.e(TAG, "setLanguage " + language);
		Locale locale = null;
		switch(language) {
			case "zh":
				locale = Locale.SIMPLIFIED_CHINESE;
				break;
			case "en":
				locale = Locale.ENGLISH;
				break;
			case "in":
				locale = new Locale("in", "");
				break;
			case "pt":
				locale = new Locale("pt", "");
				break;
			default:
				locale = Locale.SIMPLIFIED_CHINESE;
				break;

		}
		Resources resources = mCtx.getResources();
		Locale.setDefault(locale);
		Configuration config = resources.getConfiguration();
		DisplayMetrics dm = resources.getDisplayMetrics();
		config.locale = locale;
		resources.updateConfiguration(config, dm);
	}
	// ------------------------------------------------------------------------------
	public void onResume() 
	{
		mChoreographer.postFrameCallback(this);

		for (LayaVideoPlayer video :  m_videoPlayers) {
			video.onResume();
		}
		if(mBIsSensor)registerSensor();
		ConchJNI.OnAppResume();//ui thread
	}
	public void onStop() {
	}

	public void onDestroy() {

		ProcessInfo.uninit();
		try{
			mCtx.unregisterReceiver(m_pNetWorkReveiver);
		} 
		catch (Exception e){
			Log.i("0","unregisterReceiver error m_pNetWorkReveiver=" + m_pNetWorkReveiver );
		}
		delInstance();

		if(m_pCavans!=null){
			m_pCavans.destory();
		}
		else{
			Log.e("Canvas", ">>>>>onDestroy m_pCavans is null");
		}

		LayaAudioMusic.uninit();
		destory();

		ConchJNI.ReleaseDLib();
	}
	@SuppressLint("NewApi") @TargetApi(Build.VERSION_CODES.HONEYCOMB)
	private void destory()
	{
		mChoreographer.removeFrameCallback(this);
		if(m_pAbsLayout!=null)
			m_pAbsLayout.removeAllViews();
		m_pAbsLayout.setOnKeyListener(null);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB){
			m_pAbsLayout.setOnGenericMotionListener(null);
		}
		m_pAbsLayout=null;
		if(m_pCavans!=null) {
			m_pCavans.destory();
			m_pCavans = null;
		}
		ms_layaConche = null;
		m_marketBundle=null;

		if(m_pLayaWebView!=null)
		{
			m_pLayaWebView.destory();
			m_pLayaWebView=null;
		}
		if(m_pEditBoxLayout!=null)
		{
			m_pEditBoxLayout.removeAllViews();
		    m_pEditBoxLayout = null;
		}
		if(m_pEditBox!=null)
		{
			m_pEditBox.destory();
			m_pEditBox=null;
		}
		mCtx=null;
		m_layaEventListener=null;
	}

	public void delInstance()
	{
		ExportJavaFunction.DelInstance();
		config.DelInstance();
		if(m_layaEventListener!=null) {
			m_layaEventListener.destory();
		}
	}



	@Override
	public void onNewIntent(Intent intent) {
	}

	@Override
	public void onRestart() {
		for (LayaVideoPlayer video :  m_videoPlayers) {
			video.onRestart();
		}
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent intent) {
		Log.d(TAG, "onActivityResult" );
		/*结果回调*/
		switch (requestCode) {
			case Constants.REQUEST_SELECT_REQUEST_CODE:
			case Constants.REQUEST_CAMERA_ACTIVITY:
				CallbackRes res = new CallbackRes();
				ArrayList<CallbackRes.TempFiles> tempFiles = new ArrayList<>();
				Gson gson = new Gson();
				if (intent != null) {
					Log.i(TAG, "onActivityResult: ");
					ArrayList<String> resultList = intent.getStringArrayListExtra(MultiImageSelectorActivity.EXTRA_RESULT);
					ArrayList<Integer> sizeList = intent.getIntegerArrayListExtra(MultiImageSelectorActivity.EXTRA_RESULT_SIZE);
					ArrayList<String> tempList = new ArrayList<>();
					Log.d(TAG, "onActivityResult: size" + resultList.size());
					if (resultList != null && resultList.size() > 0) {
						for (int i = 0; i < resultList.size(); i++) {
							String path = resultList.get(i);
							String md5 = Utils.md5(path).toLowerCase();
							String tmpFileName = md5 + Utils.getExtension(path);
							String tempPath = ConchJNI.GetLocalTempCachePath() + "/" + tmpFileName;
							int size = sizeList.get(i);
							Log.d(TAG, "onActivityResult: path " + path);
							Log.d(TAG, "onActivityResult: md5 " + md5);
							Log.d(TAG, "onActivityResult: files --------" + ConchJNI.GetLocalTempCachePath());
							CallbackRes.TempFiles files = new CallbackRes.TempFiles();
							ImageUtils.compressImage(path, tempPath);
							tempList.add("wxfile://tmp/" + tmpFileName);
							files.setPath("wxfile://tmp/" + tmpFileName);
							files.setSize(size);
							tempFiles.add(files);
							Log.d(TAG, "onActivityResult: files " + files.getPath());
							Log.d(TAG, "onActivityResult: files " + files.getSize());
						}
						Log.d(TAG, "onActivityResult: files --------" + ConchJNI.GetLocalTempCachePath());
//						res.setTempFilePaths(resultList);
						res.setTempFilePaths(tempList);
						res.setTempFiles(tempFiles);
						String json = gson.toJson(res);
						Log.d(TAG, "onActivityResult: json " + json);
						ConchJNI.onChooseImageComplete(1, json);
					} else {
						String json = gson.toJson(res);
						ConchJNI.onChooseImageComplete(0, json);
					}
				} else {
					String json = gson.toJson(res);
					ConchJNI.onChooseImageComplete(0, json);
				}
			break;
			default:
				break;
		}
	}

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        boolean isPermissions = true;
        for (int i = 0; i < permissions.length; i++) {
            if (grantResults[i] == PackageManager.PERMISSION_DENIED) {
                isPermissions = false;
                if (!ActivityCompat.shouldShowRequestPermissionRationale((Activity) mCtx, permissions[i])) { //用户选择了"不再询问"
                    if (isToast) {
                        Toast.makeText(mCtx, "请手动打开权限", Toast.LENGTH_SHORT).show();
                        isToast = false;
                    }
                }
            }
        }
        isToast = true;
        if (isPermissions) {
            if (requestCode == Constants.REQUEST_PERMISSION_CODE_SAVE_IMAGE) {
                saveImageToPhotosAlbum(saveImageToAlbumFilePath);
            }
            else if (requestCode == Constants.REQUEST_PERMISSION_CODE_CAMERA) {
				((Activity)mCtx).startActivityForResult(new Intent(mCtx, CameraActivity.class), Constants.REQUEST_CAMERA_ACTIVITY);
				Log.d(TAG, "onRequestPermissionsResult");
			}
        } else {
            Log.d(TAG, "onRequestPermissionsResult: PERMISSION_DENIED");
            if (requestCode == Constants.REQUEST_PERMISSION_CODE_SAVE_IMAGE) {
                ConchJNI.onSaveImageToPhotosAlbumComplete(0);
                saveImageToAlbumFilePath = "";
            }
        }
    }


    public void game_plugin_onUrlBack() {
		ConchJNI.onRunCmd(0x400 + 3333 + 104, -1, 0);
	}

	public void game_plugin_onUrlRefresh() {
		ConchJNI.onRunCmd(0x400 + 3333 + 102, 0, 0);
	}

	public int game_plugin_getTouchMovRange() {
		return 10;
	}

	public void setLocalizable(boolean b)
	{
		localizable=b;
		//ConchJNI.onSendToDesktop("sfsdfsadfsadf");
	}

	public void setGameUrl(String _param) {
		m_strUrl = _param;
	}
	
	public void setParamExt(String _param){
		m_strExt = _param;
	}
	
	public void setRuntimeExt()
	{
		if (m_strExt.length() > 0) {
			ConchJNI.configSetParamExt(m_strExt);
		}
	}
	
	public void setSoFile(String _param) {
		m_strSoFile = _param;
	}

	public void setSoPath(String _param) {
		m_strSoPath = _param;
	}

	public void setJarFile(String _param) {
		m_strJarFile = _param;
	}

	public void setAppCacheDir(String _param) {
		m_strCachePath = _param;
	}

	public void setDownloadThreadNum(int nNum){ m_nDownloadThreadNum = nNum;}

	public void setExpansionZipDir( final String mainPath,final String patchPath )
	{
		m_strExpansionMainPath = mainPath;
		m_strExpansionPatchPath = patchPath;
	}
	public void setConfigJS(final String js)
	{
		m_strConfigJS = js;
	}

	// 获得so文件路径
	public String getSoPath() {
		return m_strSoPath;
	}

	// 获得jar文件
	public String getJarFile() {
		return m_strJarFile;
	}

	// 获得游戏缓存目录
	public String getAppCacheDir() {
		return m_strCachePath;
	}

	public int getDownloadThreadNum()
	{
		return m_nDownloadThreadNum;
	}

	// 游戏主动退出游戏
	public void game_plugin_exitGame() {
		if (m_layaEventListener != null)
			m_layaEventListener.ExitGame();
	}
	public void game_plugin_finish() {
		if (m_layaEventListener != null)
			m_layaEventListener.Finish();
	}

	// 游戏插件初始化
	public void game_conch3_init() {
		this.onCreate();
	}

	// 获得游戏需要显示的view
	public View game_conch3_get_view() {
		return m_pAbsLayout;
	}

	// 进入后台时调用
	public void game_conch3_onPause() {
		this.onPause();
	}

	// 恢复前台时调用
	public void game_conch3_onResume() {
		this.onResume();
	}

	// 退出游戏时调用
	public void game_conch3_onStop() {
		this.onStop();
	}

	@Override
	public void setLayaEventListener(ILayaEventListener _lis) {
		m_layaEventListener = _lis;

	}

	@Override
	public void setInterceptKey(boolean _intercept) {
		m_interceptKey = _intercept;
		setGameFocus();
	}

	public boolean getInterceptKey() {
		return m_interceptKey;
	}

	public void setGameFocus() {
		if (m_interceptKey) {
			Log.i("layaConch3", "m_interceptKey OK!");
			if (m_pAbsLayout == null) {
				return;
			}
			m_pAbsLayout.setFocusable(true);
			m_pAbsLayout.setFocusableInTouchMode(true);
			m_pAbsLayout.requestFocus();
			m_pAbsLayout.setOnKeyListener(this);
		}
	}

	static public Bundle getMarketBundle(){
		if(m_marketBundle == null){
			m_marketBundle = new Bundle();
			return m_marketBundle;
		}
		return m_marketBundle;
	}
	static public void setMarketBundle( Bundle bundle){
		m_marketBundle = bundle;
	}

    @Override
    public void setAssetInfo(AssetManager am) {
        game_conch3_setAssetInfo(am);        
    }

    @Override
    public void setIsPlugin(boolean isPlugin) {
        game_conch3_SetIsPlug(isPlugin);
    }

	public static String getLocalVersion(Context ctx) {
		int localVersion = 0;
		try {
			PackageInfo packageInfo = ctx.getApplicationContext()
					.getPackageManager()
					.getPackageInfo(ctx.getPackageName(), 0);
			localVersion = packageInfo.versionCode;
		} catch (PackageManager.NameNotFoundException e) {
			e.printStackTrace();
		}
		return Integer.toString(localVersion);
	}
	public static String getLocalVersionName(Context ctx) {
		String localVersion = "";
		try {
			PackageInfo packageInfo = ctx.getApplicationContext()
					.getPackageManager()
					.getPackageInfo(ctx.getPackageName(), 0);
			localVersion = packageInfo.versionName;
		} catch (PackageManager.NameNotFoundException e) {
			e.printStackTrace();
		}
		return localVersion;
	}
	@Override
	public void setAlertTitle(String title){
		m_strAlertTitle = title;
	}
	@Override
	public void  setStringOnBackPressed(String str) {
		m_strOnBackPressed = str;
	}

	public void chooseImage(int count, String sizeType, String sourceType) {
		Activity activity = (Activity) mCtx;
		if (sourceType.equals("camera")) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
				boolean hasPermission = Utils.checkPermission(activity, CameraActivity.REQUIRED_PERMISSIONS, Constants.REQUEST_PERMISSION_CODE_CAMERA);
				if (hasPermission) {
					activity.startActivityForResult(new Intent(activity, CameraActivity.class), Constants.REQUEST_CAMERA_ACTIVITY);
				}
			}
		} else {

			if(sizeType != null && sourceType != null) {
				Intent intent = new Intent(activity, MultiImageSelectorActivity.class);
				intent.putExtra("count", count);
				intent.putExtra("sizeType", sizeType);
				intent.putExtra("sourceType", sourceType);
				Log.d("chooseImage", "chooseImage: " + count);
				Log.d("chooseImage", "chooseImage: " + sizeType);
				Log.d("chooseImage", "chooseImage: " + sourceType);
				activity.startActivityForResult(intent, Constants.REQUEST_SELECT_REQUEST_CODE);
			}
		}
	}

	public void beforeSaveImageToPhotosAlbum(String filePath) {
		Log.d("beforeSaveImage", ": " + filePath);
        saveImageToAlbumFilePath = filePath;
        boolean hasPermission = Utils.checkPermission(mCtx, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE}, Constants.REQUEST_PERMISSION_CODE_SAVE_IMAGE);
        if (hasPermission) {
            saveImageToPhotosAlbum(saveImageToAlbumFilePath);
        }
    }

	public void saveImageToPhotosAlbum(String filePath) {
		Log.d("saveImageToPhotosAlbum", "saveImageToPhotosAlbum: " + filePath);
		boolean result = false;
		try {
			FileInputStream fis = new FileInputStream(filePath);
			result = insertToAlbum(fis, ImageUtils.getMimeType(filePath), Utils.getExtension(filePath));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		if (result) {
			ConchJNI.onSaveImageToPhotosAlbumComplete(1);
		} else {
			ConchJNI.onSaveImageToPhotosAlbumComplete(0);
		}
        saveImageToAlbumFilePath = "";
	}

	private boolean insertToAlbum(InputStream inputStream, String type, String suffix) {
		Log.d(TAG, "insertToAlbum: ");
		if (inputStream == null)
			return false;

		boolean result = false;
		String fileName = System.currentTimeMillis() + suffix;
		ContentValues contentValues = new ContentValues();
		contentValues.put(MediaStore.Images.ImageColumns.DISPLAY_NAME, fileName);
		if (type != null && !type.equals("")) {
			contentValues.put(MediaStore.Images.Media.MIME_TYPE, type);
		}

		//插入相册图片表
		Uri uri;
		if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) { //SD 卡是否可用，可用则用 SD 卡，否则用内部存储
			uri = mCtx.getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, contentValues);
			Log.d(TAG, "insertToAlbum: EXTERNAL_CONTENT_URI uri " + uri);
		} else {
			uri = mCtx.getContentResolver().insert(MediaStore.Images.Media.INTERNAL_CONTENT_URI, contentValues);
			Log.d(TAG, "insertToAlbum: INTERNAL_CONTENT_URI uri " + uri);
		}
		if (uri != null) {
			//写入文件
			result = writeToFile(uri, inputStream);
		}
		Log.d(TAG, "insertToAlbum: result " + result);
		return result;
	}

	private boolean writeToFile(Uri uri, InputStream inputStream) {
		Log.d(TAG, "writeToFile: ");
		if (uri == null || inputStream == null)
			return false;

		boolean result;
		try {
			OutputStream outputStream = mCtx.getContentResolver().openOutputStream(uri);
			byte[] in = new byte[1024];
			int len = 0;
			do {
				len = inputStream.read(in);
				if (len != -1) {
					outputStream.write(in, 0, len);
					outputStream.flush();
				}
			} while (len != -1);
			result = true;
			inputStream.close();
			outputStream.close();
		} catch (Exception e) {
			Log.d(TAG, e.getLocalizedMessage());
			result = false;
		}
		return result;
	}
}
