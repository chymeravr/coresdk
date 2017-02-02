package com.chymeravr.adclient;


import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.analytics.Event;
import com.chymeravr.common.WebRequestQueue;

import org.json.JSONException;
import org.json.JSONObject;

import java.sql.Timestamp;

/**
 * Created by robin_chimera on 11/28/2016.
 */

/*
    This class manages the Image360 ad display for the SDK
    It is responsible for managing the ad depending on lifecycle
    events. It listens for lifecycle events and renders ads at an
    appropriate time.
    Its core responsibilities are
        - maintaining load states
        - store an ad
        - call display methods for ad
        - request a new ad
 */

public final class Image360Ad extends Ad {
    private static final String TAG = "Image360Ad";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */

    private int showRequestCode = 0;

    private ServerListener<JSONObject> adServerListener;
    private ServerListener<Bitmap> mediaServerListener;
    private ServerListener<byte[]> mediaDownloadServerListener;

    private RequestGenerator requestGenerator;

    public enum IntentActions {CLOSE, OPEN, CLICK, LEFTAPPLICATION};

    private Activity activity;
    private final String placementId;

    private AnalyticsManager analyticsMgr;

    private WebRequestQueue webRequestQueue;

    private void adListenerCallbacks(){
        Log.v(TAG, "Ad Closed");
//        ((Activity)this.getContext()).finishActivity(showRequestCode);
//        this.activity.finishActivity(showRequestCode);
        this.getAdListener().onAdClosed();
    }

    class MessageHandler extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            adListenerCallbacks();
        }
    }

    public Image360Ad(String adUnitID, Activity activity, AdListener adListener) {
        super(adUnitID, activity, adListener);

        this.activity = activity;

        LocalBroadcastManager.getInstance(activity).registerReceiver(new Image360Ad.MessageHandler(),
                new IntentFilter("adClosed"));

        this.analyticsMgr = ChymeraVrSdk.getAnalyticsManager();
        this.webRequestQueue = ChymeraVrSdk.getWebRequestQueue();

        this.placementId = adUnitID;

        this.adServerListener = new AdServerListener(this, this.webRequestQueue.getRequestQueue());
        this.mediaServerListener = new Image360MediaServerListener(this, this.webRequestQueue.getRequestQueue());
        this.mediaDownloadServerListener = new Image360DownloadMediaServerListener(this, this.webRequestQueue.getRequestQueue());
        this.requestGenerator = new RequestGenerator(Type.IMAGE360);
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(AdRequest adRequest) {
        this.setLoading(true);

        analyticsMgr.push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ADREQUEST,
                Event.Priority.MEDIUM));

        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        JsonObjectRequest jsonObjRequest =
                this.requestGenerator.getAdServerJsonRequest(adRequest, this.adServerListener);

        this.adServerListener.getRequestQueue().add(jsonObjRequest);
    }

    @Override
    void onAdServerResponseSuccess(JSONObject response) {
        try {
            this.setMediaUrl(response.getString("mediaUrl"));

            // download media from url and save in internal memory
            InputStreamVolleyRequest mediaDownloadRequest = this.requestGenerator
                    .getMediaDownloadRequest(this.getMediaUrl(), this.mediaDownloadServerListener);
            this.mediaServerListener.getRequestQueue().add(mediaDownloadRequest);

            Log.i(TAG, "Ad Server response successfully processed. Proceeding to query Media Server");
        } catch (JSONException e) {
            this.setLoading(false);
            this.getAdListener().onAdFailedToLoad();
            Log.e(TAG, "Exception", e);
        }
    }

    @Override
    void onMediaServerResponseSuccess(Object media) {
        this.getAdListener().onAdLoaded();
        this.setLoaded(true);
        analyticsMgr.push(new Event((new Timestamp(System.currentTimeMillis())).getTime(),
                Event.EventType.ADLOAD,
                Event.Priority.MEDIUM));
        this.setLoading(false);
        Log.i(TAG, "Media Server Response Successful");
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {
        Intent intent = new Intent(this.activity, Image360Activity.class);
        this.activity.startActivityForResult(intent, showRequestCode);

        this.getAdListener().onAdOpened();
    }

}
