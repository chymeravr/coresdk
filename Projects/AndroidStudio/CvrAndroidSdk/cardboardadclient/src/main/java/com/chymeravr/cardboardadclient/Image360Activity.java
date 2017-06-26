package com.chymeravr.cardboardadclient;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import com.chymeravr.analytics.EventPriority;
import com.chymeravr.common.Config;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.eventreceiver.RuntimeAdMeta;
import com.chymeravr.schemas.eventreceiver.SDKEvent;
import com.google.vr.ndk.base.GvrLayout;
import com.google.vr.ndk.base.AndroidCompat;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import lombok.Getter;

/**
 * Created by robin_chimera on 5/10/2017.
 */

public final class Image360Activity extends Activity {
    private static final String TAG = "Image360CardbrdActivity";

    // google vr layout for holding the surface
    private GvrLayout gvrLayout;

    // native C++ handle for image360 application
    private long nativeImage360ActivityHandle;

    // gl surface on which the rendering happens
    private GLSurfaceView surfaceView;

    // load native libraries
    static{
        // gvr libs
        System.loadLibrary("gvr");

        // ChymeraVr Libs
        System.loadLibrary("CardboardAdClient");
    }

    private String clickUrl;

    // TODO: 4/27/2017 remove getter - clients should not get these from here
    // ad identfication ids
    @Getter
    private String servingId;

    @Getter
    private int instanceId;

    // we need the class name to go transition back to
    private String returningClassName;

    // the function talks to analytics manager as and when called
    public void emitEvent(EventType eventType, EventPriority priority, HashMap<String, String> map){
        long currTime = new Timestamp(System.currentTimeMillis()).getTime();
        RuntimeAdMeta adMeta = new RuntimeAdMeta(this.getServingId(), this.getInstanceId());
        SDKEvent event = new SDKEvent(currTime, eventType, adMeta);
        event.setParamsMap(map);
        ChymeraVrSdk.getSdkInstance().getAnalyticsManager().push(event, priority);
    }

    // This is done on the GL thread because refreshViewerProfile isn't thread-safe.
    private final Runnable refreshViewerProfileRunnable =
            new Runnable() {
                @Override
                public void run() {
                    gvrLayout.getGvrApi().refreshViewerProfile();
                }
            };

    // scheduler polls the hmd for parameters (quaternion) at regular intervals
    private ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);

    private final Runnable hmdPollRunner = new Runnable() {
        public void run() {
            Image360Activity.this.getCardboardHmdParams();
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");

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

        // get app file path
        String basePath = this.getFilesDir().getAbsolutePath();

        // Fetch url to show when user clicks
        Intent intent = getIntent();
        this.clickUrl = intent.getStringExtra("clickUrl");

        String imageAdFilePath = intent.getStringExtra("imageAdFilePath");

        this.instanceId = intent.getIntExtra("instanceId", -1);
        this.servingId = intent.getStringExtra("servingId");
        this.returningClassName = intent.getStringExtra("returningClass");

        // Initialize GvrLayout and the native renderer.
        gvrLayout = new GvrLayout(this);
        nativeImage360ActivityHandle =
                nativeCreateRenderer(
                        getClass().getClassLoader(),
                        this.getApplicationContext(),
                        gvrLayout.getGvrApi().getNativeGvrContext(), basePath, imageAdFilePath);


        nativeOnStart(nativeImage360ActivityHandle);

        // Add the GLSurfaceView to the GvrLayout.
        surfaceView = new GLSurfaceView(this);
        surfaceView.setEGLContextClientVersion(2);
        surfaceView.setEGLConfigChooser(8, 8, 8, 0, 0, 0);
        surfaceView.setPreserveEGLContextOnPause(true);
        surfaceView.setRenderer(
                new GLSurfaceView.Renderer() {
                    @Override
                    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                        Log.d(TAG, "GL Surface Created");
                        Image360Activity.this.emitEvent(EventType.AD_SHOW, EventPriority.HIGH, null);

                        nativeInitializeGl(nativeImage360ActivityHandle);

                        scheduler.scheduleAtFixedRate(hmdPollRunner, Config.getHmdSamplingDelay(),
                                Config.getHmdSamplingPeriod(), TimeUnit.SECONDS);
                    }

                    @Override
                    public void onSurfaceChanged(GL10 gl, int width, int height) {
                    }

                    @Override
                    public void onDrawFrame(GL10 gl) {
                        // call native draw function
                        nativeDrawFrame(nativeImage360ActivityHandle);

                        if(nativeCloseAd(nativeImage360ActivityHandle)){

                            Image360Activity.this.finish();
                        }
                    }
                });
        surfaceView.setOnTouchListener(
                new View.OnTouchListener() {
                    @Override
                    public boolean onTouch(View v, MotionEvent event) {
                        if (event.getAction() == MotionEvent.ACTION_DOWN) {
                            // Give user feedback and signal a trigger event.
                            ((Vibrator) getSystemService(Context.VIBRATOR_SERVICE)).vibrate(50);
                            // int keyEventResult = nativeOnTriggerEvent(nativeImage360ActivityHandler);
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
        nativeOnPause(nativeImage360ActivityHandle);
        gvrLayout.onPause();
        surfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume");
        nativeOnResume(nativeImage360ActivityHandle);
        gvrLayout.onResume();
        surfaceView.onResume();
        surfaceView.queueEvent(refreshViewerProfileRunnable);
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.d(TAG, "onStop");
        //nativeOnStop(nativeImage360ActivityHandle);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
        // Destruction order is important; shutting down the GvrLayout will detach
        // the GLSurfaceView and stop the GL thread, allowing safe shutdown of
        // native resources from the UI thread.

        // is this event right?
        this.emitEvent(EventType.AD_CLOSE, EventPriority.HIGH, null);
        nativeDestroyRenderer(nativeImage360ActivityHandle);

        gvrLayout.shutdown();
        this.scheduler.shutdown();
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
        // Avoid accidental volume key presses while the phone is in the VR headset.
        if(event.getAction() == KeyEvent.ACTION_UP){
            return true;
        }
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

    private void getCardboardHmdParams(){
        if(this.nativeImage360ActivityHandle != 0){
            float[] hmdParams = this.nativeGetHmdParams(this.nativeImage360ActivityHandle);
            HashMap<String, String> hmdEyeMap = new HashMap<>();

            // order in which gvr gives their quaternions - JPL format
            String[] parameterKeys = {"qx", "qy", "qz", "qw"};

            int i = 0;
            for (String key : parameterKeys) {
                hmdEyeMap.put(key, String.valueOf(hmdParams[i++]));

            }
            this.emitEvent(EventType.AD_VIEW_METRICS, EventPriority.LOW, hmdEyeMap);
        }
    }

    private void onDownloadClick(){
        // handle what happens when user clicks on notification button
    }

    private native long nativeCreateRenderer(
            ClassLoader appClassLoader, Context context, long nativeGvrContext, String appDir, String Image360AdFileName);

    private native void nativeOnStart(long nativeImage360ActivityHandler);

    private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);

    private native void nativeInitializeGl(long nativeTreasureHuntRenderer);

    private native long nativeDrawFrame(long nativeTreasureHuntRenderer);

    private native void nativeOnPause(long nativeTreasureHuntRenderer);

    private native void nativeOnResume(long nativeTreasureHuntRenderer);

    //private native void nativeOnStop(long nativeTreasureHuntRenderer);

    private native float[] nativeGetHmdParams(long nativeTreasureHuntRenderer);

    private native boolean nativeCloseAd(long nativeImage360ActivityHandle);
}
