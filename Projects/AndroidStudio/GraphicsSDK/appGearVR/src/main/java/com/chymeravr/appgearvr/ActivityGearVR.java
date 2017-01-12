package com.chymeravr.appgearvr;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

//import com.chymeravr.graphicssdkgearvr.adapter.Image360JniAdapter;
//import com.chymeravr.graphicssdkgearvr.adapter.Image360JniAdapterGearVR;

public class ActivityGearVR extends Activity implements SurfaceHolder.Callback{

    // load native-lib.so since it has all the native functions
    static {
        System.loadLibrary("image360gearvr");
    }

    // Native methods for Activity lifecyle
    private native long onCreateNative(Activity activity, AssetManager assetManager);
    private native void onStartNative(long handle );
    private native void onPauseNative(long handle );
    private native void onResumeNative(long handle );
    private native void onStopNative(long handle );
    private native void onDestroyNative(long handle );

    // Native methods for Surface Lifecycle
    public static native void onSurfaceCreatedNative( long handle, Surface s );
    public static native void onSurfaceChangedNative( long handle, Surface s );
    public static native void onSurfaceDestroyedNative( long handle );

    private static final String TAG = "Image360::Android";
    private SurfaceView mView;
    private SurfaceHolder mSurfaceHolder;
    private long mNativeHandle;

    @Override
    protected void onCreate(Bundle icicle)
    {
        Log.v(TAG, "----------------------------------------------");
        Log.v(TAG, "ActivityGearVR::onCreate() Begin");
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

        //mNativeHandle =                               // call native library creation function herer
        mNativeHandle = onCreateNative(this, assetManager);
        Log.v(TAG, "ActivityGearVR::onCreate() Complete");
    }

    @Override
    protected void onStart()
    {
        Log.v(TAG, "ActivityGearVR::onStart()");
        super.onStart();

        // call native start
        onStartNative( mNativeHandle);
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
        super.onPause();

        // call native pause
        onPauseNative( mNativeHandle );
    }

    @Override
    protected void onStop()
    {
        Log.v(TAG, "ActivityGearVR::onStop()");
        super.onStop();

        // call native stop
        onStopNative( mNativeHandle );
    }

    @Override
    protected void onDestroy()
    {
        Log.v(TAG, "ActivityGearVR::onDestroy()");
        super.onDestroy();

        // call native Destruction
        onDestroyNative( mNativeHandle );
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceCreated()");
        if ( mNativeHandle != 0)
        {
            onSurfaceCreatedNative( mNativeHandle, holder.getSurface() );
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.v(TAG, "ActivityGearVR::onSurfaceChanged()");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v(TAG, "ActivityGearVR::onSurfaceDestroyed()");
    }
}
