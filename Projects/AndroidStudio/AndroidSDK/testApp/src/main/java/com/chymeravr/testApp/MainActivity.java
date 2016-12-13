package com.chymeravr.testApp;


import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.location.Location;
import android.net.Uri;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.adclient.AdListener;
import com.chymeravr.adclient.AdRequest;
import com.chymeravr.adclient.ChymeraVRAndroidSDK;
import com.chymeravr.adclient.Image360Ad;

import java.text.SimpleDateFormat;
import java.util.Date;

public class MainActivity extends AppCompatActivity {
    String msg = "TestAppImg360";

    Image360Ad imageAd;//
    Button button;
    ImageView imgView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.v(msg, "Creating Activity");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        addListenerOnButton();
        imgView = (ImageView) this.findViewById(R.id.ImageView);


        // deprecated setting, but required on Android versions prior to 3.0
        //surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        ChymeraVRAndroidSDK.initialize(this, "testCode");

        this.imageAd = new Image360Ad("testImageAd", this);

        this.imageAd.setAdListener(new AdListener() {
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
        final AdRequest adRequest = new AdRequest.Builder()
                .setLocation(location)
                .setBirthday(dob)
                .setGender(AdRequest.Gender.MALE).build();
        Toast.makeText(this, "About to start Loading . . . " + this.imageAd.isLoading + " " + this.imageAd.isLoading(),
                Toast.LENGTH_LONG).show();


        this.imageAd.loadAd(adRequest);
        Toast.makeText(this, "Loading . . . " + this.imageAd.isLoading + " " + this.imageAd.isLoading(),
                Toast.LENGTH_LONG).show();
    }

    /**
     * Called when the activity has become visible.
     */
    @Override
    protected void onResume() {
        super.onResume();
        Log.d(msg, "The onResume() event");
    }

    /**
     * Called when another activity is taking focus.
     */
    @Override
    protected void onPause() {
        super.onPause();
        Log.d(msg, "The onPause() event");
    }

    /**
     * Called when the activity is no longer visible.
     */
    @Override
    protected void onStop() {
        super.onStop();
        Log.d(msg, "The onStop() event");
    }

    /**
     * Called just before the activity is destroyed.
     */
    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(msg, "The onDestroy() event");
    }

    public void addListenerOnButton() {

        button = (Button) findViewById(R.id.button1);

        button.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View arg0) {

                MainActivity.this.imageAd.show();

                byte[] testByteArray = imageAd.getByteArray();
                Toast.makeText(MainActivity.this, "Success!!! " + MainActivity.this.imageAd.isLoading + " "
                        + MainActivity.this.imageAd.isLoading() +
                                (testByteArray == null),
                        Toast.LENGTH_LONG).show();
                Log.v(msg, "Testing image loading");

                if(MainActivity.this.imageAd.isLoaded()){
                    Bitmap bmp = MainActivity.this.imageAd.getImageBitmap();
                    Bitmap mutableBitmap = bmp.copy(Bitmap.Config.ARGB_8888, true);
                    Log.v(msg, "Bitmap : " +  mutableBitmap.getPixel(1, 1) + " " + mutableBitmap.getPixel(2, 2) + " ");

                    Canvas canvas = new Canvas(mutableBitmap);
                    Paint paint = new Paint();
                    canvas.drawBitmap(bmp, 0, 0, paint);
                    MainActivity.this.imgView.setImageBitmap(mutableBitmap);

                } else if(MainActivity.this.imageAd.isLoading()){
                    Toast.makeText(MainActivity.this, "Still loading an image ad. No image360Ad to show.",
                            Toast.LENGTH_LONG).show();
                }else{
                    Toast.makeText(MainActivity.this, "No image ad requested. Load an ad and try again.",
                            Toast.LENGTH_LONG).show();
                }

            }

        });

    }

}
