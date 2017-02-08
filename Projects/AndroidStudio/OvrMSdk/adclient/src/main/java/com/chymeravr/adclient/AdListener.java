package com.chymeravr.adclient;

import java.io.Serializable;

/**
 * Created by robin_chimera on 11/28/2016.
 */

public interface AdListener extends Serializable {

    public void onAdLoaded();

    public void onAdFailedToLoad();

    public void onAdOpened();

    public void onAdClosed();

    public void onAdLeftApplication();

}
