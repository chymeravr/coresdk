//package com.chymeravr.appgearvr;
//
//import android.app.Activity;
//import android.content.Context;
//import android.content.res.AssetManager;
//import android.media.AudioManager;
//import android.util.Log;
//import android.view.KeyEvent;
//import android.view.MotionEvent;
//import android.view.Surface;
//import android.view.SurfaceHolder;
//import android.view.SurfaceView;
//import android.view.WindowManager;
//
///**
// * Created by robin_chimera on 1/21/2017.
// */
//
//public class Image360Ad implements SurfaceHolder.Callback{
//    private final Activity activity;
//    private final String placementId;
//
//    private static final String TAG = "Image360::Android";
//    private SurfaceView mView;
//    private SurfaceHolder mSurfaceHolder;
//    private long mNativeHandle;
//
//    static {
//        System.loadLibrary("image360ad");
//    }
//
//    // Native methods for Activity lifecyle
//    private native long onCreateNative(Activity activity, AssetManager assetManager, String appDir);
//    private native void onStartNative(Activity activity, long handle );
//    private native void onPauseNative(long handle );
//    private native void onResumeNative(long handle );
//    private native void onStopNative(long handle );
//    private native void onDestroyNative(long handle );
//
//    // Native methods for Surface Lifecycle
//    public static native void onSurfaceCreatedNative( long handle, Surface s );
//    public static native void onSurfaceChangedNative( long handle, Surface s );
//    public static native void onSurfaceDestroyedNative( long handle );
//
//    public static native void onKeyEventNative( long handle, int keyCode, int action );
//    public static native void onTouchEventNative( long handle, int action, float x, float y );
//
//
//    // TODO: 1/21/2017 Verify ad id, activity etc.
//    public Image360Ad(Activity activity, String placementId){
//        this.activity = activity;
//        this.placementId = placementId;
//
//        this.mView = new SurfaceView( activity );
//        this.activity.setContentView(mView);
//        this.mView.getHolder().addCallback( this );
//
//        // Force the screen to stay on, rather than letting it dim and shut off
//        // while the user is watching amovie.
//        this.activity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
//
//        // Force screen brightness to stay at maximum
//        WindowManager.LayoutParams params = this.activity.getWindow().getAttributes();
//        params.screenBrightness = 1.0f;
//        this.activity.getWindow().setAttributes( params );
//
//        // TODO: 1/21/2017 Replace this with a network call to get images from server
//        AssetManager assetManager = this.activity.getAssets();
//
//        // call native library creation function herer
//        String basePath = this.activity.getFilesDir().getAbsolutePath();
//        mNativeHandle = onCreateNative(this.activity, assetManager, basePath);
//    }
//
//    // TODO: 1/21/2017 Activity lifecylce not in our control. Work around this before release
//    public void onStart(){
//        onStartNative(this.activity, this.mNativeHandle);
//    }
//
//    public void onResume(){
//        onResumeNative(this.mNativeHandle);
//    }
//
//    public void onPause(){
//        onPauseNative(this.mNativeHandle);
//    }
//
//    public void onStop(){
//        onStopNative(this.mNativeHandle);
//    }
//
//    public void onDestroy(){
//        onDestroyNative(this.mNativeHandle);
//    }
//
//    @Override
//    public void surfaceCreated(SurfaceHolder holder) {
//        Log.v(TAG, "ActivityGearVR::onSurfaceCreated()");
//        if ( this.mNativeHandle != 0)
//        {
//            onSurfaceCreatedNative( this.mNativeHandle, holder.getSurface() );
//            this.mSurfaceHolder = holder;
//        }
//    }
//
//    @Override
//    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
//        Log.v(TAG, "ActivityGearVR::onSurfaceChanged()");
//        if (this.mNativeHandle != 0)
//        {
//            onSurfaceChangedNative(this.mNativeHandle, holder.getSurface() );
//            this.mSurfaceHolder = holder;
//        }
//    }
//
//    @Override
//    public void surfaceDestroyed(SurfaceHolder holder) {
//        Log.v(TAG, "ActivityGearVR::onSurfaceDestroyed()");
//        if ( this.mNativeHandle != 0 )
//        {
//            onSurfaceDestroyedNative( this.mNativeHandle);
//            this.mSurfaceHolder = null;
//        }
//
//    }
//
//    private void adjustVolume(int direction)
//    {
//        AudioManager audio = (AudioManager) this.activity.getSystemService(Context.AUDIO_SERVICE);
//        audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, direction, 0);
//    }
//
//
//    public boolean dispatchKeyEvent( KeyEvent event )
//    {
//        if ( mNativeHandle != 0 )
//        {
//            int keyCode = event.getKeyCode();
//            int action = event.getAction();
//            if ( action != KeyEvent.ACTION_DOWN && action != KeyEvent.ACTION_UP )
//            {
//                return this.activity.dispatchKeyEvent( event );
//            }
//            if ( keyCode == KeyEvent.KEYCODE_VOLUME_UP )
//            {
//                adjustVolume( 1 );
//                return true;
//            }
//            if ( keyCode == KeyEvent.KEYCODE_VOLUME_DOWN )
//            {
//                adjustVolume( -1 );
//                return true;
//            }
//            if ( action == KeyEvent.ACTION_UP )
//            {
//                Log.v( TAG, "GLES3JNIActivity::dispatchKeyEvent( " + keyCode + ", " + action + " )" );
//            }
//            onKeyEventNative( mNativeHandle, keyCode, action );
//            return true;
//        }
//        return false;
//    }
//
//    public boolean dispatchTouchEvent( MotionEvent event )
//    {
//        if ( mNativeHandle != 0 )
//        {
//            int action = event.getAction();
//            float x = event.getRawX();
//            float y = event.getRawY();
//            if ( action == MotionEvent.ACTION_UP )
//            {
//                Log.v( TAG, "GLES3JNIActivity::dispatchTouchEvent( " + action + ", " + x + ", " + y + " )" );
//            }
//            onTouchEventNative( mNativeHandle, action, x, y );
//        }
//        return true;
//    }
//}
