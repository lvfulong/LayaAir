package layaair.game.browser;


import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import androidx.annotation.RequiresApi;

import java.util.ArrayList;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ConchSurfaceView extends SurfaceView {
    private static final String TAG = "ConchSurfaceView";
    public TouchFilter mTouchFilter = new TouchFilter();
    private GLThread mGLThread;
    private Activity mActivity;
    private boolean mIsReady = false;
    private ConchJNI.ConchOptions mOptions = null;
    private final ConcurrentLinkedQueue<Runnable> mEvents = new ConcurrentLinkedQueue<Runnable>();
    public ConchSurfaceView(Context context, ConchJNI.ConchOptions options) {
        super(context);
        init(context, options);
    }

    private void init(Context context, ConchJNI.ConchOptions options) {
        mOptions = options;
        mActivity = (Activity) context;
        mGLThread = new GLThread(mActivity, this);
        getHolder().addCallback(mGLThread);
        mGLThread.start();
    }

    public boolean onTouchEvent(MotionEvent event)
    {
        super.onTouchEvent(event);
        if(event != null && mTouchFilter != null)
        {
            mTouchFilter.onTouchEvent(event);
        }
        return true;
    }

    public void destroy() {
        this.mTouchFilter = null;
    }


    protected void executeGLThreadJobs()
    {
        Runnable job;
        while ((job = mEvents.poll()) != null)
            job.run();
    }

    public void runInGLThread(Runnable f) {
        mEvents.add(f);
    }

    public void runInUIThread(Runnable f) {
        post(f);
    }

    class GLThread extends Thread implements SurfaceHolder.Callback {
        private Activity mActivity;
        private ConchSurfaceView mSurfaceView;
        GLThread(Activity activity, ConchSurfaceView surfaceView) {
            mActivity = activity;
            mSurfaceView = surfaceView;
        }

        public void surfaceCreated(SurfaceHolder holder) {
            Log.d(TAG, "surfaceCreated()");
            Surface surface = holder.getSurface();
            if (!mIsReady) {
                runInGLThread(() -> ConchJNI.init(mActivity, mOptions, surface));
                mIsReady = true;
            }
            else {
                runInGLThread(() -> ConchJNI.onSurfaceCreated(surface));
            }

        }

        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            Log.d(TAG, "surfaceChanged(" + width + ", " + height + ")");
            runInGLThread(() -> ConchJNI.onSurfaceResize(width, height));
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
            Log.d(TAG, "surfaceDestroyed()");
            runInGLThread(() -> ConchJNI.onSurfaceDestroy());
        }
        volatile boolean mPause;
        volatile boolean mQuit;

        @Override
        public void run() {
            setName("ScriptThread");
            try {
                while (!mQuit) {
                    synchronized (this)	{ wait(); }
                    while (!mQuit && !mPause) {
                        executeGLThreadJobs();
                        if (mIsReady) {
                            ConchJNI.performUpdates();
                        }
                    }
                }
            } catch (InterruptedException e) {
            }
        }

        public void quit() {
            mQuit = true;
            synchronized (this)	{ notify(); }
        }

        public void resumeExecution() {
            mPause = false;
            synchronized (this)	{ notify(); }
        }

        public void pauseExecution() {
            mPause = true;
            synchronized (this)	{ notify(); }
        }
    }
    public void onPause() {
        runInGLThread(new Runnable() {
            @Override
            public void run() {
                ConchJNI.onAppPause();
                mGLThread.pauseExecution();
            }
        });
    }

    public void onResume() {  
        mGLThread.resumeExecution();
        runInGLThread(new Runnable() {
            @Override
            public void run() {
                ConchJNI.onAppResume();
            }
        });
    }
    public void shutdown() {
        runInGLThread(() -> ConchJNI.uninit());
    }
}
