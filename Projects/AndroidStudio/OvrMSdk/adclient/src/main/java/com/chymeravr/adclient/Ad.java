package com.chymeravr.adclient;

import android.content.Context;

import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.analytics.Event;
import com.chymeravr.common.WebRequestQueue;

import org.json.JSONObject;

import java.sql.Timestamp;
import java.util.HashMap;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

/**
 * Created by robin_chimera on 12/5/2016.
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
abstract class Ad {

//    public enum Type {
//        IMG_360,
//        VID_360,
//        VR_EXPERIENCE,
//        TEXTURE_AD
//    };

    @Getter
    @NonNull
    private final String placementId;

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
    private final AdListener adListener;

    @Getter(AccessLevel.PACKAGE)
    @Setter(AccessLevel.PACKAGE)
    private String mediaUrl;

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


    protected void emitEvent(Event.EventType eventType, Event.Priority priority, HashMap<String, Object> map){
        long currTime = new Timestamp(System.currentTimeMillis()).getTime();
        Event event = new Event(currTime, eventType, priority, map, this.getServingId(), this.getInstanceId());

        this.getAnalyticsManager().push(event);
    }

    public abstract void loadAd(AdRequest adRequest);

    public abstract void show();

    abstract void onAdServerResponseSuccess(JSONObject response);

    abstract void onMediaServerResponseSuccess(Object media);
}
