package co.chimeralabs.sdk.android.adapter;

import android.app.Activity;
import android.view.Surface;
import android.view.SurfaceHolder;

/**
 * Created by chimeralabs on 8/8/2016.
 */

public class Image360JniAdapterNoHMD implements Image360JniAdapter{
    private long jniClassHandle;

    static{
        System.loadLibrary("image360gear_jni");
    }

    public native long createImage360Native(Activity obj);
    public native void onSurfaceCreatedNative(long jniClassHandle, Surface surface);
    public native void onSurfaceChangedNative(long jniClassHandle, Surface surface, int format, int width, int height);
    public native void onSurfaceDestroyedNative(long jniClassHandle, Surface surface);
    public native void onResumeNative(long jniClassHandle);
    public native void onPauseNative(long jniClassHandle);
    public native void onStopNative(long jniClassHandle);

    @Override
    public void createImage360(Activity object) {
        this.jniClassHandle = createImage360Native(object);
    }

    @Override
    public void onSurfaceCreated(SurfaceHolder surfaceHolder) {
        onSurfaceCreatedNative(this.jniClassHandle, surfaceHolder.getSurface());
    }

    @Override
    public void onSurfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height) {
        onSurfaceChangedNative(this.jniClassHandle, surfaceHolder.getSurface(), format, width, height);
    }

    @Override
    public void onSurfaceDestroyed(SurfaceHolder surfaceHolder) {
        onSurfaceDestroyedNative(jniClassHandle, surfaceHolder.getSurface());
    }

    @Override
    public void onPause() {
        onPauseNative(jniClassHandle);
    }

    @Override
    public void onResume() {
        onResumeNative(jniClassHandle);
    }

    @Override
    public void onStop() {
        onStopNative(jniClassHandle);
    }
}