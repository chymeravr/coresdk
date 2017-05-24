package com.chymeravr.daydreamadclient;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.Config;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.eventreceiver.RuntimeAdMeta;
import com.chymeravr.schemas.eventreceiver.SDKEvent;
import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.DaydreamApi;
import com.google.vr.ndk.base.GvrLayout;
import com.google.vr.sdk.controller.Controller;
import com.google.vr.sdk.controller.ControllerManager;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import lombok.Getter;

/**
 * Created by robin_chimera on 3/22/2017.
 */

public final class Image360Activity extends Activity {

    private static final String TAG = "Image360AdDaydreamActivity";

    // google vr layout for holding the surface
    private GvrLayout gvrLayout;

    // daydream parameters - api object, controller etc.
    private DaydreamApi daydreamApi;
    private ControllerManager ddControllerManager;
    private Controller ddController;

    // native C++ handle for image360 application
    private long nativeImage360ActivityHandler;

    // gl surface on which rendering happens
    private GLSurfaceView surfaceView;

    // count vars to ensure we only notify once / close once
    // TODO: 4/27/2017 change this to bools
    private int notifyCount = 1;
    private int closeCount = 1;

    // enum representing possible controller click responses
    private enum ControllerClickEventResponse{
        NO_EVENT,
        DOWNLOAD,
        CLOSE_AD
    }

    // load native libraries
    static {
        // gvr libs
        System.loadLibrary("gvr");
        //System.loadLibrary("gvr_audio");
        // chymeravr libs
        System.loadLibrary("DaydreamAdClient");
    }

    // below params are passed from Image360Ad class which in turn fetched it from server and passed to activity
    // url to send in notification
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
    public void emitEvent(EventType eventType, AnalyticsManager.Priority priority, HashMap<String, String> map){
        long currTime = new Timestamp(System.currentTimeMillis()).getTime();
        RuntimeAdMeta adMeta = new RuntimeAdMeta(this.getServingId(), this.getInstanceId());
        SDKEvent event = new SDKEvent(currTime, eventType, adMeta);
        event.setParamsMap(map);
        AnalyticsManager.push(event, priority);
    }

    // This is done on the GL thread because refreshViewerProfile isn't thread-safe.
    private final Runnable refreshViewerProfileRunnable =
            new Runnable() {
                @Override
                public void run() {
                    gvrLayout.getGvrApi().refreshViewerProfile();
                }
            };


    // ddController is not thread safe - we call this from the rendering thread and execute on main UI thread
    private final Runnable r = new Runnable() {
        public void run() {
            if(ddController.clickButtonState) {

                ControllerClickEventResponse keyEventResponse = ControllerClickEventResponse.values()[nativeOnControllerClicked(nativeImage360ActivityHandler)];

                switch (keyEventResponse){
                    case NO_EVENT:
                        break;
                    case DOWNLOAD:
                        //notifyUser();
                        onDownloadClick();
                        finishAdActivity();
                        break;
                    case CLOSE_AD:
                        finishAdActivity();
                        break;
                }
            }
        }
    };

    // gracefully exit the vr ad back to users add - daydream provides a neat way to do this
    // only call this function once
    private void finishAdActivity(){

        if(closeCount <= 0){
            return;
        }
        closeCount--;
        Class<?> clientClass;
        try {
            // return to client activity (class) that called the ad show on closing
            clientClass = Class.forName(Image360Activity.this.returningClassName);
            ComponentName c= new ComponentName(Image360Activity.this, clientClass);
            Intent vrIntent = DaydreamApi.createVrIntent(c);
            daydreamApi.launchInVr(vrIntent);
        } catch (ClassNotFoundException e) {
            Log.e(TAG, e.toString());
        }
    }

    // scheduler polls the hmd for parameters (quaternion) at regular intervals
    private ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);

    private final Runnable hmdPollRunner = new Runnable() {
        public void run() {
            Log.v(TAG, "Running Scheduler");
            Image360Activity.this.getDaydreamHmdParams();
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");

        // initialize daydream api
        this.daydreamApi = DaydreamApi.create(this);

        // initialize daydream controller manager
        this.ddControllerManager = new ControllerManager(this, new ControllerManager.EventListener() {
            @Override
            public void onApiStatusChanged(int i) {
                Log.d(TAG, "Api Status Changed");
            }

            @Override
            public void onRecentered() {
                Log.d(TAG, "Recentered");
            }
        });

        // initialize daydream controller
        this.ddController = this.ddControllerManager.getController();

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
        nativeImage360ActivityHandler =
                nativeCreateRenderer(
                        getClass().getClassLoader(),
                        this.getApplicationContext(),
                        gvrLayout.getGvrApi().getNativeGvrContext(), basePath, imageAdFilePath);


        nativeOnStart(nativeImage360ActivityHandler);

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
                        Image360Activity.this.emitEvent(EventType.AD_SHOW, AnalyticsManager.Priority.HIGH, null);

                        nativeInitializeGl(nativeImage360ActivityHandler);

                        scheduler.scheduleAtFixedRate(hmdPollRunner, Config.hmdSamplingDelay, Config.hmdSamplingPeriod, TimeUnit.SECONDS);
                    }

                    @Override
                    public void onSurfaceChanged(GL10 gl, int width, int height) {
                    }

                    @Override
                    public void onDrawFrame(GL10 gl) {
                        // call native draw function
                        nativeDrawFrame(nativeImage360ActivityHandler);

                        // update controller status
                        ddController.update();

                        // daydream functions are not thread safe - run them on UI thread
                        Image360Activity.this.runOnUiThread(r);
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
        ddControllerManager.start();
        nativeOnPause(nativeImage360ActivityHandler);
        gvrLayout.onPause();
        surfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume");
        ddControllerManager.start();
        nativeOnResume(nativeImage360ActivityHandler);
        gvrLayout.onResume();
        surfaceView.onResume();
        surfaceView.queueEvent(refreshViewerProfileRunnable);

        // TODO: 4/27/2017 remove hardcoded thread pool size
        //this.scheduler = Executors.newScheduledThreadPool(1);
        //this.scheduler.scheduleAtFixedRate(hmdPollRunner, 1, 1, TimeUnit.SECONDS);
//        final ScheduledFuture<?> beeperHandle =
//                scheduler.scheduleAtFixedRate(hmdPollRunner, 1, 1, SECONDS);

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
        // Destruction order is important; shutting down the GvrLayout will detach
        // the GLSurfaceView and stop the GL thread, allowing safe shutdown of
        // native resources from the UI thread.
        this.emitEvent(EventType.AD_CLICK, AnalyticsManager.Priority.HIGH, null);
        gvrLayout.shutdown();
        nativeDestroyRenderer(nativeImage360ActivityHandler);
        this.ddControllerManager.stop();
        this.daydreamApi.close();
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

    private void getDaydreamHmdParams(){
        if(this.nativeImage360ActivityHandler != 0){
            float[] hmdParams = this.nativeGetHmdParams(this.nativeImage360ActivityHandler);
            HashMap<String, String> hmdEyeMap = new HashMap<>();

            // order in which daydream gives their quaternions - JPL format
            String[] parameterKeys = {"qx", "qy", "qz", "qw"};

            int i = 0;
            for (String key : parameterKeys) {
                hmdEyeMap.put(key, String.valueOf(hmdParams[i++]));

            }
            Log.d(TAG, "getDaydreamHmdParams: "
                    + hmdParams[0] + ", "
                    + hmdParams[1] + ", "
                    + hmdParams[2] + ", "
                    + hmdParams[3]);
            this.emitEvent(EventType.AD_VIEW_METRICS, AnalyticsManager.Priority.LOW, hmdEyeMap);
        }
    }

    private void onDownloadClick(){
        // send user to vr play store of advertiser for starting download
        // exit from ad

        Intent launchIntent = new Intent(Intent.ACTION_VIEW);

        launchIntent.addCategory(Intent.CATEGORY_LAUNCHER);

        //launchIntent.setComponent(cp);
        launchIntent.setData(Uri.parse("https://play.google.com/vr/store/apps/details?id=com.google.android.apps.youtube.vr"));
        //startActivity(launchIntent);

        ComponentName playStoreComponent = new ComponentName("com.google.android.vr.home", "com.google.android.apps.vr.playstore.PlayStoreActivity");
        Intent downloadAppIntent = daydreamApi.createVrIntent(playStoreComponent);
        downloadAppIntent.setData(Uri.parse("https://play.google.com/vr/store/apps/details?id=" + this.clickUrl));
        daydreamApi.launchInVr(downloadAppIntent);
    }

    private native long nativeCreateRenderer(
            ClassLoader appClassLoader, Context context, long nativeGvrContext, String appDir, String Image360AdFileName);

    private native void nativeOnStart(long nativeImage360ActivityHandler);

    private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);

    private native void nativeInitializeGl(long nativeTreasureHuntRenderer);

    private native long nativeDrawFrame(long nativeTreasureHuntRenderer);

    private native int nativeOnControllerClicked(long nativeTreasureHuntRenderer);

    private native void nativeOnPause(long nativeTreasureHuntRenderer);

    private native void nativeOnResume(long nativeTreasureHuntRenderer);

    private native float[] nativeGetHmdParams(long nativeTreasureHuntRenderer);
}

