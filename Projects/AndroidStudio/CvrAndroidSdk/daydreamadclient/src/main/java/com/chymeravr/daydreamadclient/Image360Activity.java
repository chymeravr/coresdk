package com.chymeravr.daydreamadclient;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
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
import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.DaydreamApi;
import com.google.vr.ndk.base.GvrLayout;
import com.google.vr.sdk.controller.Controller;
import com.google.vr.sdk.controller.Controller.ConnectionStates;
import com.google.vr.sdk.controller.ControllerManager;
import com.google.vr.sdk.controller.ControllerManager.ApiStatus;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


/**
 * Created by robin_chimera on 3/22/2017.
 * Defines Activity for Image360 Ad
 */

public final class Image360Activity extends Activity {

    private static final String TAG = "Img360DaydreamActivity";

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
        // chymeravr libs
        System.loadLibrary("DaydreamAdClient");
    }

    // google vr layout for holding the surface
    private GvrLayout gvrLayout;
    // gl surface on which rendering happens
    private GLSurfaceView surfaceView;

    // daydream parameters - api object, controller etc.
    private DaydreamApi daydreamApi;
//    private ControllerManager controllerManager;
//    private Controller controller;

    // native C++ handle for image360 application
    private long nativeImage360ActivityHandler;

    // count vars to ensure we only notify once / close once
    private boolean isDownloadClicked = false;
    private boolean isCloseClicked = false;


    // below params are passed from Image360Ad class which in turn fetched it from server
    // and passed to activity
    // url to open daydream play store when use clicks action button
    private String clickUrl;

    // ad identfication ids
    private String servingId;
    private int instanceId;

    private Intent clientClassReturningIntent;

    // scheduler polls the hmd for parameters (quaternion) at regular intervals
    private ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);

    // The various events we need to handle happen on arbitrary threads. They need to be reposted to
    // the UI thread in order to manipulate the TextViews. This is only required if your app needs to
    // perform actions on the UI thread in response to controller events.
    private Handler uiHandler = new Handler();

    EventListener listener = new EventListener();

    // the function talks to analytics manager as and when called
    public void emitEvent(EventType eventType, EventPriority priority, HashMap<String, String> map){
        long currTime = new Timestamp(System.currentTimeMillis()).getTime();
        RuntimeAdMeta adMeta = new RuntimeAdMeta(this.servingId, this.instanceId);
        SDKEvent event = new SDKEvent(currTime, eventType, adMeta);
        event.setParamsMap(map);
        ChymeraVrSdk.getAnalyticsManager().push(event, priority);
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
//    private final Runnable controllerThread = new Runnable() {
//        public void run() {
//            if(Image360Activity.this.controller.clickButtonState) {
//                Log.d(TAG, "Controller Click DDController");
//                ControllerClickEventResponse keyEventResponse
//                        = ControllerClickEventResponse.values()[nativeOnControllerClicked(nativeImage360ActivityHandler)];
//
//                switch (keyEventResponse){
//                    case NO_EVENT:
//                        Log.d(TAG, "No Event");
//                        break;
//                    case DOWNLOAD:
//                        Log.d(TAG, "Download Event");
//                        onDownloadClick();
//                        finishAdActivity();
//                        break;
//                    case CLOSE_AD:
//                        Log.d(TAG, "Close Event");
//                        finishAdActivity();
//                        break;
//                }
//            }
//        }
//    };

    // gracefully exit the vr ad back to users add - daydream provides a neat way to do this
    // only call this function once
    public void finishAdActivity(){
        Log.d(TAG, "Finishing Ad Activity");
        if(this.isCloseClicked){
            return;
        }
        this.isCloseClicked = true;
        this.daydreamApi.launchInVr(this.clientClassReturningIntent);
    }

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
//        this.controllerManager = new ControllerManager(this, listener);

        // initialize daydream controller
//        this.controller = this.controllerManager.getController();
//        this.controller.setEventListener(listener);

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
        String returningClassName = intent.getStringExtra("returningClass");

        // Instantiate an intent to return to calling activity when ad finishes
        Class<?> clientClass;
        try {
            // return to client activity (class) that called the ad show on closing
            clientClass = Class.forName(returningClassName);
            ComponentName c= new ComponentName(this, clientClass);
            this.clientClassReturningIntent = DaydreamApi.createVrIntent(c);
        } catch (ClassNotFoundException e) {
            Log.e(TAG, e.toString());
        }

        // Initialize GvrLayout and the native renderer.
        gvrLayout = new GvrLayout(this);

        String CONTROLLER_TEXTURE_FILE_PATH = "chymeraSDKAssets/textures/ddcontroller_idle.png";
        String CONTROLLER_MODEL_FILE_PATH = "chymeraSDKAssets/models/ddController.obj";
        String LASER_BEAM_TEXTURE_FILE_PATH = "chymeraSDKAssets/textures/laserTexture.png";

        nativeImage360ActivityHandler =
                nativeCreateRenderer(this,
                        this.getApplicationContext(),
                        gvrLayout.getGvrApi().getNativeGvrContext(), basePath, imageAdFilePath,
                        CONTROLLER_MODEL_FILE_PATH, CONTROLLER_TEXTURE_FILE_PATH, LASER_BEAM_TEXTURE_FILE_PATH);


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
                        Image360Activity.this.emitEvent(EventType.AD_SHOW, EventPriority.HIGH, null);

                        nativeInitializeGl(nativeImage360ActivityHandler);

                        scheduler.scheduleAtFixedRate(hmdPollRunner, Config.getHmdSamplingDelay(),
                                Config.getHmdSamplingPeriod(), TimeUnit.SECONDS);
                    }

                    @Override
                    public void onSurfaceChanged(GL10 gl, int width, int height) {
                    }

                    @Override
                    public void onDrawFrame(GL10 gl) {
                        // call native draw function
                        nativeDrawFrame(nativeImage360ActivityHandler);

                        // update controller status
//                        Image360Activity.this.controller.update();

                        // daydream functions are not thread safe - run them on UI thread
                        //Image360Activity.this.runOnUiThread(Image360Activity.this.controllerThread);
//                        uiHandler.post(listener);
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
//        this.controllerManager.stop();
        nativeOnPause(this.nativeImage360ActivityHandler);
        this.gvrLayout.onPause();
        this.surfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume");
//        this.controllerManager.start();
        nativeOnResume(this.nativeImage360ActivityHandler);
        this.gvrLayout.onResume();
        this.surfaceView.onResume();
        this.surfaceView.queueEvent(this.refreshViewerProfileRunnable);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
        // Destruction order is important; shutting down the GvrLayout will detach
        // the GLSurfaceView and stop the GL thread, allowing safe shutdown of
        // native resources from the UI thread.
        this.emitEvent(EventType.AD_CLOSE, EventPriority.HIGH, null);
        this.gvrLayout.shutdown();
        nativeDestroyRenderer(nativeImage360ActivityHandler);
//        this.controllerManager.stop();
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
            this.emitEvent(EventType.AD_VIEW_METRICS, EventPriority.LOW, hmdEyeMap);
        }
    }

    private void onDownloadClick(){
        // send user to vr play store of advertiser for starting download
        // exit from ad
        this.emitEvent(EventType.AD_CLICK, EventPriority.HIGH, null);

        Intent launchIntent = new Intent(Intent.ACTION_VIEW);

        launchIntent.addCategory(Intent.CATEGORY_LAUNCHER);

        // NOTE : Bind Server Supplied address to this.
        launchIntent.setData(Uri.parse("https://play.google.com/vr/store/apps/details?id=com.google.android.apps.youtube.vr"));

        ComponentName playStoreComponent = new ComponentName("com.google.android.vr.home",
                "com.google.android.apps.vr.playstore.PlayStoreActivity");

        Intent downloadAppIntent = DaydreamApi.createVrIntent(playStoreComponent);
        downloadAppIntent.setData(Uri.parse("https://play.google.com/vr/store/apps/details?id=" + this.clickUrl));
        this.daydreamApi.launchInVr(downloadAppIntent);
    }

    // We receive all events from the Controller through this listener. In this example, our
    // listener handles both ControllerManager.EventListener and Controller.EventListener events.
    // This class is also a Runnable since the events will be reposted to the UI thread.
    private class EventListener extends Controller.EventListener
            implements ControllerManager.EventListener, Runnable {

        // The status of the overall controller API. This is primarily used for error handling since
        // it rarely changes.
        private String apiStatus;

        // The state of a specific Controller connection.
        private int controllerState = ConnectionStates.DISCONNECTED;

        @Override
        public void onApiStatusChanged(int state) {
            apiStatus = ApiStatus.toString(state);
            Log.d(TAG, "Controller API Status : " + apiStatus);
            uiHandler.post(this);
        }

        @Override
        public void onConnectionStateChanged(int state) {
            controllerState = state;
            Log.d(TAG, "Controller Connection State Changed : " + state);
            uiHandler.post(this);
        }

        @Override
        public void onRecentered() {
            // In a real GVR application, this would have implicitly called recenterHeadTracker().
            // Most apps don't care about this, but apps that want to implement custom behavior when a
            // recentering occurs should use this callback.
            //controllerOrientationView.resetYaw();
            Log.d(TAG, "Controller Recentered");
        }

        @Override
        public void onUpdate() {
            uiHandler.post(this);
        }

        // Update the various TextViews in the UI thread.
        @Override
        public void run() {
//            Image360Activity.this.controller.update();
//
//            if (controller.isTouching) {
//                Log.d(TAG,
//                        String.format("[%4.2f, %4.2f]", controller.touch.x, controller.touch.y));
//            }
//            if(controller.clickButtonState){
//                Log.d(TAG, String.format("Clicked Buttons State : [%s]",
//                        controller.clickButtonState ? "T" : " "));
//            }
        }
    }

    private native long nativeCreateRenderer(
            Image360Activity image360Activity,
            Context applicationContext, long nativeGvrContext, String appDir,
            String Image360AdFileName, String controllerModelFilename,
            String controllerTextureFilename, String laserBeamTextureFilename );

    private native void nativeOnStart(long nativeImage360ActivityHandler);

    private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);

    private native void nativeInitializeGl(long nativeTreasureHuntRenderer);

    private native long nativeDrawFrame(long nativeTreasureHuntRenderer);

//    private native int nativeOnControllerClicked(long nativeTreasureHuntRenderer);

    private native void nativeOnPause(long nativeTreasureHuntRenderer);

    private native void nativeOnResume(long nativeTreasureHuntRenderer);

    private native float[] nativeGetHmdParams(long nativeTreasureHuntRenderer);
}

