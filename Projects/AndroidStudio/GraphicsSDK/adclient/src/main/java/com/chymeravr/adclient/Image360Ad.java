package com.chymeravr.adclient;


import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.media.AudioManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

import com.android.volley.toolbox.JsonObjectRequest;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by robin_chimera on 11/28/2016.
 */

/*
    This class manages the Image360 ad display for the SDK
    It is responsible for managing the ad depending on lifecycle
    events. It listens for lifecycle events and renders ads at an
    appropriate time.
    Its core responsibilities are
        - maintaining load states
        - store an ad
        - call display methods for ad
        - request a new ad
 */

public final class Image360Ad extends Ad implements SurfaceHolder.Callback {
    private static final String TAG = "ChymeraVR::Image360";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */

    private ServerListener<JSONObject> adServerListener;
    private ServerListener<Bitmap> mediaServerListener;
    private ServerListener<byte[]> mediaDownloadServerListener;

    private RequestGenerator requestGenerator;

    private final Activity activity;
    private final String placementId;

    private SurfaceView mView;
    private SurfaceHolder mSurfaceHolder;
    private long mNativeHandle;

    // load the native library for image 360 ads
    static {
        System.loadLibrary("image360ad");
    }

    // Native methods for Activity lifecyle
    private native long onCreateNative(Activity activity, AssetManager assetManager, String appDir);

    private native void onStartNative(Activity activity, long handle);

    private native void onPauseNative(long handle);

    private native void onResumeNative(long handle);

    private native void onStopNative(long handle);

    private native void onDestroyNative(long handle);

    // Native methods for Surface Lifecycle
    public static native void onSurfaceCreatedNative(long handle, Surface s);

    public static native void onSurfaceChangedNative(long handle, Surface s);

    public static native void onSurfaceDestroyedNative(long handle);

    // Native methods for handling touch and key events
    public static native void onKeyEventNative(long handle, int keyCode, int action);

    public static native void onTouchEventNative(long handle, int action, float x, float y);

    public Image360Ad(String adUnitID, Context context, AdListener adListener) {
        super(adUnitID, context, adListener);

        this.activity = (Activity) context;
        this.placementId = adUnitID;
        this.adServerListener = new AdServerListener(this);
        this.mediaServerListener = new Image360MediaServerListener(this);
        this.mediaDownloadServerListener = new Image360DownloadMediaServerListener(this);
        this.requestGenerator = new RequestGenerator(Type.IMAGE360);

        AssetManager assetManager = this.activity.getAssets();
        String basePath = this.activity.getFilesDir().getAbsolutePath();

        this.mNativeHandle = onCreateNative(this.activity, assetManager, basePath);
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(AdRequest adRequest) {
        this.setLoading(true);
        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        JsonObjectRequest jsonObjRequest =
                this.requestGenerator.getAdServerJsonRequest(adRequest, this.adServerListener);

        this.adServerListener.getRequestQueue().add(jsonObjRequest);

//        this.onStart();
        this.onResume();
    }

    @Override
    void onAdServerResponseSuccess(JSONObject response) {
        try {
            this.setMediaUrl(response.getString("mediaUrl"));

            // download media from url and save in internal memory
            InputStreamVolleyRequest mediaDownloadRequest = this.requestGenerator
                    .getMediaDownloadRequest(this.getMediaUrl(), this.mediaDownloadServerListener);
            this.mediaServerListener.getRequestQueue().add(mediaDownloadRequest);

            Log.i(TAG, "Ad Server response successfully processed. Proceeding to query Media Server");
        } catch (JSONException e) {
            this.setLoading(false);
            this.getAdListener().onAdFailedToLoad();
            Log.e(TAG, "Exception", e);
        }
    }

    @Override
    void onMediaServerResponseSuccess(Object media) {
        this.getAdListener().onAdLoaded();
        this.setLoaded(true);
        this.setLoading(false);
        Log.i(TAG, "Media Server Response Successful");
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {

        Log.d(TAG, "attempting to show image");
        if (this.isLoaded()) {
            Log.d(TAG, "an image has been loaded. Create surface");

            this.mView = new SurfaceView(activity);
            this.activity.setContentView(mView);
            this.mView.getHolder().addCallback(this);

            // Force the screen to stay on, rather than letting it dim and shut off
            // while the user is watching amovie.
            this.activity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

            // Force screen brightness to stay at maximum
            WindowManager.LayoutParams params = this.activity.getWindow().getAttributes();
            params.screenBrightness = 1.0f;
            this.activity.getWindow().setAttributes(params);

        }
    }

    // TODO: 1/21/2017 Activity lifecylce not in our control. Work around this before release
    public void onStart() {
        onStartNative(this.activity, this.mNativeHandle);
    }

    public void onResume() {
        if (this.mNativeHandle != 0) {
            onResumeNative(this.mNativeHandle);
        }
    }

    public void onPause() {
        if (this.mNativeHandle != 0) {
            onPauseNative(this.mNativeHandle);
        }
    }

    public void onStop() {
        if (this.mNativeHandle != 0) {
            onStopNative(this.mNativeHandle);
        }
    }

    public void onDestroy() {
        if (this.mNativeHandle != 0) {
            this.onPause();
//            this.onStop();
            this.surfaceDestroyed(this.mSurfaceHolder);
//            onDestroyNative(this.mNativeHandle);
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceCreated()");
        if (this.mNativeHandle != 0) {
            onSurfaceCreatedNative(this.mNativeHandle, holder.getSurface());
            this.mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.v(TAG, "ActivityGearVR::onSurfaceChanged()");
        if (this.mNativeHandle != 0) {
            onSurfaceChangedNative(this.mNativeHandle, holder.getSurface());
            this.mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceDestroyed()");
        if (this.mNativeHandle != 0) {
            onSurfaceDestroyedNative(this.mNativeHandle);
            this.mSurfaceHolder = null;
        }

    }

    private void adjustVolume(int direction) {
        AudioManager audio = (AudioManager) this.activity.getSystemService(Context.AUDIO_SERVICE);
        audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, direction, 0);
    }


    public boolean dispatchKeyEvent(KeyEvent event) {
        if (mNativeHandle != 0) {
            int keyCode = event.getKeyCode();
            int action = event.getAction();
            if (action != KeyEvent.ACTION_DOWN && action != KeyEvent.ACTION_UP) {
                return this.activity.dispatchKeyEvent(event);
            }
            if (keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
                adjustVolume(1);
                return true;
            }
            if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN) {
                adjustVolume(-1);
                return true;
            }
            if (action == KeyEvent.ACTION_UP) {
                Log.v(TAG, "GLES3JNIActivity::dispatchKeyEvent( " + keyCode + ", " + action + " )");
            }
            onKeyEventNative(mNativeHandle, keyCode, action);
            return true;
        }
        return false;
    }

    public boolean dispatchTouchEvent(MotionEvent event) {
        if (mNativeHandle != 0) {
            int action = event.getAction();
            float x = event.getRawX();
            float y = event.getRawY();
            if (action == MotionEvent.ACTION_UP) {
                Log.v(TAG, "GLES3JNIActivity::dispatchTouchEvent( " + action + ", " + x + ", " + y + " )");
            }
            onTouchEventNative(mNativeHandle, action, x, y);
        }
        return true;
    }
}