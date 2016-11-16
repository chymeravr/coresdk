package co.chimeralabs.sdk.android.activity;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import co.chimeralabs.sdk.android.adapter.Image360JniAdapter;
import co.chimeralabs.sdk.android.adapter.Image360JniAdapterGearVR;


/**
 * Created by chimeralabs on 8/5/2016.
 */
public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";
    private Image360JniAdapter image360Adapter;

    public MainActivity(){
        image360Adapter = new Image360JniAdapterGearVR();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        image360Adapter.createImage360(this);
        String storeString = "Hello from Java";
        //image360Adapter.testSendToJni(storeString);
        //String msg = image360Adapter.testReceiveFromJni();
        //Log.i(TAG, msg);
    }

}
