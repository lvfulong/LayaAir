package demo.layaPlayer;

import static android.content.Context.CONNECTIVITY_SERVICE;

import android.content.Context;
import android.net.ConnectivityManager;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.google.zxing.integration.android.IntentIntegrator;


import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import layaair.game.browser.ConchJNI;
import layaair.game.browser.ExportJavaFunction;



public class JSBridge {
    public static Handler m_Handler = new Handler(Looper.getMainLooper());
    public static void showFloatPanel(final boolean show) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        if (demo.JSBridge.mMainActivity.mFloatPanel != null) {
                            if (show) {
                                demo.JSBridge.mMainActivity.mFloatPanel.show();
                            } else {
                                demo.JSBridge.mMainActivity.mFloatPanel.hide();
                            }
                        }
                    }
                });
    }

    public static void showScanner(final boolean show) {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        if (demo.JSBridge.mMainActivity.mFloatPanel != null) {
                            if (show) {
                                new IntentIntegrator(demo.JSBridge.mMainActivity).setCaptureActivity(ScanActivity.class).initiateScan();
                            } else {
                                if (ScanActivity.m_instance != null) {
                                    ScanActivity.m_instance.finish();
                                }
                            }
                        }
                    }
                });
    }

    public static void getIP() {
        m_Handler.post(
                new Runnable() {
                    public void run() {
                        ExportJavaFunction.CallBackToJS(JSBridge.class,"getIP",  getIP(demo.JSBridge.mMainActivity));
                    }
                });
    }

    public static void onOrientationChange() {
        demo.JSBridge.mMainActivity.mFloatPanel.updatePosition();
    }
    public static void onScanResult(String result) {
        Log.d("JSBridge", "url " + result);
        if (ScanActivity.m_instance != null) {
            ScanActivity.m_instance.finish();
        }
        String js = "window.layaPlayerOnScanResult('";
        js += result;
        js += "');";
        ConchJNI.RunJS(js);
    }
    @SuppressWarnings("deprecation")
    public static String getIP(Context context){
            try {
                for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
                    NetworkInterface intf = en.nextElement();
                    for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                        InetAddress inetAddress = enumIpAddr.nextElement();
                        if (!inetAddress.isLoopbackAddress() && (inetAddress instanceof Inet4Address)) {
                            android.net.NetworkInfo activeNetInfo = ((ConnectivityManager)context.getSystemService(CONNECTIVITY_SERVICE)).getActiveNetworkInfo();
                            String str = "";
                            if(activeNetInfo.getType() == ConnectivityManager.TYPE_WIFI) {
                                str += "WifiNetworkIP: ";
                            }
                            else if(activeNetInfo.getType() == ConnectivityManager.TYPE_MOBILE) {
                                str += "MobileNetworkIP: ";
                            }
                            else {
                                str += "UnknowNetworkIP: ";
                            }
                            str += inetAddress.getHostAddress().toString();
                            return str;
                        }
                    }
                }
            }
            catch (SocketException ex) {
                ex.printStackTrace();
            }
            return "";
    }
}
