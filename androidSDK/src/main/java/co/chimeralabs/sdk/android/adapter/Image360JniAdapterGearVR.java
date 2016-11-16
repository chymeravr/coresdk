package co.chimeralabs.sdk.android.adapter;

import android.app.Activity;
import android.view.Surface;
import android.view.SurfaceHolder;

/**
 * Created by chimeralabs on 8/4/2016.
 */

public class Image360JniAdapterGearVR implements Image360JniAdapter {
    private long jniClassHandle;

    static {
        System.loadLibrary("image360gear_jni");
    }

    public native long createImage360NativeGearVR(Activity obj);
    public native void onSurfaceCreatedNativeGearVR(long jniClassHandle, Surface surface);
    public native void onSurfaceChangedNativeGearVR(long jniClassHandle, Surface surface, int format, int width, int height);
    public native void onSurfaceDestroyedNativeGearVR(long jniClassHandle, Surface surface);
    public native void onResumeNativeGearVR(long jniClassHandle);
    public native void onPauseNativeGearVR(long jniClassHandle);
    public native void onStopNativeGearVR(long jniClassHandle);

    public void testSendToJni(String storeString) {
        //testReceiveFromJavaNative(jniClassHandle, storeString);
    }

    public String testReceiveFromJni() {
        //return testSendToJavaNative(jniClassHandle);
        return  null;
    }

    @Override
    public void createImage360(Activity object) {
        this.jniClassHandle = createImage360NativeGearVR(object);
    }

    @Override
    public void onSurfaceCreated(SurfaceHolder surfaceHolder) {
        onSurfaceCreatedNativeGearVR(this.jniClassHandle, surfaceHolder.getSurface());
    }

    @Override
    public void onSurfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height) {
        onSurfaceChangedNativeGearVR(this.jniClassHandle, surfaceHolder.getSurface(), format, width, height);
    }

    @Override
    public void onSurfaceDestroyed(SurfaceHolder surfaceHolder) {
        onSurfaceDestroyedNativeGearVR(jniClassHandle, surfaceHolder.getSurface());
    }

    @Override
    public void onPause() {
        onPauseNativeGearVR(jniClassHandle);
    }

    @Override
    public void onResume() {
        onResumeNativeGearVR(jniClassHandle);
    }

    @Override
    public void onStop() {
        onStopNativeGearVR(jniClassHandle);
    }
}
