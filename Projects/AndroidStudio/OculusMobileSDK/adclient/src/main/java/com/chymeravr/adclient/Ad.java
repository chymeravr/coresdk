package com.chymeravr.adclient;

import android.content.Context;

import com.chymeravr.analytics.AnalyticsManager;
import com.chymeravr.common.WebRequestQueue;

import org.json.JSONObject;

import lombok.AccessLevel;
import lombok.Getter;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

/**
 * Created by robin_chimera on 12/5/2016.
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
abstract class Ad {

    public enum Type {
        IMAGE360,
        VIDEO360,
        VREXPERIENCE,
        TEXTUREAD
    };

    @Getter
    private final String placementId;

    @Getter
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

    @Getter
    private final AnalyticsManager analyticsManager;

    @Getter
    private final WebRequestQueue webRequestQueue;

    public abstract void loadAd(AdRequest adRequest);

    public abstract void show();

    abstract void onAdServerResponseSuccess(JSONObject response);

    abstract void onMediaServerResponseSuccess(Object media);
}
