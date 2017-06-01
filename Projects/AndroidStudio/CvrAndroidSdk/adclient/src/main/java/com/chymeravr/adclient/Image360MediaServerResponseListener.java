package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.Response;

import lombok.NonNull;
import lombok.RequiredArgsConstructor;


/**
 * Created by robin_chimera on 1/23/2017.
 * Defines callbacks for downloading media from server
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
final class Image360MediaServerResponseListener implements Response.Listener<byte[]> {
    private final String TAG = "MediaServerResponse";

    @NonNull
    private Image360AdServices adServices;

    @Override
    public void onResponse(byte[] response) {
        Log.d(TAG, "Image 360 Media Server Response Success");
        this.adServices.onMediaServerResponseSuccess(response);
    }
}
