package layaair.game.browser;


import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.RequiresApi;

public class ConchSurfaceView extends SurfaceView {
    private static final String TAG = "ConchSurfaceView";
    public boolean mIsReady = false;
    public TouchFilter mTouchFilter = new TouchFilter();
    public ConchSurfaceView(Context context) {
        super(context);
        init();
    }

    public ConchSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public ConchSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public ConchSurfaceView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init();
    }

    private void init() {
        this.getHolder().setFormat(PixelFormat.OPAQUE);
        final SurfaceHolder.Callback callback = new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                Log.d(TAG, "surfaceCreated()");
                mIsReady = true;
                ConchJNI.onSurfaceCreated(holder.getSurface());
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                Log.d(TAG, "surfaceChanged(" + width + ", " + height + ")");
                ConchJNI.OnSurfaceResize(width, height);
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.d(TAG, "surfaceDestroyed()");
                ConchJNI.OnSurfaceDestroy();
            }
        };

        SurfaceHolder holder = this.getHolder();
        holder.addCallback(callback);
    }

    public boolean onTouchEvent(MotionEvent event)
    {
        super.onTouchEvent(event);
        if(event != null)
        {
            mTouchFilter.onTouchEvent(event);
        }
        return true;
    }

    public void destroy() {
        this.mTouchFilter = null;
    }
}

