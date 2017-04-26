package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/28/2016.
 * provides an interface for listening in to ad lifecycle
 */

public interface VrAdListener {

    void onVrAdLoaded();

    void onVrAdLoadFailed(VrAdRequest.Error errorReason, String additionalErrorInfo);

    void onVrAdOpened();

    void onVrAdClosed();
}
