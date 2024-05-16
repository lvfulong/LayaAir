package layaair.game.browser;

import android.app.Activity;
import android.graphics.Rect;
import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

import androidx.core.widget.NestedScrollView;

public class KeyboardUtil {
    private static final String TAG = "KeyboardUtil";


    public static void assistActivity(Activity activity, int viewId) {
        new KeyboardUtil(activity, viewId);
    }

    private View mChildOfContent;
    private NestedScrollView mScrollView;
    private RelativeLayout.LayoutParams relativeLayoutParams;

    private KeyboardUtil(Activity activity, int viewId) {
        FrameLayout content = (FrameLayout) activity
                .findViewById(android.R.id.content);
        mChildOfContent = content.getChildAt(0);
        mScrollView = (NestedScrollView) content.findViewById(viewId);
        mChildOfContent.getViewTreeObserver().addOnGlobalLayoutListener(
                new ViewTreeObserver.OnGlobalLayoutListener() {
                    @Override
                    public void onGlobalLayout() {
                        possiblyResizeChildOfContent();
                    }
                });
    }

    private void possiblyResizeChildOfContent() {
        int contentHeight = mChildOfContent.getRootView().getHeight();
        int curDisplayHeight = computeUsableHeight();
        if (contentHeight - curDisplayHeight > contentHeight / 4) {
            Log.e(TAG, "possiblyResizeChildOfContent: 1" );
            mScrollView.scrollTo(0,600);
//                mScrollView.fullScroll(ScrollView.FOCUS_DOWN);
        } else {
            Log.e(TAG, "possiblyResizeChildOfContent: 2" );
        }
    }

    /**
     * 获取屏幕可显示区域高度
     *
     * @return
     */
    private int computeUsableHeight() {
        Rect r = new Rect();
        mChildOfContent.getWindowVisibleDisplayFrame(r);
        return r.height();
    }
}