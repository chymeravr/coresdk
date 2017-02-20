package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/28/2016.
 */

//public interface AdListener extends Serializable {
//
//    void onAdLoaded();
//
//    void onAdFailedToLoad();
//
//    void onAdOpened();
//
//    void onAdClosed();
//}

public abstract class AdListener{
    public void onAdLoaded(){};

    public void onAdFailedToLoad(){};

    public void onAdOpened(){};

    public void onAdClosed(){};
}
