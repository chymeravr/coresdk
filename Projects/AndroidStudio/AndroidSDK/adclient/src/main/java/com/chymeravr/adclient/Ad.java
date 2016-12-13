package com.chymeravr.adclient;

import android.content.Context;

import org.json.JSONObject;

import lombok.Getter;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

/**
 * Created by robin_chimera on 12/5/2016.
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
abstract class Ad {

    public enum Type{
        IMAGE360,
        VIDEO360,
        VREPERIENCE,
        TEXTUREAD
    };

    private final String adUnitID;

    @Getter
    protected final Context context;

    @Getter
    @Setter
    protected AdListener adListener;

    @Getter
    @Setter
    String mediaUrl;

    @Getter
    public volatile boolean isLoading = false;
    @Getter
    public volatile boolean isLoaded = false;

    public abstract void loadAd(AdRequest adRequest);

    public abstract void show();

    abstract void onAdServerResponseSuccess(JSONObject response);

    abstract void onMediaServerResponseSuccess(Object media);
}
