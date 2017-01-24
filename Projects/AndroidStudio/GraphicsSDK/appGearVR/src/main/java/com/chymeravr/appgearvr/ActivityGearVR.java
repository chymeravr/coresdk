package com.chymeravr.appgearvr;

import android.app.Activity;
import android.location.Location;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import com.chymeravr.adclient.AdListener;
import com.chymeravr.adclient.AdRequest;
import com.chymeravr.adclient.Image360Ad;

import java.text.SimpleDateFormat;
import java.util.Date;


public class ActivityGearVR extends Activity {

    public static final String TAG = "Image360::ActivityGVR";

    Image360Ad image360TestAd;

    @Override
    protected void onCreate(Bundle icicle) {
        Log.d(TAG, "onCreate()");
        super.onCreate(icicle);

        this.image360TestAd = new Image360Ad("testImage360Ad", this, new AdListener() {
            @Override
            public void onAdLoaded() {
            }

            @Override
            public void onAdFailedToLoad() {
            }

            @Override
            public void onAdOpened() {
            }

            @Override
            public void onAdClosed() {
            }

            @Override
            public void onAdLeftApplication() {
            }
        });
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
        super.onDestroy();
        this.image360TestAd.onDestroy();
    }


    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        Log.d(TAG, "dispatchKey - destroy add leave vr etc");
//        this.image360TestAd.onStop();
//        boolean result =  this.image360TestAd.dispatchKeyEvent(event);
        this.image360TestAd.onDestroy();
        return true;
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        if (this.image360TestAd.isLoaded()) {
            this.image360TestAd.show();
        }

        return this.image360TestAd.dispatchTouchEvent(event);
    }


}
