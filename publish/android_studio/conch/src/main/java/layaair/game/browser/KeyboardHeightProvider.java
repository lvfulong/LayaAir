package layaair.game.browser;

import android.app.Activity;

import android.content.res.Configuration;
import android.content.res.Resources;
import android.util.Log;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.util.DisplayMetrics;

import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;

import android.view.WindowManager.LayoutParams;

import android.widget.PopupWindow;

import layaair.game.R;
import layaair.game.utility.DensityUtils;


public class KeyboardHeightProvider extends PopupWindow {
    private final static String TAG = "KeyboardHeightProvider";
    private KeyboardHeightObserver observer;
    private int keyboardLandscapeHeight;
    private int keyboardPortraitHeight;
    private View popupView;
    private View parentView;
    private Activity activity;
    private boolean isSoftKeyboardOpened = false;
    public KeyboardHeightProvider(Activity activity) {
		super(activity);
        this.activity = activity;

        LayoutInflater inflator = (LayoutInflater) activity.getSystemService(Activity.LAYOUT_INFLATER_SERVICE);
        this.popupView = inflator.inflate(R.layout.popupwindow, null, false);
        setContentView(popupView);

        setSoftInputMode(LayoutParams.SOFT_INPUT_ADJUST_RESIZE | LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
        setInputMethodMode(PopupWindow.INPUT_METHOD_NEEDED);

        parentView = activity.findViewById(android.R.id.content);

        setWidth(0);
        setHeight(LayoutParams.MATCH_PARENT);

        popupView.getViewTreeObserver().addOnGlobalLayoutListener(new OnGlobalLayoutListener() {

                @Override
                public void onGlobalLayout() {
                    if (popupView != null) {
                        handleOnGlobalLayout();
                    }
                }
            });
    }

    public void start() {

        if (!isShowing() && parentView.getWindowToken() != null) {
            setBackgroundDrawable(new ColorDrawable(0));
            showAtLocation(parentView, Gravity.NO_GRAVITY, 0, 0);
        }
    }

    public void close() {
        this.observer = null;
        dismiss();
    }

    public void setKeyboardHeightObserver(KeyboardHeightObserver observer) {
        this.observer = observer;
    }

    private void handleOnGlobalLayout() {
        Rect rect = new Rect();
        popupView.getWindowVisibleDisplayFrame(rect);
        int screenHeight = DensityUtils.getScreenHeight(this.activity);
        int keyboardHeight = screenHeight - rect.bottom;
        boolean visible = keyboardHeight > screenHeight / 4;
        if (!isSoftKeyboardOpened && visible) {
            isSoftKeyboardOpened = true;
            if (observer != null) {
                observer.onSoftKeyboardOpened(keyboardHeight);
            }
        } else if (isSoftKeyboardOpened && !visible) {
            isSoftKeyboardOpened = false;
            if (observer != null) {
                observer.onSoftKeyboardClosed();
            }
        }
    }

    private int getScreenOrientation() {
        return activity.getResources().getConfiguration().orientation;
    }
}
