package com.chymeravr.adclient;

import android.app.Activity;
import android.content.Context;
import android.media.AudioManager;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

/**
 * Created by robin_chimera on 1/24/2017.
 */

public final class Image360Activity extends Activity implements SurfaceHolder.Callback{

    public static final String TAG = "Image360AdActivity";

    private SurfaceView mView;
    private SurfaceHolder mSurfaceHolder;
    private long mNativeHandle;

    // load the native library for image 360 ads
    static {
        System.loadLibrary("image360ad");
    }

    // Native methods for Activity lifecyle
    private native long onCreateNative(Activity activity, String appDir);

    private native void onStartNative(long handle);

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

    @Override
    protected void onCreate(Bundle icicle) {
        Log.d(TAG, "onCreate()");
        super.onCreate(icicle);


        String basePath = this.getFilesDir().getAbsolutePath();
        this.mNativeHandle = this.onCreateNative(this, basePath);

        this.mView = new SurfaceView(this);
        this.setContentView(mView);
        this.mView.getHolder().addCallback(this);

        // Force the screen to stay on, rather than letting it dim and shut off
        // while the user is watching amovie.
        this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // Force screen brightness to stay at maximum
        WindowManager.LayoutParams params = this.getWindow().getAttributes();
        params.screenBrightness = 1.0f;
        this.getWindow().setAttributes(params);


    }

    @Override
    protected void onStart() {
        Log.d(TAG, "onStart()");
        super.onStart();
        this.onStartNative(this.mNativeHandle);
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume()");
        super.onResume();
        this.onResumeNative(this.mNativeHandle);

    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause()");
        super.onPause();
        this.onPauseNative(this.mNativeHandle);
    }

    @Override
    protected void onStop() {
        Log.d(TAG, "onStop()");
        super.onStop();
        this.onStopNative(this.mNativeHandle);
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy()");
        super.onDestroy();
        this.onDestroyNative(this.mNativeHandle);
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
        AudioManager audio = (AudioManager) this.getSystemService(Context.AUDIO_SERVICE);
        audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, direction, 0);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        Log.d(TAG, "dispatchKeyEvent");
        this.finish();                                  // this will kill the activity ~ below code becomes useless
        if (mNativeHandle != 0) {
            int keyCode = event.getKeyCode();
            int action = event.getAction();
            if (action != KeyEvent.ACTION_DOWN && action != KeyEvent.ACTION_UP) {
                return this.dispatchKeyEvent(event);
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
                Log.v(TAG, "dispatchKeyEvent( " + keyCode + ", " + action + " )");
            }
            this.onKeyEventNative(this.mNativeHandle, keyCode, action);
            return true;
        }
        return false;
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        if (this.mNativeHandle != 0) {
            int action = event.getAction();
            float x = event.getRawX();
            float y = event.getRawY();
            if (action == MotionEvent.ACTION_UP) {
                Log.v(TAG, "GLES3JNIActivity::dispatchTouchEvent( " + action + ", " + x + ", " + y + " )");
            }
            this.onTouchEventNative(this.mNativeHandle, action, x, y);
        }
        return true;
    }
}
