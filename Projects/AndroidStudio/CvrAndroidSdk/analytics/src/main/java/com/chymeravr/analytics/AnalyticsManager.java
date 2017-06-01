package com.chymeravr.analytics;

import com.chymeravr.schemas.eventreceiver.SDKEvent;

/**
 * Created by robin_chimera on 5/27/2017.
 * This interface defines a blueprint for AnalyticsManagers.
 * We should be able to reconfigure analytics managers from the server and hence the reconfigure func.
 * Also, they should support pushing events into a collection type data store for holding till the
 * next scheduled server call to send the events.
 * We need to initialize any resources (scheduler services) for our use and shutdown - flush any
 * pending events, release expensive resources etc.
 */


public interface AnalyticsManager {

    public void push(SDKEvent event, EventPriority priority);

    public boolean initialize();

    public boolean shutdown();

    public void reConfigure();
}
