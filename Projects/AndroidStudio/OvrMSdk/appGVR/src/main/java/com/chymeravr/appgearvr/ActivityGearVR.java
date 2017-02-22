package com.chymeravr.appgearvr;

import android.app.Activity;
import android.location.Location;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import com.chymeravr.adclient.AdListener;
import com.chymeravr.adclient.AdRequest;
import com.chymeravr.adclient.ChymeraVrSdk;
import com.chymeravr.adclient.Image360Ad;

import java.text.SimpleDateFormat;
import java.util.Date;



public class ActivityGearVR extends Activity {

    public static final String TAG = "Image360::TestApp";

    Image360Ad image360TestAd;

    public boolean isShowing = false;

    @Override
    protected void onCreate(Bundle icicle) {
        Log.d(TAG, "onCreate()");
        super.onCreate(icicle);

        ChymeraVrSdk.initialize(this, "89ec2db1-284e-44af-968e-0618103f89c8");

        this.image360TestAd = new Image360Ad(this, new AdListener() {
            @Override
            public void onAdLoaded() {
            }

            @Override
            public void onAdLoadFailed(AdRequest.Error error, String errorReason) {
            }

            @Override
            public void onAdOpened() {
            }

            @Override
            public void onAdClosed() {
            }
        });
        this.image360TestAd.setPlacementId("3efc7f15-33a6-4480-bb71-3bd74aca4f1f");
    }

    @Override
    protected void onStart() {
        Log.d(TAG, "onStart()");
        super.onStart();
        final String PROVIDER = "flp";
        final double lat = 37.377166;
        final double lng = -122.086966;
        final float accuracy = 3.0f;

         /* From input arguments, create a single Location with provider set to
         * "flp"*/

        // Create a new Location
        Location newLocation = new Location(PROVIDER);
        newLocation.setLatitude(lat);
        newLocation.setLongitude(lng);
        newLocation.setAccuracy(accuracy);

        Location location = newLocation;

        String date = 1989 + "/" + 7 + "/" + 15;
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy/MM/dd");
        Date dob = new Date();
        try {
            dob = formatter.parse(date);
        } catch (Exception e) {
            e.printStackTrace();
        }
        final AdRequest adRequest = AdRequest.builder()
                .location(location)
                .birthday(dob)
                .gender(AdRequest.Gender.MALE).build();


        this.image360TestAd.loadAd(adRequest);
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume()");
        super.onResume();

    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause()");
        super.onPause();
    }

    @Override
    protected void onStop() {
        Log.d(TAG, "onStop()");
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy()");
        ChymeraVrSdk.shutdown();
        super.onDestroy();
    }


    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        Log.d(TAG, "dispatchKeyEvent");
        if(event.getAction()==KeyEvent.ACTION_UP){
            return false;
        }
        if (this.image360TestAd.isLoaded() && !this.isShowing) {
            this.isShowing = true;
            this.image360TestAd.show();
        } else {
            Log.d(TAG, "No Ad Loaded!! Try again after some time.");
        }
        return true;
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        // display an ad. a bool flag is created to ensure multiple calls to render an ad are not made
        // Its expensive to show an ad - it creates an activity - intializes native code etc.
//        if (this.image360TestAd.isLoaded() && !this.isShowing) {
//            this.isShowing = true;
//            this.image360TestAd.show();
//        } else {
//            Log.d(TAG, "No Ad Loaded!! Try again after some time.");
//        }
        Log.d(TAG, "dispatchTouchEvent");

        return true;
    }

}
