package com.chymeravr.adclient;

import android.content.Context;

import com.chymeravr.analytics.EventPriority;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.serving.AdFormat;

import org.json.JSONObject;

import java.util.Map;

/**
 * Created by robin_chimera on 12/6/2016.
 */

/* A fake ad class implementation for testing. this doesnt do anything*/
public class FakeAd extends Ad {
    public FakeAd(Context context, VrAdListener vrAdListener) {
        super(AdFormat.IMG_360, context, vrAdListener);
    }

    @Override
    protected void emitEvent(EventType eventType, EventPriority priority, Map<String, String> map) {
        return;
    }

    @Override
    public void loadAd(VrAdRequest vrAdRequest) {
        this.setLoading(true);
    }

    @Override
    public void show() {
        return;
    }

    @Override
    public void onAdServerResponseSuccess(JSONObject response) {
        return;
    }

    @Override
    public void onMediaServerResponseSuccess() {
        return;
    }


}
