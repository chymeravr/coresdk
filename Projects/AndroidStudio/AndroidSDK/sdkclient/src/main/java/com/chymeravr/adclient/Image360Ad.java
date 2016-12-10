package com.chymeravr.adclient;


import com.android.volley.toolbox.ImageRequest;
import com.android.volley.toolbox.JsonObjectRequest;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

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
    private static final String TAG = "ChymeraVRImage360";
    /* below are some private variables used for internal representation of ad data and
    *  server requests */
    private byte[] byteArray;
    private Bitmap imageBitmap;
    private ServerListener<JSONObject> adServerListener;
    private ServerListener<Bitmap> mediaServerListener;
    private RequestGenerator requestGenerator;

    public Image360Ad(String adUnitID, Context context) {
        super(adUnitID, context);
        this.adServerListener = new Image360AdServerListener(this);
        this.mediaServerListener = new Image360MediaServerListener(this);
        this.requestGenerator = new RequestGenerator(Type.IMAGE360);
    }

    Bitmap getImageBitmap() {
        return this.imageBitmap;
    }

    byte[] getByteArray() {
        return this.byteArray;
    }

    /* Request AdServer for a new image360 ad.
    * It passes targeting parameters from AdRequest for better Ads*/
    public void loadAd(AdRequest adRequest) {
        this.isLoading = true;
        Log.i(TAG, "Requesting Server for a New 360 Image Ad");

        JsonObjectRequest jsonObjRequest =
                requestGenerator.getAdServerJsonRequest(adRequest, this.adServerListener);

        this.adServerListener.getRequestQueue().add(jsonObjRequest);
    }

    @Override
    void onAdServerResponseSuccess(JSONObject response) {
        try {
            Image360Ad.this.setMediaUrl(response.getString("url"));
            ImageRequest imageRequest =
                    requestGenerator.getMediaServerRequest(this.getMediaUrl(), this.mediaServerListener);
            this.mediaServerListener.getRequestQueue().add(imageRequest);

            Log.i(TAG, "Ad Server response succesfully processed. Proceeding to query Media Server");
        } catch (JSONException e) {
            this.isLoading = false;
            this.adListener.onAdFailedToLoad();
            e.printStackTrace();
            Log.e(TAG, "An exception occurred while processing the JSON Response from the ad server");
        }
    }

    @Override
    void onMediaServerResponseSuccess(Object media) {
        this.imageBitmap = (Bitmap) media;
        this.byteArray = Util.convertToByteArray(this.imageBitmap);
        this.adListener.onAdLoaded();
        this.isLoaded = true;
        this.isLoading = false;
        Log.i(TAG, "Media Server Response Successful");
    }

    /* Display ad by calling the native graphics library (or 3rd party API if that is the case)*/
    public void show() {

    }

}