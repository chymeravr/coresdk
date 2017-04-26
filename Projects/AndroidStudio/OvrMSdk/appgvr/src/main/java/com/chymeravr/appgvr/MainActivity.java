/* Copyright 2017 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.chymeravr.appgvr;

import android.app.Activity;
import android.content.Context;
import android.location.Location;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import com.chymeravr.adclient.VrAdListener;
import com.chymeravr.adclient.VrAdRequest;
import com.chymeravr.gvradclient.ChymeraVrSdk;
import com.chymeravr.gvradclient.Image360Ad;
import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.DaydreamApi;
import com.google.vr.ndk.base.GvrLayout;

import java.text.SimpleDateFormat;
import java.util.Date;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * A Gvr API sample application.
 */
public class MainActivity extends Activity {
    private GvrLayout gvrLayout;
    private long nativeTreasureHuntRenderer;
    private GLSurfaceView surfaceView;

    Image360Ad image360TestAd;
    public boolean isShowing = false;

    private static final String TAG = "CVRAppGvr";
    // This is done on the GL thread because refreshViewerProfile isn't thread-safe.
    private final Runnable refreshViewerProfileRunnable =
            new Runnable() {
                @Override
                public void run() {
                    gvrLayout.getGvrApi().refreshViewerProfile();
                }
            };

    private DaydreamApi daydreamApi;
    private VrAdRequest vrAdRequest;
    static {
        System.loadLibrary("gvr");
        System.loadLibrary("gvr_audio");
        System.loadLibrary("appgvr");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");

        this.daydreamApi = DaydreamApi.create(this);

        ChymeraVrSdk.initialize(this, "89ec2db1-284e-44af-968e-0618103f89c8");

        this.image360TestAd = new Image360Ad(this, new VrAdListener() {
            @Override
            public void onVrAdLoaded() {
            }

            @Override
            public void onVrAdLoadFailed(VrAdRequest.Error error, String errorReason) {
            }

            @Override
            public void onVrAdOpened() {
            }

            @Override
            public void onVrAdClosed() {
            }
        });
        this.image360TestAd.setDaydreamApi(this.daydreamApi);
        this.image360TestAd.setPlacementId("3efc7f15-33a6-4480-bb71-3bd74aca4f1f");

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
        vrAdRequest = VrAdRequest.builder()
                .location(location)
                .birthday(dob)
                .gender(VrAdRequest.Gender.MALE).build();


        this.image360TestAd.loadAd(vrAdRequest);

        // Ensure fullscreen immersion.
        setImmersiveSticky();
        getWindow()
                .getDecorView()
                .setOnSystemUiVisibilityChangeListener(
                        new View.OnSystemUiVisibilityChangeListener() {
                            @Override
                            public void onSystemUiVisibilityChange(int visibility) {
                                if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                                    setImmersiveSticky();
                                }
                            }
                        });

        // Initialize GvrLayout and the native renderer.
        gvrLayout = new GvrLayout(this);
        nativeTreasureHuntRenderer =
                nativeCreateRenderer(
                        getClass().getClassLoader(),
                        this.getApplicationContext(),
                        gvrLayout.getGvrApi().getNativeGvrContext());

        // Add the GLSurfaceView to the GvrLayout.
        surfaceView = new GLSurfaceView(this);
        surfaceView.setEGLContextClientVersion(2);
        surfaceView.setEGLConfigChooser(8, 8, 8, 0, 0, 0);
        surfaceView.setPreserveEGLContextOnPause(true);
        surfaceView.setRenderer(
                new GLSurfaceView.Renderer() {
                    @Override
                    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                        nativeInitializeGl(nativeTreasureHuntRenderer);
                    }

                    @Override
                    public void onSurfaceChanged(GL10 gl, int width, int height) {
                    }

                    @Override
                    public void onDrawFrame(GL10 gl) {
                        nativeDrawFrame(nativeTreasureHuntRenderer);
                    }
                });
        surfaceView.setOnTouchListener(
                new View.OnTouchListener() {
                    @Override
                    public boolean onTouch(View v, MotionEvent event) {
                        if (event.getAction() == MotionEvent.ACTION_DOWN) {
                            // Give user feedback and signal a trigger event.
                            ((Vibrator) getSystemService(Context.VIBRATOR_SERVICE)).vibrate(50);
                            nativeOnTriggerEvent(nativeTreasureHuntRenderer);
                            return true;
                        }
                        return false;
                    }
                });
        gvrLayout.setPresentationView(surfaceView);

        // Add the GvrLayout to the View hierarchy.
        setContentView(gvrLayout);

        // Enable scan line racing.
        if (gvrLayout.setAsyncReprojectionEnabled(true)) {
            // Scanline racing decouples the app framerate from the display framerate,
            // allowing immersive interaction even at the throttled clockrates set by
            // sustained performance mode.
            AndroidCompat.setSustainedPerformanceMode(this, true);
        }

        // Enable VR Mode.
        AndroidCompat.setVrModeEnabled(this, true);

        // Prevent screen from dimming/locking.
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "onPause");
        nativeOnPause(nativeTreasureHuntRenderer);
        gvrLayout.onPause();
        surfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume");
        nativeOnResume(nativeTreasureHuntRenderer);
        gvrLayout.onResume();
        surfaceView.onResume();
        surfaceView.queueEvent(refreshViewerProfileRunnable);

        ChymeraVrSdk.shutdown();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
        // Destruction order is important; shutting down the GvrLayout will detach
        // the GLSurfaceView and stop the GL thread, allowing safe shutdown of
        // native resources from the UI thread.
        gvrLayout.shutdown();
        nativeDestroyRenderer(nativeTreasureHuntRenderer);
        //image360TestAd.destroy();
        this.daydreamApi.close();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        Log.d(TAG, "onWindowFocusChanged");
        if (hasFocus) {
            setImmersiveSticky();
        }
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        Log.d(TAG, "dispatchKeyEvent");

        Log.d(TAG, "Going to show u an ad now . . . .");
        //this.isShowing = true;

        if(event.getAction() == KeyEvent.ACTION_UP){
            return false;
        }

        this.image360TestAd.show();
        this.image360TestAd.loadAd(vrAdRequest);
        // Avoid accidental volume key presses while the phone is in the VR headset.
        if (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP
                || event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_DOWN) {
            return true;
        }

        return super.dispatchKeyEvent(event);
    }

    private void setImmersiveSticky() {
        Log.d(TAG, "setImmersiveSticky");
        getWindow()
                .getDecorView()
                .setSystemUiVisibility(
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                | View.SYSTEM_UI_FLAG_FULLSCREEN
                                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    private native long nativeCreateRenderer(
            ClassLoader appClassLoader, Context context, long nativeGvrContext);

    private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);

    private native void nativeInitializeGl(long nativeTreasureHuntRenderer);

    private native long nativeDrawFrame(long nativeTreasureHuntRenderer);

    private native void nativeOnTriggerEvent(long nativeTreasureHuntRenderer);

    private native void nativeOnPause(long nativeTreasureHuntRenderer);

    private native void nativeOnResume(long nativeTreasureHuntRenderer);
}
