package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.Response;
import com.android.volley.VolleyError;

import lombok.NonNull;
import lombok.RequiredArgsConstructor;

/**
 * Created by robin_chimera on 5/29/2017.
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
final class AdServerErrorListener implements Response.ErrorListener{
    private final String TAG = "AdServerErrorListener";

    @NonNull
    private Image360AdServices adServices;

    @Override
    public void onErrorResponse(VolleyError error) {
        Log.e(TAG, "Error", error);
        this.adServices.onAdServerResponseError(error);
    }
}
