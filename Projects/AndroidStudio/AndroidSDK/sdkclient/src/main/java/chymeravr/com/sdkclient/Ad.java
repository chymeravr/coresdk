package chymeravr.com.sdkclient;

import android.content.Context;

import com.android.volley.RequestQueue;

import org.json.JSONObject;

/**
 * Created by robin_chimera on 12/5/2016.
 */

abstract class Ad {
    private Context context;
    private AdListener adListener;
    private String adUnitID;
    private String mediaUrl;
    private boolean isLoading;
    private boolean isLoaded;

    public Context getContext() {
        return context;
    }

    public void setContext(Context context) {
        this.context = context;
    }

    public void setAdListener(AdListener adListener) {
        this.adListener = adListener;
    }

    public AdListener getAdListener() {
        return adListener;
    }

    public String getAdUnitID() {
        return adUnitID;
    }

    public void setAdUnitID(String adUnitID) {
        this.adUnitID = adUnitID;
    }

    public boolean isLoaded() {
        return this.isLoaded;
    }

    public boolean isLoading(){
        return this.isLoading;
    };

    void setMediaUrl(String mediaUrl) {
        this.mediaUrl = mediaUrl;
    }

    String getMediaUrl() {
        return mediaUrl;
    }

    void setLoading(boolean loading) {
        isLoading = loading;
    }

    void setLoaded(boolean loaded) {
        isLoaded = loaded;
    }

    abstract public void loadAd(AdRequest adRequest);
    abstract public void show();

    abstract void onAdServerResponseSuccess(JSONObject response);
    abstract <T> void onMediaServerResponseSuccess(T media);
}
