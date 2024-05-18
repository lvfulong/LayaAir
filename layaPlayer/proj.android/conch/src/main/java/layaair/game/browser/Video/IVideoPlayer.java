package layaair.game.browser.Video;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.SurfaceTexture;
import android.media.AudioManager;
import android.media.MediaMetadataRetriever;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsoluteLayout;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import androidx.annotation.RequiresApi;
import layaair.game.R;

import static android.media.MediaPlayer.SEEK_CLOSEST;

public class IVideoPlayer implements TextureView.SurfaceTextureListener {

    private final static String TAG = "IVideoPlayer";

    private String mDataSource = null;
    private FrameLayout mSurfaceView;
    private boolean isPlayerWorking = false;

    private final Context mContext;
    private final AbsoluteLayout mParentLayout;
    private MediaPlayer mMediaPlayer;
    private TextureView mTextureView;
    private ImageView mImageView;
    private Bitmap mPoster = null;
    private Button button;
    private Surface mSurface;
    private SurfaceTexture mSurfaceTexture;
    private int mPercentIndex;
    private int mCurrentTime;
    private int mVideoWidth;
    private int mVideoHeight;
    private int m_x;
    private int m_y;

    enum MediaPlayerStatus {
        NotInit,
        Idle,
        Initalized,
        Preparing,
        Prepared,
        Started,
        Stopped,
        Paused,
        OnPaused,
    };

    enum PendingType {
        Null,
        Load,
        Play,
        Pause,
        Stop,
    }

    static class PendingData {
        public PendingType type = PendingType.Null;
        public String additionData = null;

        public PendingData()
        {
            this(PendingType.Null, null);
        }

        public PendingData(PendingType type)
        {
            this(type, null);
        }

        public PendingData(PendingType type, String additionData)
        {
            this.type = type;
            this.additionData = additionData;
        }
    }

    private final ArrayList<PendingData> mPendingDataAr = new ArrayList<PendingData>();
    private boolean isSurfaceCreated = false;

    private MediaPlayerStatus mMediaPlayerState = MediaPlayerStatus.NotInit;

    public IVideoPlayer(Context context, AbsoluteLayout parentLayout) {
        mContext = context;
        mParentLayout = parentLayout;
        mVideoWidth = 300; //default value in wx
        mVideoHeight = 150; //default value in wx
        m_x = 0;
        m_y = 0;
        initTextureView();
    }

    private void initTextureView() {
        Log.i(TAG, "[Debug][Video]init texture");
//        this.mTextureView=textureView;
        final IVideoPlayer thisObj = this;

        ((Activity)mContext).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mTextureView = new TextureView(mContext);
                mTextureView.setSurfaceTextureListener(thisObj);
                mTextureView.setLayoutParams(new AbsoluteLayout.LayoutParams(mVideoWidth, mVideoHeight, m_x, m_y));

                //test
                LayoutInflater layoutInflater = LayoutInflater.from(mContext);
//                RelativeLayout relativeLayout = (RelativeLayout) layoutInflater.inflate(R.layout.video_player, null,false);
//                    m_textureView = relativeLayout.findViewById(R.id.texture_view);
//                    m_textureView.setSurfaceTextureListener(thisObj);
//                button = relativeLayout.findViewById(R.id.button_play);
//                mTextureView.setLayoutParams(new RelativeLayout.LayoutParams(320, 176));
//                mTextureView.setLayoutParams(new AbsoluteLayout.LayoutParams(320, 176, m_x, m_y));
//                    320 176
//                mParentLayout.addView(relativeLayout);
                //end test


                mParentLayout.addView(mTextureView);
                mImageView = new ImageView(mContext);
                mImageView.setLayoutParams(new AbsoluteLayout.LayoutParams(mVideoWidth, mVideoHeight, m_x, m_y));
                mParentLayout.addView(mImageView, new ViewGroup.LayoutParams(
                            ViewGroup.LayoutParams.WRAP_CONTENT,
                            ViewGroup.LayoutParams.WRAP_CONTENT));
                if (mPoster != null) {
                    Log.i(TAG, "run: set poster");
                    mPoster = Bitmap.createScaledBitmap(mPoster, mVideoWidth, mVideoHeight, true);
                    mImageView.setImageBitmap(mPoster);
                }
                mImageView.setVisibility(View.INVISIBLE);
                Log.i(TAG, "run: poster invisible");
            }
        });
    }

    private void initMediaPlayer() {
        Log.i(TAG, "[Debug][Video]init media player");
        mMediaPlayer = new MediaPlayer();
        mMediaPlayer.setSurface(mSurface);
        mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
        mMediaPlayerState = MediaPlayerStatus.Idle;

        mMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                mCurrentTime = 0;
                mMediaPlayerState = MediaPlayerStatus.Stopped;
                if (mOnEndedListener != null) {
                    mOnEndedListener.onEnded();
                }
            }
        });

        mMediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                Log.i(TAG, "[Debug][Video] onError");
                if (what == MediaPlayer.MEDIA_ERROR_UNKNOWN) {
                    Log.i(TAG, "[Debug][Video] media error unknown");
                }
                if (what == MediaPlayer.MEDIA_ERROR_SERVER_DIED) {
                    Log.i(TAG, "[Debug][Video] media server died");
                }
                if (extra == MediaPlayer.MEDIA_ERROR_IO) {
                    Log.i(TAG, "[Debug][Video] media error io");
                }
                if (extra == MediaPlayer.MEDIA_ERROR_MALFORMED) {
                    Log.i(TAG, "[Debug][Video] media error Bitstream is not conforming to the related coding standard or file spec");
                }
                if (extra == MediaPlayer.MEDIA_ERROR_UNSUPPORTED) {
                    Log.i(TAG, "[Debug][Video] media error media framework does not support the feature");
                }
                if (extra == MediaPlayer.MEDIA_ERROR_TIMED_OUT) {
                    Log.i(TAG, "[Debug][Video] media error time out");
                }
                if (mOnErrorListener != null) {
                    mOnErrorListener.onError(mp, what, extra);
                }
                return false;
            }
        });

        mMediaPlayer.setOnBufferingUpdateListener(new MediaPlayer.OnBufferingUpdateListener() {
            @Override
            public void onBufferingUpdate(MediaPlayer mp, int percent) {
                Log.i(TAG, "[Debug][Video] onBufferingUpdate");
                if (mOnProgressListener != null) {
                    mOnProgressListener.onProgress(mp, percent);
                    Log.i(TAG, "[Debug][Video] onProgress");
                }
                //缓冲暂停
                if (mMediaPlayer != null && !mMediaPlayer.isPlaying() && mMediaPlayerState == MediaPlayerStatus.Started) {
                    Log.i(TAG, "[Debug][Video] onBufferingUpdate pause");
                    if (mOnWaitingListener != null) {
                        Log.i(TAG, "[Debug][Video] onBufferingUpdate pause onWaiting");
                        mOnWaitingListener.onWaiting();
                    }
                }
            }
        });

        mMediaPlayer.setOnVideoSizeChangedListener(new MediaPlayer.OnVideoSizeChangedListener() {
            @Override
            public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
//                setWidth(width);
//                setHeight(height);
            }
        });

        //test
//        button.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                Log.i(TAG, "onClick: ");
//                if (mMediaPlayerState == MediaPlayerStatus.Started) {
//                    pause();
//                } else {
//                    play();
//                }
//            }
//        });

        isSurfaceCreated = true;
        if (mMediaPlayer != null && !mMediaPlayer.isPlaying() && (mMediaPlayerState == MediaPlayerStatus.Prepared || mMediaPlayerState == MediaPlayerStatus.Stopped)) {
            showPoster(true);
        }
        for (PendingData p : mPendingDataAr) {
            if (p.type == PendingType.Load)
                setDataSource(p.additionData);
            else if (p.type == PendingType.Play)
                play();
            else if (p.type == PendingType.Pause)
                pause();
            else if (p.type == PendingType.Stop)
                stop();
        }
        mPendingDataAr.clear();
    }

    public void setDataSource(String url) {
        Log.i(TAG, "[Debug][Video] setDataSource " + url);
        mDataSource = url;
        if (!isSurfaceCreated) {
            Log.i(TAG, "Load: PendingData");
            mPendingDataAr.clear();
            mPendingDataAr.add(new PendingData(PendingType.Load, mDataSource));
            return;
        }
        Log.i(TAG, "[Debug][Video] Load: Begin Load Video " + url);

        try {
            Uri videoUri = Uri.parse(mDataSource);
            mMediaPlayer.setDataSource(mContext, videoUri);
            mMediaPlayerState = MediaPlayerStatus.Initalized;

            mMediaPlayer.prepareAsync();
            mMediaPlayerState = MediaPlayerStatus.Preparing;
            mMediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mediaPlayer) {
                    mediaPlayer.setOnInfoListener(new MediaPlayer.OnInfoListener() {
                        @Override
                        public boolean onInfo(MediaPlayer mp, int what, int extra) {
                            if (what == MediaPlayer.MEDIA_INFO_VIDEO_RENDERING_START) {
                                Log.i(TAG, "[Debug][Video] onInfo: poster MEDIA_INFO_VIDEO_RENDERING_START ");
                                showPoster(false);
                            }
                            return false;
                        }
                    });
                    mMediaPlayerState = MediaPlayerStatus.Prepared;
                    //setCurrentTime(m_currentTime);
                    Log.i(TAG, "[Debug][Video]Load: Video is Prepared. " + mMediaPlayer.getDuration());
//                    applyAutoplay();
                }
            });

            Log.i(TAG, "Load: poster start");
            MediaMetadataRetriever mmr= new MediaMetadataRetriever();
            mmr.setDataSource(String.valueOf(videoUri), new HashMap<String, String>());
            Bitmap bitmap = mmr.getFrameAtTime();
            setPoster(bitmap);
            mmr.release();
            showPoster(true);
            Log.i(TAG, "Load: poster end");
        }
        catch (IOException e) {
            Log.e(TAG, "[Debug][Video]Load: Have errors on load video");
            e.printStackTrace();
        }
    }

    private boolean isPlayerWorking() {
        return isPlayerWorking;
    }

    private void setPlayerWorking(boolean playerWorking) {
        isPlayerWorking = playerWorking;
    }

    private boolean isPlaying() {
        return mMediaPlayer.isPlaying();
    }

    public void play() {
        if (!isSurfaceCreated) {
            mPendingDataAr.add(new PendingData(PendingType.Play));
            return;
        }

        Log.i(TAG, "[Debug][Video]Play: Start to Play!");
        mMediaPlayer.start();
        mMediaPlayerState = MediaPlayerStatus.Started;
        if (mOnPlayListener != null) {
            Log.i(TAG, "[Debug][Video] onPlay");
            mOnPlayListener.onPlay();
        }
    }

    public void pause() {
        if (!isSurfaceCreated) {
            mPendingDataAr.add(new PendingData(PendingType.Pause));
            return;
        }
        if (mMediaPlayerState == MediaPlayerStatus.Started) {
            mMediaPlayerState = MediaPlayerStatus.Paused;
            mMediaPlayer.pause();
            mCurrentTime = getCurrentTime();
            Log.i(TAG, "[Debug][Video]Pause: Start to Pause");
            if (mOnPauseListener != null) {
                Log.i(TAG, "[Debug][Video] onPause");
                mOnPauseListener.onPause();
            }
        }
    }

    public void stop() {
        if (!isSurfaceCreated) {
            mPendingDataAr.add(new PendingData(PendingType.Stop));
            return;
        }
        if (mMediaPlayerState == MediaPlayerStatus.Started ||
                mMediaPlayerState == MediaPlayerStatus.Paused) {
            mMediaPlayer.stop();
            mMediaPlayerState = MediaPlayerStatus.Stopped;
//            mCurrentTime = 0;
        }
    }

    public void destroy() {
        Dispose();
    }

    private void Dispose() {
        if(mMediaPlayer != null) {
            mMediaPlayer.release();
            mMediaPlayer = null;
        }
        ((Activity)mContext).runOnUiThread(
                new Runnable() {
                    @Override
                    public void run() {
                        mParentLayout.removeView(mTextureView);
                        mTextureView = null;
                        mPoster.recycle();
                        mPoster = null;
                        mSurface = null;
                    }
                }
        );
    }

    public void setX(int x) {
        if(m_x != x) {
            m_x = x;
            setVideoSize();
        }
    }

    public int getX() {
        return m_x;
    }

    public void setY(int y) {
        if(m_y != y) {
            m_y = y;
            setVideoSize();
        }
    }

    public int getY() {
        return m_y;
    }

    public void setWidth(int width) {
        if(mVideoWidth != width) {
            mVideoWidth = width;
            Log.d(TAG, "setWidth: " + width);
            setVideoSize();
        }
    }

    public int getWidth() {
        return mVideoWidth;
    }

    public void setHeight(int height) {
        if(mVideoHeight != height) {
            mVideoHeight = height;
            Log.d(TAG, "setHeight: " + height);
            setVideoSize();
        }
    }

    public int getHeight() {
        return mVideoHeight;
    }

    private void setVideoSize() {
        if (mPoster != null) {
            Log.i(TAG, "setVideoSize: poster");
            mPoster = Bitmap.createScaledBitmap(mPoster, mVideoWidth, mVideoHeight, true);
        }
        ((Activity)mContext).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mTextureView.setLayoutParams(new AbsoluteLayout.LayoutParams(mVideoWidth, mVideoHeight, m_x, m_y ));
                mImageView.setLayoutParams(new AbsoluteLayout.LayoutParams(mVideoWidth, mVideoHeight, m_x, m_y ));
            }
        });
    }

    public int getCurrentTime() {
        return mMediaPlayer.getCurrentPosition();
    }

    public void setCurrentTime(int pos) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            mMediaPlayer.seekTo( pos, SEEK_CLOSEST );
        }
        else {
            mMediaPlayer.seekTo( pos );
        }
    }

    public void seek(int msec) {
        setCurrentTime(msec);
    }

    public interface OnEndedListener {
        //监听视频播放到末尾事件
        void onEnded();
    }

    private OnEndedListener mOnEndedListener = null;

    public void setOnEndedListener(OnEndedListener listener) {
        mOnEndedListener = listener;
    }

    public void removeEndedListener() {
        mOnEndedListener = null;
    }

    public interface OnErrorListener {
        //监听视频错误事件
        void onError(MediaPlayer mp, int what, int extra);
    }

    private OnErrorListener mOnErrorListener = null;

    public void setOnErrorListener(OnErrorListener listener) {
        mOnErrorListener = listener;
    }

    public void removeOnErrorListener() {
        mOnErrorListener = null;
    }

    public interface OnPauseListener {
        //监听视频暂停事件
        void onPause();
    }

    private OnPauseListener mOnPauseListener = null;

    public void setOnPauseListener(OnPauseListener listener) {
        mOnPauseListener = listener;
    }

    public void removeOnPauseListener() {
        mOnPauseListener = null;
    }

    public interface OnPlayListener {
        void onPlay();
    }

    private OnPlayListener mOnPlayListener = null;

    public void setOnPlayListener(OnPlayListener listener) {
        mOnPlayListener = listener;
    }

    public void removeOnPlayListener() {
        mOnPlayListener = null;
    }

    public interface OnProgressListener {
        //监听视频下载（缓冲）事件
        void onProgress(MediaPlayer mp, int percent);
    }

    private OnProgressListener mOnProgressListener = null;

    public void setOnProgressListener(OnProgressListener listener) {
        mOnProgressListener = listener;
    }

    public void removeOnProgressListener() {
        mOnProgressListener = null;
    }

    public interface OnTimeUpdateListener{
        //监听视频播放进度更新事件
        void onTimeUpdate(int time);
    }

    private OnTimeUpdateListener mOnTimeUpdateListener = null;

    public void setOnTimeUpdateListener(OnTimeUpdateListener listener) {
        mOnTimeUpdateListener = listener;
    }

    public void removeTimeUpdateListener() {
        mOnTimeUpdateListener = null;
    }

    public interface OnWaitingListener {
        //监听视频由于需要缓冲下一帧而停止时触发
        void onWaiting();
    }

    private OnWaitingListener mOnWaitingListener = null;

    public void setOnWaitingListener(OnWaitingListener listener) {
        mOnWaitingListener = listener;
    }

    public void removeOnWaitingListener() {
        mOnWaitingListener = null;
    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        Log.i(TAG, "[Debug][Video]onSurfaceTextureAvailable");
        if (mSurfaceTexture == null) {
            mSurfaceTexture = surface;
            mSurface = new Surface(mSurfaceTexture);
            //prepare();
        } else {
            mTextureView.setSurfaceTexture(mSurfaceTexture);
        }
        initMediaPlayer();
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

    }

    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        Log.d(TAG, "onSurfaceTextureDestroyed: ");
        return null == mSurfaceTexture;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
//        Log.d(TAG, "onSurfaceTextureUpdated: ");
    }

    public void setPoster(Bitmap bitmap) {
        Log.i(TAG, "setPoster: " + (mPoster != null));
        if (mPoster == null) {
            mPoster = Bitmap.createScaledBitmap(bitmap, mVideoWidth, mVideoHeight, true);
            ((Activity)mContext).runOnUiThread(
                    new Runnable() {
                        @Override
                        public void run() {
                            mImageView.setImageBitmap(mPoster);
                            Log.i(TAG, "setPoster: set");
                        }
                    }
            );
        }
    }

    private void showPoster(final boolean show) {
        Log.i(TAG, "showPoster: " + show);
        ((Activity)mContext).runOnUiThread(
                new Runnable() {
                    @Override
                    public void run() {
                        if (show) {
                            mImageView.setVisibility(View.VISIBLE);
                            Log.i(TAG, "run: poster visible");
                        } else {
                            mImageView.setVisibility(View.INVISIBLE);
                            Log.i(TAG, "run: poster invisible");
                        }
                    }
                }
        );
    }

}
