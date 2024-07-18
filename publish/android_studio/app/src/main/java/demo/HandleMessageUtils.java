package demo;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;


public class HandleMessageUtils {
    private static final String LOG_TAG = "HandleMessageUtils";
    public static String handleSyncMessage(String eventName, String data) {
        Log.d(LOG_TAG, eventName +" " + data);
        return "sync result";
    }
    public static void handleAsyncMessage(String eventName, String data) {
    }
}
