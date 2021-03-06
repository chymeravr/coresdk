package com.chymeravr.gearvradclient;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.widget.ImageView;

import com.chymeravr.analytics.EventPriority;
import com.chymeravr.common.Config;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.eventreceiver.RuntimeAdMeta;
import com.chymeravr.schemas.eventreceiver.SDKEvent;

import java.sql.Timestamp;
import java.util.HashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import lombok.Getter;

/**
 * Created by robin_chimera on 1/24/2017.
 * Custom Activity to show ads in
 */

public final class Image360Activity extends Activity implements SurfaceHolder.Callback {

    public static final String TAG = "Image360AdOvrMActivity";

    private SurfaceView mView;
    private SurfaceHolder mSurfaceHolder;
    private long mNativeHandle;

    private Animation anim;
    private ImageView v;

    private String clickUrl;

    @Getter
    private String servingId;

    @Getter
    private int instanceId;

    private enum KeyEventResponse{
        NO_EVENT,
        NOTIFY_ME,
        CLOSE_AD
    }

    // load the native library for image 360 ads
    static {
        System.loadLibrary("GearVrAdClient");
    }

    private ScheduledExecutorService scheduler;

    private final Runnable hmdPollRunner = new Runnable() {
        public void run() {
            Image360Activity.this.getOvrHmdParams();
        }
    };

    // Native methods for Activity lifecyle
    private native long onCreateNative(Activity activity, String appDir, String appFileName, AssetManager mgr);

    private native void onStartNative(long handle);

    private native void onPauseNative(long handle);

    private native void onResumeNative(long handle);

    private native void onStopNative(long handle);

    private native void onDestroyNative(long handle);

    // Native methods for Surface Lifecycle
    public native void onSurfaceCreatedNative(long handle, Surface s);

    public native void onSurfaceChangedNative(long handle, Surface s);

    public native void onSurfaceDestroyedNative(long handle);

    // Native methods for handling touch and key events
    public native int onKeyEventNative(long handle, int keyCode, int action);

    public native int onTouchEventNative(long handle, int action, float x, float y);

    public native float[] getHMDParamsNative(long handle);

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
            finishAdActivity();
        }
    }

    public void emitEvent(EventType eventType, EventPriority priority, HashMap<String, String> map){
        long currTime = new Timestamp(System.currentTimeMillis()).getTime();
        RuntimeAdMeta adMeta = new RuntimeAdMeta(this.getServingId(), this.getInstanceId());
        SDKEvent event = new SDKEvent(currTime, eventType, adMeta);
        event.setParamsMap(map);
        ChymeraVrSdk.getAnalyticsManager().push(event, priority);
    }

    @Override
    protected void onCreate(Bundle icicle) {
        Log.d(TAG, "onCreate()");
        super.onCreate(icicle);
        setContentView(R.layout.gl_context);
        //overridePendingTransition(R.anim.image360fadein, R.anim.image360fadeout);

        // Fetch url to show when user clicks
        Intent intent = getIntent();
        this.clickUrl = intent.getStringExtra("clickUrl");

        String imageAdFilePath = intent.getStringExtra("imageAdFilePath");

        this.instanceId = intent.getIntExtra("instanceId", -1);
        this.servingId = intent.getStringExtra("servingId");

        // Register an kill activity intent to destroy the activity when user is done and return to parent
        LocalBroadcastManager.getInstance(this).registerReceiver(new MessageHandler(),
                new IntentFilter("finishAd"));

        // call native creation method for all the HMD magic with OVR
        String basePath = this.getFilesDir().getAbsolutePath();

        AssetManager mgr = getResources().getAssets();

        this.mNativeHandle = this.onCreateNative(this, basePath, imageAdFilePath, mgr);

        // We draw everything in a surface embedded within the activity layout
        //this.mView = new SurfaceView(this);
        this.mView = (SurfaceView) findViewById(R.id.mView);
        //this.setContentView(mView);
        this.mView.getHolder().addCallback(this);

        // Force the screen to stay on, rather than letting it dim and shut off
        // while the user is watching amovie.
        this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // Force screen brightness to stay at maximum
        WindowManager.LayoutParams params = this.getWindow().getAttributes();
        params.screenBrightness = 1.0f;
        this.getWindow().setAttributes(params);

    }

    @Override
    protected void onStart() {
        Log.d(TAG, "onStart()");
        super.onStart();
        this.onStartNative(this.mNativeHandle);
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume()");
        super.onResume();
        this.onResumeNative(this.mNativeHandle);

    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause()");
        super.onPause();
        this.onPauseNative(this.mNativeHandle);
    }

    @Override
    protected void onStop() {
        Log.d(TAG, "onStop()");
        this.emitEvent(EventType.AD_CLOSE, EventPriority.HIGH, null);
        super.onStop();
        this.onStopNative(this.mNativeHandle);
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy()");
        super.onDestroy();
        this.onDestroyNative(this.mNativeHandle);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.d(TAG, "ActivityGearVR::onSurfaceCreated()");

        this.emitEvent(EventType.AD_SHOW, EventPriority.HIGH, null);
        if (this.mNativeHandle != 0) {
            onSurfaceCreatedNative(this.mNativeHandle, holder.getSurface());
            this.mSurfaceHolder = holder;

            // schedule polling for hmd parameters
            // TODO: 4/27/2017 remove hardcoded pool size 
            this.scheduler = Executors.newScheduledThreadPool(1);
            scheduler.scheduleAtFixedRate(hmdPollRunner, Config.getHmdSamplingDelay(),
                    Config.getHmdSamplingDelay(), TimeUnit.SECONDS);
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.d(TAG, "ActivityGearVR::onSurfaceChanged()");
        if (this.mNativeHandle != 0) {
            onSurfaceChangedNative(this.mNativeHandle, holder.getSurface());
            this.mSurfaceHolder = holder;
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.d(TAG, "ActivityGearVR::onSurfaceDestroyed()");
        if (this.mNativeHandle != 0) {
            onSurfaceDestroyedNative(this.mNativeHandle);
            this.mSurfaceHolder = null;

            this.scheduler.shutdown();
        }
    }


    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        // TODO: 2/27/2017 implement UI click events for notification and ad close

        if(event.getAction() == KeyEvent.ACTION_UP){
            return false;
        }

        KeyEventResponse keyEventResponse = KeyEventResponse.NO_EVENT;

        Log.d(TAG, "dispatchKeyEvent");
        if (mNativeHandle != 0) {
            int keyCode = event.getKeyCode();
            int action = event.getAction();
            if ( action != KeyEvent.ACTION_DOWN && action != KeyEvent.ACTION_UP )
            {
                return super.dispatchKeyEvent( event );
            }

            if(event.getAction()==KeyEvent.ACTION_UP){
                return false;
            }

            keyEventResponse = KeyEventResponse.values()[onKeyEventNative(mNativeHandle, keyCode, action)];

            switch (keyEventResponse){
                case NO_EVENT:
                    break;
                case NOTIFY_ME:
                    notifyUser();
                    break;
                case CLOSE_AD:
                    Intent intent = new Intent("finishAd");
                    LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
                    break;
            }
            return true;
        }

        return false;
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        // will probably not need this - remove in future
        super.dispatchTouchEvent(event);
        if (this.mNativeHandle != 0) {
            int action = event.getAction();
            float x = event.getRawX();
            float y = event.getRawY();
            KeyEventResponse keyEventResponse = KeyEventResponse.NO_EVENT;
            if (action == MotionEvent.ACTION_UP) {
                Log.d(TAG, "GLES3JNIActivity::dispatchTouchEvent( " + action + ", " + x + ", " + y + " )");
            }

            keyEventResponse = KeyEventResponse.values()[onTouchEventNative(mNativeHandle, action, x, y)];

            switch (keyEventResponse){
                case NO_EVENT:
                    break;
                case NOTIFY_ME:
                    Log.d(TAG, "Notifying Me");
                    notifyUser();
                    exit();
                    break;
                case CLOSE_AD:
                    Log.d(TAG, "Closing This Ad ");
                    exit();
                    break;
            }
            return true;
        }
        return true;
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

    // TODO: 4/27/2017 switch to quaternion representation
    private void getOvrHmdParams() {
        if(this.mNativeHandle != 0) {
            float[] hmdParams = this.getHMDParamsNative(this.mNativeHandle);
            HashMap<String, String> hmdEyeMap = new HashMap<>();

            String[] parameterKeys =
                            {"L00", "L01", "L02", "L03",
                            "L10", "L11", "L12", "L13",
                            "L20", "L21", "L22", "L23",
                            "L30", "L31", "L32", "L33",

                            "R00", "R01", "R02", "R03",
                            "R10", "R11", "R12", "R13",
                            "R20", "R21", "R22", "R23",
                            "R30", "R31", "R32", "R33"};

            int i = 0;
            for (String key : parameterKeys) {
                hmdEyeMap.put(key, String.valueOf(hmdParams[i++]));
            }

            this.emitEvent(EventType.AD_VIEW_METRICS, EventPriority.LOW, hmdEyeMap);
        }
    }

    public void exit(){
        v = (ImageView)findViewById(R.id.fadeview);

        Bitmap image = Bitmap.createBitmap(v.getWidth(), v.getHeight(), Bitmap.Config.ARGB_8888);
        image.eraseColor(android.graphics.Color.BLACK);

        v.setImageBitmap(image);

        anim = new AlphaAnimation(0.0f, 1.0f);
        anim.setDuration(1500);
        anim.setRepeatCount(0);

        anim.setAnimationListener(new Animation.AnimationListener() {

            @Override
            public void onAnimationStart(Animation animation) {
                // TODO Auto-generated method stub

            }

            @Override
            public void onAnimationRepeat(Animation animation) {
                // TODO Auto-generated method stub

            }

            @Override
            public void onAnimationEnd(Animation animation) {
                Log.d(TAG, "VISIBILITY IS GONE");
                mView.setVisibility(View.GONE);
                Intent intent = new Intent("finishAd");
                LocalBroadcastManager.getInstance(Image360Activity.this).sendBroadcast(intent);
            }
        });

        v.startAnimation(anim);
    }
}
