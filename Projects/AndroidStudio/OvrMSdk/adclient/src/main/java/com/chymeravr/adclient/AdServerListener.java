package com.chymeravr.adclient;

import android.util.Log;

import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.Util;
import com.chymeravr.schemas.eventreceiver.EventType;

import org.json.JSONObject;

import lombok.NonNull;
import lombok.RequiredArgsConstructor;

/**
 * Created by robin_chimera on 12/6/2016.
 * Handles call backs from the Ad Server
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
class AdServerListener implements Response.ErrorListener, Response.Listener<JSONObject> {//extends ServerListener<JSONObject> {
    private final String TAG = "AdServListener";

    @NonNull
    private Ad ad;

    @Override
    public void onErrorResponse(VolleyError error) {
        Log.e(TAG, "Error", error);

        this.ad.setLoading(false);
        this.ad.getAdListener().onAdFailedToLoad();
        // send error logs to server
        this.ad.emitEvent(EventType.ERROR, AnalyticsManager.Priority.LOW, Util.getErrorMap(error));
    }

    @Override
    public void onResponse(JSONObject response) {
        Log.i(TAG, "Ad Server Response Success!");
        this.ad.onAdServerResponseSuccess(response);
    }
}
