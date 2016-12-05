package chymeravr.com.sdkclient;

import android.content.Context;

/**
 * Created by robin_chimera on 12/5/2016.
 */

public abstract class Ad {
    private Context context;
    private AdListener adListener;
    private String adUnitID;
    private String mediaUrl;

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

    public void setMediaUrl(String mediaUrl) {
        this.mediaUrl = mediaUrl;
    }

    public String getMediaUrl() {
        return mediaUrl;
    }

    abstract public void loadAd(AdRequest adRequest);
    abstract public void show();

    abstract void onAdServerResponseSuccess();
    abstract <T> void onMediaServerResponseSuccess(T media);
}
