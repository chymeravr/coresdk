package com.chymeravr.analytics;

import android.util.Log;

import com.chymeravr.common.Config;
import com.chymeravr.schemas.eventreceiver.SDKEvent;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import lombok.NonNull;

/**
 * Created by robin_chimera on 2/1/2017.
 * Manages calls to Event Server for sending analytics info
 * Buffers requests based on priority for better network performance
 * uses executor services to do stuff in the background
 * we are not enforcing singleton ness ~ kindly do not initialize and use
 * more than one.
 */

final class ArrayQAnalyticsManager implements AnalyticsManager {
    public static final String TAG = "ArrayQAnalyticsManager";

    private boolean initialized = false;

    private ExecutorService executorService;

    // resource lock guards the three event queues
    private Lock resourceLock = new ReentrantLock();

    // event queues for sdk - they are always accessed together - insertion or flushing
    @NonNull
    private EventQueue highPriorityQueue;
    @NonNull
    private EventQueue medPriorityQueue;
    @NonNull
    private EventQueue lowPriorityQueue;

    ArrayQAnalyticsManager(EventQueue highPriorityQueue, EventQueue medPriorityQueue,
                           EventQueue lowPriorityQueue) {
        this.highPriorityQueue = highPriorityQueue;
        this.medPriorityQueue = medPriorityQueue;
        this.lowPriorityQueue = lowPriorityQueue;

    }

    @Override
    public boolean initialize(){
        if(this.initialized) {
            return false;
        }
        this.executorService = Executors.newFixedThreadPool(Config.getAnalyticsManagerThreadPoolSize());
        return true;
    }

    @Override
    public void push(SDKEvent event, EventPriority priority){
        final SDKEvent eventF = event;
        final EventPriority priorityF = priority;
        this.executorService.execute(new Runnable(){
            @Override
            public void run() {
                resourceLock.lock();
                switch(priorityF){
                    case HIGH:
                        highPriorityQueue.enqueue(eventF);
                        break;
                    case MEDIUM:
                        medPriorityQueue.enqueue(eventF);
                        break;
                    case LOW:
                        lowPriorityQueue.enqueue(eventF);
                        break;
                }
                resourceLock.unlock();
            }
        });
    }

    @Override
    public void reConfigure(){
        Log.d(TAG, "Re Configuring Analytics Manager based on new parameters from server");
        executorService.execute(new Runnable(){
            @Override
            public void run(){
                resourceLock.lock();
                highPriorityQueue.reSize(Config.getHighPriorityQueueSize());
                medPriorityQueue.reSize(Config.getMedPriorityQueueSize());
                lowPriorityQueue.reSize(Config.getLowPriorityQueueSize());
                resourceLock.unlock();
            }
        });
    }

    @Override
    public boolean shutdown(){
        if(!initialized){
            return false;
        }
        Log.d(TAG, "Initiation Analytics Manager Shutdown");
        executorService.execute(new Runnable(){
            @Override
            public void run() {
                resourceLock.lock();
                highPriorityQueue.flush();
                medPriorityQueue.flush();
                lowPriorityQueue.flush();
                resourceLock.unlock();
            }
        });

        executorService.shutdown();
        initialized = false;
        return true;
    }
}
