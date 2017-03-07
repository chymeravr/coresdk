package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/28/2016.
 * provides an interface for listening in to ad lifecycle
 */

public interface AdListener {

    void onAdLoaded();

    void onAdLoadFailed(AdRequest.Error errorReason, String additionalErrorInfo);

    void onAdOpened();

    void onAdClosed();
}
