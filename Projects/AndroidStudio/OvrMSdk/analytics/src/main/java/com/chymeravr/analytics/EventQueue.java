package com.chymeravr.analytics;

import com.chymeravr.schemas.eventreceiver.SDKEvent;

/**
 * Created by robin_chimera on 1/31/2017.
 */

public interface EventQueue {
    public int getSize();
    public void enqueue(SDKEvent event);
    public void flush();
    public boolean isFull();
    public void reSize(int size);
}
