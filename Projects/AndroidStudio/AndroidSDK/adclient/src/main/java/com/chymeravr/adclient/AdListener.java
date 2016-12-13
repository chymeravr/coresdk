package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/28/2016.
 */

public interface AdListener {

    public abstract void onAdLoaded();
    public abstract void onAdFailedToLoad();
    public abstract void onAdOpened();
    public abstract void onAdClosed();
    public abstract void onAdLeftApplication();

}
