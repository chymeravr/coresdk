package com.chymeravr.appgearvr;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;


public class ActivityGearVR extends Activity implements SurfaceHolder.Callback{

    // load the native library since it has all the native functions
    static {
        System.loadLibrary("image360Android");
    }

    // Native methods for Activity lifecyle
    private native long onCreateNative(Activity activity, AssetManager assetManager);
    private native void onStartNative(Activity activity, long handle );
    private native void onPauseNative(long handle );
    private native void onResumeNative(long handle );
    private native void onStopNative(long handle );
    private native void onDestroyNative(long handle );

    // Native methods for Surface Lifecycle
    public static native void onSurfaceCreatedNative( long handle, Surface s );
    public static native void onSurfaceChangedNative( long handle, Surface s );
    public static native void onSurfaceDestroyedNative( long handle );

    public static native void onKeyEventNative( long handle, int keyCode, int action );
    public static native void onTouchEventNative( long handle, int action, float x, float y );

    private static final String TAG = "Image360::Android";
    private SurfaceView mView;
    private SurfaceHolder mSurfaceHolder;
    private long mNativeHandle;

    @Override
    protected void onCreate(Bundle icicle)
    {
        Log.v(TAG, "----------------------------------------------");
        Log.v(TAG, "ActivityGearVR::onCreate()");
        super.onCreate(icicle);

        mView = new SurfaceView( this );
        setContentView( mView );
        mView.getHolder().addCallback( this );

        // Force the screen to stay on, rather than letting it dim and shut off
        // while the user is watching amovie.
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // Force screen brightness to stay at maximum
        WindowManager.LayoutParams params = getWindow().getAttributes();
        params.screenBrightness = 1.0f;
        getWindow().setAttributes( params );

        AssetManager assetManager = this.getAssets();

        // call native library creation function herer
        mNativeHandle = onCreateNative(this, assetManager);
    }

    @Override
    protected void onStart()
    {
        Log.v(TAG, "ActivityGearVR::onStart()");
        super.onStart();

        // call native start
        onStartNative(  this, mNativeHandle);
    }

    @Override
    protected void onResume()
    {
        Log.v(TAG, "ActivityGearVR::onResume()");
        super.onResume();

        // call native resume
        onResumeNative( mNativeHandle);
    }

    @Override
    protected void onPause()
    {
        Log.v(TAG, "ActivityGearVR::onPause()");
        // call native pause
        onPauseNative( mNativeHandle );
        super.onPause();
    }

    @Override
    protected void onStop()
    {
        Log.v(TAG, "ActivityGearVR::onStop()");
        // call native stop
        onStopNative( mNativeHandle );
        super.onStop();
    }

    @Override
    protected void onDestroy()
    {
        Log.v(TAG, "ActivityGearVR::onDestroy()");
        // call native Destruction
        if( mSurfaceHolder != null ) {
            onSurfaceDestroyedNative( mNativeHandle );
        }
        onDestroyNative(mNativeHandle);
        super.onDestroy();
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceCreated()");
        if ( mNativeHandle != 0)
        {
            onSurfaceCreatedNative( mNativeHandle, holder.getSurface() );
            mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.v(TAG, "ActivityGearVR::onSurfaceChanged()");
        if (mNativeHandle != 0)
        {
            onSurfaceChangedNative(mNativeHandle, holder.getSurface() );
            mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceDestroyed()");
        if ( mNativeHandle != 0 )
        {
            onSurfaceDestroyedNative( mNativeHandle);
            mSurfaceHolder = null;
        }
    }

    private void adjustVolume(int direction)
    {
        AudioManager audio = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, direction, 0);
    }

    @Override
    public boolean dispatchKeyEvent( KeyEvent event )
    {
        if ( mNativeHandle != 0 )
        {
            int keyCode = event.getKeyCode();
            int action = event.getAction();
            if ( action != KeyEvent.ACTION_DOWN && action != KeyEvent.ACTION_UP )
            {
                return super.dispatchKeyEvent( event );
            }
            if ( keyCode == KeyEvent.KEYCODE_VOLUME_UP )
            {
                adjustVolume( 1 );
                return true;
            }
            if ( keyCode == KeyEvent.KEYCODE_VOLUME_DOWN )
            {
                adjustVolume( -1 );
                return true;
            }
            if ( action == KeyEvent.ACTION_UP )
            {
                Log.v( TAG, "GLES3JNIActivity::dispatchKeyEvent( " + keyCode + ", " + action + " )" );
            }
            onKeyEventNative( mNativeHandle, keyCode, action );
            return true;
        }
        return false;
    }

    @Override
    public boolean dispatchTouchEvent( MotionEvent event )
    {
        if ( mNativeHandle != 0 )
        {
            int action = event.getAction();
            float x = event.getRawX();
            float y = event.getRawY();
            if ( action == MotionEvent.ACTION_UP )
            {
                Log.v( TAG, "GLES3JNIActivity::dispatchTouchEvent( " + action + ", " + x + ", " + y + " )" );
            }
            onTouchEventNative( mNativeHandle, action, x, y );
        }
        return true;
    }
}
