package demo;

import android.util.Log;
import layaair.game.browser.HandleMessageCallback;


public class HandleMessageUtils {
    private static final String LOG_TAG = "HandleMessageUtils";
    public static String handleSyncMessage(String eventName, String data) {
        Log.d(LOG_TAG, eventName +" " + data);
        return "sync result";
    }
    public static void handleAsyncMessage(String eventName, String data, HandleMessageCallback cb) {
        Log.d(LOG_TAG, eventName +" " + data);
        cb.callback("async result");
    }
}
