package com.chymeravr.adclient;


import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.util.Log;

import com.android.volley.toolbox.JsonObjectRequest;

import org.json.JSONException;
import org.json.JSONObject;

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

    private ServerListener<JSONObject> adServerListener;
    private ServerListener<Bitmap> mediaServerListener;
    private ServerListener<byte[]> mediaDownloadServerListener;

    private RequestGenerator requestGenerator;

    private final Activity activity;
    private final String placementId;



    public Image360Ad(String adUnitID, Context context, AdListener adListener) {
        super(adUnitID, context, adListener);

        this.activity = (Activity) context;
        this.placementId = adUnitID;
        this.adServerListener = new AdServerListener(this);
        this.mediaServerListener = new Image360MediaServerListener(this);
        this.mediaDownloadServerListener = new Image360DownloadMediaServerListener(this);
        this.requestGenerator = new RequestGenerator(Type.IMAGE360);
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(AdRequest adRequest) {
        this.setLoading(true);
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
        this.setLoading(false);
        Log.i(TAG, "Media Server Response Successful");
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {
        Intent intent = new Intent(this.getContext(), Image360Activity.class);

        this.getContext().startActivity(intent);

    }

}