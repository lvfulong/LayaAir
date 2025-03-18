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

public class ConchSurfaceView extends SurfaceView implements Conch.RunCallback {
    private static final String TAG = "ConchSurfaceView";
    public boolean mIsReady = false;
    public TouchFilter mTouchFilter = new TouchFilter();
    private GLThread mGLThread;
    private boolean mPaused = false;
    private Handler mGLLooperHandler;
    private Activity mActivity;
    public Conch mConch = null;
    private ConchJNI.ConchOptions mOptions = null;
    private final ConcurrentLinkedQueue<Runnable> mEvents = new ConcurrentLinkedQueue<Runnable>();
    public ConchSurfaceView(Context context, ConchJNI.ConchOptions options) {
        super(context);
        init(context, options);
    }

    private void init(Context context, ConchJNI.ConchOptions options) {

        setFocusable(true);
        setFocusableInTouchMode(true);
        setClickable(true);
        ArrayList<View> view = new ArrayList<>();
        view.add(this);
        addTouchables(view);
        setWillNotCacheDrawing(false);
        //initGestures(context);


        this.getHolder().setFormat(PixelFormat.OPAQUE);
        mOptions = options;
        mActivity = (Activity) context;
        mGLThread = new GLThread(mActivity, this);
        getHolder().addCallback(mGLThread);
        mGLThread.start();
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


    protected void executeGLThreadJobs()
    {
        Runnable job;
        while ((job = mEvents.poll()) != null)
            job.run();
    }

    @Override
    public void runInGLThread(Runnable f) {
        //mGLLooperHandler.post(r);
        //if (!mState.librariesHasBeenLoaded())
        //    return;

        //if (Thread.currentThread() == m_MainThread)
        //    r.run();
        //else
        mEvents.add(f);
    }

    @Override
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
            //mIsReady = true;
            Surface surface = holder.getSurface();
            if (mSurfaceView.mConch == null) {
                mSurfaceView.mConch = new Conch(mActivity, mOptions, (Conch.RunCallback) mSurfaceView, surface);
            }
            else {
                mSurfaceView.mConch.onSurfaceCreated(surface);
            }

        }

        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            Log.d(TAG, "surfaceChanged(" + width + ", " + height + ")");
            mSurfaceView.mConch.onSurfaceResize(width, height);
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
            Log.d(TAG, "surfaceDestroyed()");
            mSurfaceView.mConch.onSurfaceDestroy();
        }

        public void shutdown() {
            mGLLooperHandler.getLooper().quitSafely();
        }

        /*public void run() {
            Looper.prepare();

            mGLLooperHandler = new Handler();

            Looper.loop();
        }*/
        volatile boolean mPause;
        volatile boolean mQuit;

        @Override
        public void run()
        {
            setName("ScriptThread");
            try
            {
                while (!mQuit)
                {
                    synchronized (this)	{ wait(); }
                    while (!mQuit && !mPause)
                    {
                        executeGLThreadJobs();
                        //if (!isFinishing() && !nativeRender())
                        //    finish();
                        if (mSurfaceView.mConch != null) {
                            ConchJNI.performUpdates();
                        }
                    }
                }
            }
            catch (InterruptedException quit) {}
        }

        public void quit()
        {
            mQuit = true;
            synchronized (this)	{ notify(); }
        }

        public void resumeExecution()
        {
            mPause = false;
            synchronized (this)	{ notify(); }
        }

        public void pauseExecution()
        {
            mPause = true;
            synchronized (this)	{ notify(); }
        }
    }
    public void onPause() {
        mGLThread.pauseExecution();
        if (mConch != null) {
            mConch.onPause();
        }
    }

    public void onResume() {
        mGLThread.resumeExecution();
        if (mConch != null) {
            mConch.onResume();
        }
    }
}

