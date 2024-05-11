package demo;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Point;
import android.graphics.Rect;
import android.os.Build;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.DisplayCutout;
import android.view.WindowInsets;
import android.view.WindowManager;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class NotchUtils {
    public static boolean isNotch(Context context) {
        ((Activity)context).getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            Log.d("Notch", "isNotch: cutout");
            WindowManager.LayoutParams lp = ((Activity)context).getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            ((Activity)context).getWindow().setAttributes(lp);
            WindowInsets rootWindowInsets = ((Activity)context).getWindow().getDecorView().getRootWindowInsets();
            DisplayCutout displayCutout = null;
            if (rootWindowInsets != null) {
                Log.d("Notch", "isNotch: if");
                displayCutout = rootWindowInsets.getDisplayCutout();
                Log.d("Notch", "isNotch: if " + displayCutout);
            } else {
                Log.d("Notch", "isNotch: else");
            }
            Log.d("Notch", "isNotch: return " + (displayCutout != null));
            return (displayCutout != null);
        } else {
            String manufacturer = Build.MANUFACTURER;
            if (manufacturer == null || manufacturer.isEmpty()) {
                return false;
            } else if (manufacturer.equalsIgnoreCase("HUAWEI")) {
                return hasNotchAtHuawei(context);
            } else if (manufacturer.equalsIgnoreCase("xiaomi")) {
                return hasNotchAtXiaomi(context);
            } else if (manufacturer.equalsIgnoreCase("oppo")) {
                return hasNotchAtOPPO(context);
            } else if (manufacturer.equalsIgnoreCase("vivo")) {
                return hasNotchAtVivo(context);
            } else if (manufacturer.equalsIgnoreCase("samsung")) {
                return hasNotchAtSamsung(context);
            } else {
                return false;
            }
        }
    }

    public static boolean hasNotchAtHuawei(Context context) {
        boolean ret = false;
        try {
            ClassLoader classLoader = context.getClassLoader();
            Class HwNotchSizeUtil = classLoader.loadClass("com.huawei.android.util.HwNotchSizeUtil");
            Method get = HwNotchSizeUtil.getMethod("hasNotchInScreen");
            ret = (boolean) get.invoke(HwNotchSizeUtil);
            return ret;
        } catch (ClassNotFoundException e) {
            Log.e("Notch", "hasNotchAtHuawei ClassNotFoundException");
            e.printStackTrace();
            return false;
        } catch (NoSuchMethodException e) {
            Log.e("Notch", "hasNotchAtHuawei NoSuchMethodException");
            e.printStackTrace();
            return false;
        } catch (Exception e) {
            Log.e("Notch", "hasNotchAtHuawei Exception");
            e.printStackTrace();
            return false;
        } finally {
            return ret;
        }
    }

    public static final int VIVO_NOTCH = 0x00000020;//是否有刘海

    public static boolean hasNotchAtVivo(Context context) {
        boolean ret = false;
        try {
            ClassLoader classLoader = context.getClassLoader();
            Class FtFeature = classLoader.loadClass("android.util.FtFeature");
            Method method = FtFeature.getMethod("isFeatureSupport", int.class);
            ret = (boolean) method.invoke(FtFeature, VIVO_NOTCH);
            return ret;
        } catch (ClassNotFoundException e) {
            Log.e("Notch", "hasNotchAtVivo ClassNotFoundException");
            e.printStackTrace();
            return false;
        } catch (NoSuchMethodException e) {
            Log.e("Notch", "hasNotchAtVivo NoSuchMethodException");
            e.printStackTrace();
            return false;
        } catch (Exception e) {
            Log.e("Notch", "hasNotchAtVivo Exception");
            e.printStackTrace();
            return false;
        } finally {
            return ret;
        }
    }

    public static boolean hasNotchAtXiaomi(Context context) {
        boolean ret = false;
        int value = 0;
        try {
            ClassLoader classLoader = context.getClassLoader();
            Class Properties = classLoader.loadClass("android.os.SystemProperties");
            Method method = Properties.getMethod("getInt", String.class, int.class);
            value = (Integer) method.invoke(Properties, "ro.miui.notch", 0);
            ret = (value == 1);
            return ret;
        } catch (InvocationTargetException e) {
            Log.e("Notch", "hasNotchAtXiaomi InvocationTargetException");
            e.printStackTrace();
            return false;
        } catch (NoSuchMethodException e) {
            Log.e("Notch", "hasNotchAtXiaomi NoSuchMethodException");
            e.printStackTrace();
            return false;
        } catch (IllegalAccessException e) {
            Log.e("Notch", "hasNotchAtXiaomi IllegalAccessException");
            e.printStackTrace();
            return false;
        } catch (ClassNotFoundException e) {
            Log.e("Notch", "hasNotchAtXiaomi ClassNotFoundException");
            e.printStackTrace();
            return false;
        } finally {
            return ret;
        }
    }

    public static boolean hasNotchAtSamsung(Context context) {
        try {
            final Resources res = context.getResources();
            final int resId = res.getIdentifier("config_mainBuiltInDisplayCutout", "string", "android");
            final String spec = resId > 0 ? res.getString(resId) : null;
            return spec != null && !TextUtils.isEmpty(spec);
        } catch (Exception e) {
            Log.e("Notch", "Can not update hasDisplayCutout. " + e.toString());
            return false;
        }
    }

    public static boolean hasNotchAtOPPO(Context context) {
        return context.getPackageManager().hasSystemFeature("com.oppo.feature.screen.heteromorphism");
    }

    public static int getStatusHeight(Context context) {
        Log.d("Notch", "getStatusHeight: ");
        int result = 0;
        int resourceId = context.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = context.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }

    public static int getSafeHeight(Context context) {
        int result = 0;
        result = getStatusHeight(context);
        Log.d("Notch", "getSafeHeight: " + result);
        return result;
    }

    public static Rect getSafeInsetRect(Activity activity) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            WindowInsets insets =  activity.getWindow().getDecorView().getRootWindowInsets();
            DisplayCutout cutout = insets.getDisplayCutout();
            Rect result = new Rect(0, 0, 0, 0);
            if (cutout != null) {
                result.left = cutout.getSafeInsetLeft();
                result.top = cutout.getSafeInsetTop();
                result.right = cutout.getSafeInsetRight();
                result.bottom = cutout.getSafeInsetBottom();
            }
            return result;
        }
        else {
            //if (isNotch(activity)) {
            //    return new Rect(0, getSafeHeight(activity), 0, 0);
            //}
            //else {
                return new Rect(0, 0, 0, 0);
            //}
        }
    }
}
