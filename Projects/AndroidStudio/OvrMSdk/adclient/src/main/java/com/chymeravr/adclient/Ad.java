package com.chymeravr.adclient;

import android.content.Context;

import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.WebRequestQueue;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.eventreceiver.RuntimeAdMeta;
import com.chymeravr.schemas.eventreceiver.SDKEvent;
import com.chymeravr.schemas.serving.AdFormat;

import org.json.JSONObject;

import java.sql.Timestamp;
import java.util.Map;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

/**
 * Created by robin_chimera on 12/5/2016.
 * an abstract ad class to hold common ad attributes and methods
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
abstract class Ad {

    @Getter
    @NonNull
    private final AdFormat adFormat;

    @Getter
    @Setter
    private String placementId;

    @Getter(AccessLevel.PACKAGE)
    @Setter(AccessLevel.PACKAGE)
    private String servingId;

    @Getter(AccessLevel.PACKAGE)
    @Setter(AccessLevel.PACKAGE)
    private int instanceId;

    @Getter
    @NonNull
    private final Context context;

    @Getter
    @NonNull
    private AdListener adListener;

    @Getter(AccessLevel.PACKAGE)
    @Setter(AccessLevel.PACKAGE)
    private String mediaUrl;

    @Getter(AccessLevel.PACKAGE)
    @Setter(AccessLevel.PACKAGE)
    private String clickUrl;

    @Getter
    @Setter(AccessLevel.PACKAGE)
    private volatile boolean isLoading = false;

    @Getter
    @Setter(AccessLevel.PACKAGE)
    private volatile boolean isLoaded = false;

    @Getter(AccessLevel.PACKAGE)
    private final AnalyticsManager analyticsManager;

    @Getter(AccessLevel.PACKAGE)
    private final WebRequestQueue webRequestQueue;


    protected void emitEvent(EventType eventType, AnalyticsManager.Priority priority, Map<String, String> map){
        long currTime = new Timestamp(System.currentTimeMillis()).getTime();
        RuntimeAdMeta adMeta = new RuntimeAdMeta(this.getServingId(), this.getInstanceId());
        SDKEvent event = new SDKEvent(currTime, eventType, adMeta);
        event.setParamsMap(map);
        this.getAnalyticsManager().push(event, priority);
    }

    public abstract void loadAd(AdRequest adRequest);

    public abstract void show();

    abstract void onAdServerResponseSuccess(JSONObject response);

    abstract void onMediaServerResponseSuccess();
}
