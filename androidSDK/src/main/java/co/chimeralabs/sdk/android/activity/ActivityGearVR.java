package co.chimeralabs.sdk.android.activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import co.chimeralabs.sdk.android.adapter.Image360JniAdapter;
import co.chimeralabs.sdk.android.adapter.Image360JniAdapterGearVR;

/**
 * Created by chimeralabs on 9/15/2016.
 */
public class ActivityGearVR extends Activity implements SurfaceHolder.Callback {
    private String tag = "ActivityGearVR";

    private SurfaceView surfaceView;
    private Image360JniAdapter jniAdapter;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        jniAdapter = new Image360JniAdapterGearVR();
        jniAdapter.createImage360(this);
        surfaceView = new SurfaceView(this);
        setContentView(surfaceView);
        surfaceView.getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        jniAdapter.onSurfaceCreated(holder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        jniAdapter.onSurfaceChanged(holder, format, width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        jniAdapter.onSurfaceDestroyed(holder);
    }

    @Override
    public void onPause(){
        super.onPause();
        jniAdapter.onPause();
    }

    @Override
    public void onResume(){
        super.onResume();
        jniAdapter.onResume();
    }

    @Override
    public void onStop(){
        super.onStop();
        jniAdapter.onStop();
    }
}
