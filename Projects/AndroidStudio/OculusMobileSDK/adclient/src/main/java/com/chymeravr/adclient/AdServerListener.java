package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;

import org.json.JSONObject;

import lombok.NonNull;

/**
 * Created by robin_chimera on 12/6/2016.
 */

class AdServerListener extends ServerListener<JSONObject> {
    private final String TAG = "AdServListener";

    public AdServerListener(@NonNull Ad ad, RequestQueue requestQueue) {
        super(ad);
        this.setRequestQueue(requestQueue);
    }

    @Override
    public void onErrorResponse(VolleyError error) {
        this.getAd().setLoading(false);
        this.getAd().getAdListener().onAdFailedToLoad();
        Log.e(TAG, "Error", error);
    }

    @Override
    public void onResponse(JSONObject response) {
        this.getAd().onAdServerResponseSuccess(response);
        Log.i(TAG, "Ad Server Response Success!");
    }
}
