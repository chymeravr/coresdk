package co.chimeralabs.sdk.android.adapter;

/**
 * Created by Sushil on 8/4/2016.
 */

import android.app.Activity;
import android.view.SurfaceHolder;

/**
 * This Class is the interface between Java code and Jni code of Android
 * Any call from Java should be made through this interface
 */

public interface Image360JniAdapter {
    public void createImage360(Activity object);
    public void onSurfaceCreated(SurfaceHolder surfaceHolder);
    public void onSurfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height);
    public void onSurfaceDestroyed(SurfaceHolder surfaceHolder);
    public void onPause();
    public void onResume();
    public void onStop();
    //public void testSendToJni(String storeString);
    //public String testReceiveFromJni();
}
