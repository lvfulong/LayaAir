package demo;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.util.DisplayMetrics;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;
import java.util.Locale;

import layaair.game.browser.ConchJNI;
import layaair.game.browser.ExportJavaFunction;

import androidx.core.app.NotificationManagerCompat;

public class LayaApp {
    private static final String TAG = "LayaApp";
    public static Handler m_Handler = new Handler(Looper.getMainLooper());
    public static Activity mMainActivity = null;
    private static LayaApp _instance;

    private static Context context;

    public static void init(Context _context){
        context = _context;


    }

    public static void onDestroy() {
        m_Handler.removeCallbacksAndMessages(null);
    }

}
