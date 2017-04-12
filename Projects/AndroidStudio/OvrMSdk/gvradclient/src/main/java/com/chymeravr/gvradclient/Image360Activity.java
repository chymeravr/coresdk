package com.chymeravr.gvradclient;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Vibrator;
import android.support.v4.app.NotificationCompat;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.GvrLayout;
import com.google.vr.sdk.controller.Controller;
import com.google.vr.sdk.controller.ControllerManager;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import lombok.Getter;

/**
 * Created by robin_chimera on 3/22/2017.
 */

public class Image360Activity extends Activity {
    private GvrLayout gvrLayout;
    private long nativeImage360ActivityHandler;
    private GLSurfaceView surfaceView;

    private ControllerManager ddControllerManager;
    private Controller ddController;

    private String clickUrl;


    @Getter
    private String servingId;

    @Getter
    private int instanceId;

    private static final String TAG = "Image360AdGvrActivity";
    // This is done on the GL thread because refreshViewerProfile isn't thread-safe.
    private final Runnable refreshViewerProfileRunnable =
            new Runnable() {
                @Override
                public void run() {
                    gvrLayout.getGvrApi().refreshViewerProfile();
                }
            };

    private enum KeyEventResponse{
        NO_EVENT,
        NOTIFY_ME,
        CLOSE_AD
    }

    static {
        System.loadLibrary("gvr");
        //System.loadLibrary("gvr_audio");
        System.loadLibrary("image360ad");
    }

    private void finishAdActivity() {
        // TODO: 2/2/2017 the surface is closed in correctly here - fix it
        // signal parent activity (from the client) to end the ad
        Intent intent = new Intent("adClosed");
        LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
        //finish();
    }

    class MessageHandler extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG, "signalling parent to close activity for image360 ad");
            finishAdActivity();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");
        // Register an kill activity intent to destroy the activity when user is done and return to parent
        LocalBroadcastManager.getInstance(this).registerReceiver(new MessageHandler(),
                new IntentFilter("finishAd"));

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

        String basePath = this.getFilesDir().getAbsolutePath();

        // Fetch url to show when user clicks
        Intent intent = getIntent();
        this.clickUrl = intent.getStringExtra("clickUrl");

        String imageAdFilePath = intent.getStringExtra("imageAdFilePath");

        this.instanceId = intent.getIntExtra("instanceId", -1);
        this.servingId = intent.getStringExtra("servingId");

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
                        nativeInitializeGl(nativeImage360ActivityHandler);
                    }

                    @Override
                    public void onSurfaceChanged(GL10 gl, int width, int height) {}

                    @Override
                    public void onDrawFrame(GL10 gl) {

                        ddController.update();
                        if(ddController.clickButtonState) {
                            Log.d(TAG, "Click Status : " + ddController.clickButtonState);


                            KeyEventResponse keyEventResponse = KeyEventResponse.values()[nativeOnControllerClicked(nativeImage360ActivityHandler)];

                            Log.d(TAG, "Response : " + keyEventResponse);
                            switch (keyEventResponse){
                                case NO_EVENT:
                                    Log.d(TAG, "No Event");
                                    break;
                                case NOTIFY_ME:
                                    notifyUser();
                                    Log.d(TAG, "Notify Me");
                                    break;
                                case CLOSE_AD:
                                    Log.d(TAG, "Closing Image 360 Activity");
                                    Image360Activity.this.finish();

//                                    Intent intent = new Intent("finishAd");
//                                    LocalBroadcastManager.getInstance(Image360Activity.this).sendBroadcast(intent);
                                    break;
                            }
                        }

                        nativeDrawFrame(nativeImage360ActivityHandler);
                    }
                });
        surfaceView.setOnTouchListener(
                new View.OnTouchListener() {
                    @Override
                    public boolean onTouch(View v, MotionEvent event) {
                        if (event.getAction() == MotionEvent.ACTION_DOWN) {
                            // Give user feedback and signal a trigger event.
                            ((Vibrator) getSystemService(Context.VIBRATOR_SERVICE)).vibrate(50);
//                            int keyEventResult = nativeOnTriggerEvent(nativeImage360ActivityHandler);
//                            Log.d(TAG, "Key event : " + keyEventResult);
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
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
        // Destruction order is important; shutting down the GvrLayout will detach
        // the GLSurfaceView and stop the GL thread, allowing safe shutdown of
        // native resources from the UI thread.
        gvrLayout.shutdown();
        nativeDestroyRenderer(nativeImage360ActivityHandler);
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

    private void notifyUser() {
        Intent notificationIntent = new Intent(Intent.ACTION_VIEW);
        notificationIntent.setData(Uri.parse(this.clickUrl));
        PendingIntent pi = PendingIntent.getActivity(this, 0, notificationIntent, 0);

        // TODO: 2/22/2017 Get these from the server as well. Issue raised in bitbucket serving repo
        Notification notification = new NotificationCompat.Builder(this)
                .setSmallIcon(R.drawable.download_icon)
                .setContentTitle("<YourContentTitle>")
                .setContentText("<YourContentText>")
                .setContentIntent(pi)
                .setAutoCancel(true)
                .build();

        NotificationManager notificationManager2 = (NotificationManager) this.getSystemService(Service.NOTIFICATION_SERVICE);
        notificationManager2.notify(0, notification);
    }

    private native long nativeCreateRenderer(
            ClassLoader appClassLoader, Context context, long nativeGvrContext, String appDir, String Image360AdFileName);

    private native void nativeOnStart(long nativeImage360ActivityHandler);

    private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);

    private native void nativeInitializeGl(long nativeTreasureHuntRenderer);

    private native long nativeDrawFrame(long nativeTreasureHuntRenderer);

    private native int nativeOnTriggerEvent(long nativeTreasureHuntRenderer);

    private native int nativeOnControllerClicked(long nativeTreasureHuntRenderer);

    private native void nativeOnPause(long nativeTreasureHuntRenderer);

    private native void nativeOnResume(long nativeTreasureHuntRenderer);
}

