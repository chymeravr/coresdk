package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.VolleyError;

import org.json.JSONObject;

/**
 * Created by robin_chimera on 12/6/2016.
 */

class Image360AdServerListener extends ServerListener<JSONObject> {
    private final String TAG = "Image360AdServListener";

    public Image360AdServerListener(Ad ad) {
        super(ad);
        this.setRequestQueue(WebRequestQueue.getInstance(ad.getContext()).getRequestQueue());
    }

    @Override
    public void onErrorResponse(VolleyError error) {
        Log.e(TAG, "Ad Server Response Failure!");
        this.getAd().isLoading = false;
        this.getAd().adListener.onAdFailedToLoad();
        error.printStackTrace();
    }

    @Override
    public void onResponse(JSONObject response) {
        this.getAd().onAdServerResponseSuccess(response);
        Log.i(TAG, "Ad Server Response Success!");
    }
}
