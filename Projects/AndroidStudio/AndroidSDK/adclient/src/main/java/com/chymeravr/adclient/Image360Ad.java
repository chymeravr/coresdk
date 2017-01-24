package com.chymeravr.adclient;


import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

import com.android.volley.toolbox.JsonObjectRequest;

import org.json.JSONException;
import org.json.JSONObject;

import lombok.Getter;

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
    private static final String TAG = "ChymeraVRImage360";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */
    @Getter
    private byte[] byteArray;

    @Getter
    private Bitmap imageBitmap;
    private ServerListener<JSONObject> adServerListener;
    private ServerListener<Bitmap> mediaServerListener;
    private ServerListener<byte[]> mediaDownloadServerListener;

    private RequestGenerator requestGenerator;

    public Image360Ad(String adUnitID, Context context, AdListener adListener) {
        super(adUnitID, context, adListener);
        this.adServerListener = new AdServerListener(this);
        this.mediaServerListener = new Image360MediaServerListener(this);
        this.mediaDownloadServerListener = new Image360DownloadMediaServerListener(this);
        this.requestGenerator = new RequestGenerator(Type.IMAGE360);
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(AdRequest adRequest) {
        this.setLoaded(true);
        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        JsonObjectRequest jsonObjRequest =
                this.requestGenerator.getAdServerJsonRequest(adRequest, this.adServerListener);

        this.adServerListener.getRequestQueue().add(jsonObjRequest);
    }

    @Override
    void onAdServerResponseSuccess(JSONObject response) {
        try {
            this.setMediaUrl(response.getString("mediaUrl"));
            Log.v(TAG, "Returned Media Url : " + this.getMediaUrl());
//            ImageRequest imageRequest =
//                    this.requestGenerator.getMediaServerRequest(this.getMediaUrl(),
//                            this.mediaServerListener);
//            this.mediaServerListener.getRequestQueue().add(imageRequest);
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
        this.imageBitmap = (Bitmap) media;
        this.byteArray = Util.convertToByteArray(this.imageBitmap);
        this.getAdListener().onAdLoaded();
        this.setLoaded(true);
        this.setLoading(false);
        Log.i(TAG, "Media Server Response Successful");
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {

    }

}