package com.chymeravr.adclient;


import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import com.android.volley.toolbox.JsonObjectRequest;
import com.chymeravr.analytics.Event;
import com.chymeravr.common.Config;
import com.chymeravr.schemas.serving.AdFormat;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;

import lombok.Getter;
import lombok.Setter;

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

    @Getter
    @Setter
    private String clickUrl;

    public enum IntentActions {CLOSE, OPEN, CLICK, LEFTAPPLICATION};

    private Activity activity;

    private void adListenerCallbacks(){
        Log.v(TAG, "Ad Closed");
        this.getAdListener().onAdClosed();
    }

    class MessageHandler extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            adListenerCallbacks();
        }
    }

    public Image360Ad(String placementId, Activity activity, AdListener adListener) {
        super(placementId, activity, adListener, ChymeraVrSdk.getAnalyticsManager(), ChymeraVrSdk.getWebRequestQueue());

        // only one instance of image360 ad will be present always
        this.setInstanceId(-1);

        this.activity = activity;

        LocalBroadcastManager.getInstance(activity).registerReceiver(new Image360Ad.MessageHandler(),
                new IntentFilter("adClosed"));

        this.adServerListener = new AdServerListener(this, this.getWebRequestQueue().getRequestQueue());
        this.mediaServerListener = new Image360MediaServerListener(this, this.getWebRequestQueue().getRequestQueue());
        this.mediaDownloadServerListener = new Image360DownloadMediaServerListener(this, this.getWebRequestQueue().getRequestQueue());
        this.requestGenerator = new RequestGenerator(this.getPlacementId(), AdFormat.IMG_360);
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(final AdRequest adRequest) {
        this.setLoading(true);
        this.emitEvent(Event.EventType.AD_REQUEST, Event.Priority.MEDIUM, null);

        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        // Fetching advertisingId completely asynchronously will lead to race condition.
        // better to wrap them all together. It is important we get the advertId because
        // it helps in tracking app downloads - usage etc.
        AsyncTask<Void, Void, String> task = new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                if(ChymeraVrSdk.getAdvertisingId() != null){
                    return null;
                }

                AdvertisingIdClient.Info idInfo = null;
                try {
                    idInfo = AdvertisingIdClient.getAdvertisingIdInfo(ChymeraVrSdk.getContext());
                } catch (GooglePlayServicesNotAvailableException e) {
                    e.printStackTrace();
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    HashMap<String, Object> errorMap = new HashMap<String, Object>();
                    errorMap.put("Error", e.toString());
                    Image360Ad.this.emitEvent(Event.EventType.ERROR, Event.Priority.LOW, errorMap);

                } catch (GooglePlayServicesRepairableException e) {
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    HashMap<String, Object> errorMap = new HashMap<String, Object>();
                    errorMap.put("Error", e.toString());
                    Image360Ad.this.emitEvent(Event.EventType.ERROR, Event.Priority.LOW, errorMap);
                } catch (Exception e) {
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    HashMap<String, Object> errorMap = new HashMap<String, Object>();
                    errorMap.put("Error", e.toString());
                    Image360Ad.this.emitEvent(Event.EventType.ERROR, Event.Priority.LOW, errorMap);
                }
                String advertId = null;
                try {
                    advertId = idInfo.getId();
                } catch (Exception e) {
                    Log.e(TAG, "Load Ad Failed due to an Exception : ", e);
                    HashMap<String, Object> errorMap = new HashMap<String, Object>();
                    errorMap.put("Error", e.toString());
                    Image360Ad.this.emitEvent(Event.EventType.ERROR, Event.Priority.LOW, errorMap);
                }
                ChymeraVrSdk.setAdvertisingId(advertId);
                Log.v(TAG, "AdvertisingId : " + advertId);
                return null;
            }

            @Override
            protected void onPostExecute(String advertId) {
                JsonObjectRequest jsonObjRequest =
                        Image360Ad.this.requestGenerator.getAdServerJsonRequest(adRequest, Image360Ad.this.adServerListener);

                Image360Ad.this.adServerListener.getRequestQueue().add(jsonObjRequest);
            }
        };
        task.execute();
    }

    @Override
    void onAdServerResponseSuccess(JSONObject response) {
        try {
            this.setServingId(response.getJSONObject("ads").getJSONObject(this.getPlacementId()).getString("servingId"));
            this.setMediaUrl(response.getJSONObject("ads").getJSONObject(this.getPlacementId()).getString("mediaUrl"));
//            this.setClickUrl(response.getJSONObject("ads").getJSONObject(this.getPlacementId()).getString("clickUrl"));
            // download media from url and save in internal memory
            InputStreamVolleyRequest mediaDownloadRequest = this.requestGenerator
                    .getMediaDownloadRequest(this.getMediaUrl(), this.mediaDownloadServerListener);
            this.mediaServerListener.getRequestQueue().add(mediaDownloadRequest);

            Log.i(TAG, "Ad Server response successfully processed. Proceeding to query Media Server");
        } catch (JSONException e) {
            this.setLoading(false);
            this.getAdListener().onAdFailedToLoad();

            HashMap<String, Object> errorMap = new HashMap<String, Object>();
            errorMap.put("Error", e.toString());
            this.emitEvent(Event.EventType.ERROR, Event.Priority.LOW, errorMap);

            Log.e(TAG, "Adserver Success Response Handler encountered Exception : ", e);
        }
    }

    @Override
    void onMediaServerResponseSuccess(Object media) {
        this.getAdListener().onAdLoaded();
        this.setLoaded(true);
        this.setLoading(false);
        this.emitEvent(Event.EventType.AD_LOAD, Event.Priority.MEDIUM, null);
        Log.i(TAG, "Media Server Response Successful");
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {
        Intent intent = new Intent(this.activity, Image360Activity.class);
        intent.putExtra("clickUrl", this.getClickUrl());
        intent.putExtra("imageAdFilePath", Config.Image360AdAssetDirectory + this.getPlacementId() + ".jpg");
        intent.putExtra("servingId", this.getServingId());
        intent.putExtra("instanceId", this.getInstanceId());
        this.activity.startActivityForResult(intent, showRequestCode);

        this.getAdListener().onAdOpened();
    }

}
