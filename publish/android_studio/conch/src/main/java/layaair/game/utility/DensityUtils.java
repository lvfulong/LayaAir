package layaair.game.utility;

import android.content.Context;
import android.util.DisplayMetrics;
import android.view.WindowManager;

public class DensityUtils {
    private static DisplayMetrics getDisplayMetrics(Context context) {
        DisplayMetrics metrics = new DisplayMetrics();
		WindowManager wm = (WindowManager)(context.getSystemService(Context.WINDOW_SERVICE));
		wm.getDefaultDisplay().getMetrics(metrics);
        return metrics;
    }

    public static int getScreenWidth(Context context) {
        return DensityUtils.getDisplayMetrics(context).widthPixels;
    }

    public static int getScreenHeight(Context context) {
        return DensityUtils.getDisplayMetrics(context).heightPixels;
    }
}
