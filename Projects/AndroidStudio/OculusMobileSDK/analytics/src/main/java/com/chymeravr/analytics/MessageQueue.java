package com.chymeravr.analytics;

/**
 * Created by robin_chimera on 1/31/2017.
 */

public interface MessageQueue {
    public void enqueue(Message message);
    public void flush();
    public boolean isFull();
}
