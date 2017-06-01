package com.chymeravr.analytics;

import com.chymeravr.common.Config;
import com.chymeravr.common.WebRequestQueue;

import lombok.NonNull;

/**
 * Created by robin_chimera on 5/27/2017.
 * Analytics manager wires up and returns an analytics manager for clients to use
 */

public final class AnalyticsManagerFactory {
    @NonNull
    private WebRequestQueue webRequestQueue;
    @NonNull
    private String appId;
    public AnalyticsManagerFactory(WebRequestQueue webRequestQueue, String appId){
        this.webRequestQueue = webRequestQueue;
        this.appId = appId;
    }
    public AnalyticsManager getArrayQAnalyticsManager(){
        EventQueue highPriorityEventQueue = new ArrayEventQueue(this.webRequestQueue,
                Config.getHighPriorityQueueSize(), this.appId);
        EventQueue medPriorityEventQueue = new ArrayEventQueue(this.webRequestQueue,
                Config.getMedPriorityQueueSize(), this.appId);
        EventQueue lowPriorityEventQueue = new ArrayEventQueue(this.webRequestQueue,
                Config.getLowPriorityQueueSize(), this.appId);
        ArrayQAnalyticsManager analyticsManager = new ArrayQAnalyticsManager(highPriorityEventQueue,
                medPriorityEventQueue,
                lowPriorityEventQueue);
        return analyticsManager;
    }
}
