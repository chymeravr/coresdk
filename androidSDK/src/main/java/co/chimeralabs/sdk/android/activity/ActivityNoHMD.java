package co.chimeralabs.sdk.android.activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import co.chimeralabs.sdk.android.adapter.Image360JniAdapter;
import co.chimeralabs.sdk.android.adapter.Image360JniAdapterNoHMD;

/**
 * Created by chimeralabs on 8/5/2016.
 */
public class ActivityNoHMD extends Activity implements SurfaceHolder.Callback {
    private static String TAG = "ActivityNoHMD";

    private SurfaceView surfaceView;
    private Image360JniAdapter image360Adapter;

    @Override
    public void onCreate(Bundle savedInstanceState){
        image360Adapter = new Image360JniAdapterNoHMD();
        super.onCreate(savedInstanceState);
        image360Adapter.createImage360(this);
        surfaceView = new SurfaceView(this);
        setContentView(surfaceView);
        surfaceView.getHolder().addCallback(this);
    }


    /***********************************************************************************************
     *
     * Surface Callbacks
     *
     * ********************************************************************************************/

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        image360Adapter.onSurfaceCreated(holder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        image360Adapter.onSurfaceChanged(holder, format, width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        image360Adapter.onSurfaceDestroyed(holder);
    }

    @Override
    public void onPause(){
        super.onPause();
        image360Adapter.onPause();
    }

    @Override
    public void onResume(){
        super.onResume();
        image360Adapter.onResume();
    }

    @Override
    public void onStop(){
        super.onStop();
        image360Adapter.onStop();
    }
}
