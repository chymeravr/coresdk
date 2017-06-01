package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 12/6/2016.
 * Handles call backs from the Ad Server
 */

import android.util.Log;

import com.android.volley.Response;

import org.json.JSONObject;

import lombok.NonNull;
import lombok.RequiredArgsConstructor;


@RequiredArgsConstructor(suppressConstructorProperties = true)
final class AdServerResponseListener implements Response.Listener<JSONObject> {
    private final String TAG = "AdServerResponsListener";

    @NonNull
    private Image360AdServices adServices;

    @Override
    public void onResponse(JSONObject response) {
        Log.i(TAG, "Ad Server Response Success!");
        this.adServices.onAdServerResponseSuccess(response);
    }
}
