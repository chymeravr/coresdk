package com.chymeravr.analytics;

/**
 * Created by robin_chimera on 1/31/2017.
 */

public interface EventQueue {
    public void enqueue(Event event);
    public void flush();
    public boolean isFull();
}
