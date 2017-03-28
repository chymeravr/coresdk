package com.chymeravr.ovrmadclient;

import android.content.Context;

import com.chymeravr.adclient.Ad;
import com.chymeravr.adclient.VrAdListener;
import com.chymeravr.adclient.VrAdRequest;
import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.WebRequestQueue;
import com.chymeravr.schemas.serving.AdFormat;

import org.json.JSONObject;

/**
 * Created by robin_chimera on 12/6/2016.
 */

/* A fake ad class implementation for testing. this doesnt do anything*/
public class FakeAd extends Ad {
    public FakeAd(Context context, VrAdListener vrAdListener, AnalyticsManager analyticsManager,
                  WebRequestQueue webRequestQueue) {
        super(AdFormat.IMG_360, context, vrAdListener, analyticsManager, webRequestQueue);
    }

    @Override
    public void loadAd(VrAdRequest vrAdRequest) {
        this.setLoading(true);
    }

    @Override
    public void show() {

    }

    @Override
    public void onAdServerResponseSuccess(JSONObject response) {

    }

    @Override
    public void onMediaServerResponseSuccess() {

    }


}
