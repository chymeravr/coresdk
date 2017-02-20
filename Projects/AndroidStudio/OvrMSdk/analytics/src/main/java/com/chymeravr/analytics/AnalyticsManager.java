package com.chymeravr.analytics;

import android.util.Log;

import com.chymeravr.common.Config;
import com.chymeravr.common.WebRequestQueue;
import com.chymeravr.schemas.eventreceiver.SDKEvent;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import lombok.Getter;

/**
 * Created by robin_chimera on 2/1/2017.
 * Manages calls to Event Server for sending analytics info
 * Buffers requests based on priority for better network performance
 */

public final class AnalyticsManager {
    public static final String TAG = "ChymeraVR:AnalyticsMgr";

    public enum Priority {HIGH, LOW, MEDIUM}

    private static boolean initialized = false;

    private static ExecutorService executorService;

    private static Lock resourceLock = new ReentrantLock();

    @Getter
    private static String applicationId;

    private static AnalyticsManager manager;

    private static EventQueue highPriorityQueue;
    private static EventQueue medPriorityQueue;
    private static EventQueue lowPriorityQueue;

    private AnalyticsManager(WebRequestQueue webRequestQueue, String appId) {
        highPriorityQueue = new ArrayEventQueue(Config.highPriorityQueueSize, webRequestQueue);
        medPriorityQueue = new ArrayEventQueue(Config.medPriorityQueueSize, webRequestQueue);
        lowPriorityQueue = new ArrayEventQueue(Config.lowPriorityQueueSize, webRequestQueue);

        applicationId = appId;
    }

    public static AnalyticsManager getInstance(WebRequestQueue webRequestQueue, String appId){
        if (manager == null) {
            manager = new AnalyticsManager(webRequestQueue, appId);
        }
        return manager;
    }

    public static boolean initialize(){
        if(initialized) {
            return false;
        }
        executorService = Executors.newFixedThreadPool(Config.analyticsManagerThreadPoolSize);
        return true;
    }

    public static void push(final SDKEvent event, final Priority priority){
        Log.v(TAG, "pushing message into analytics manager");
        executorService.execute(new Runnable(){
            @Override
            public void run() {
                resourceLock.lock();
                switch(priority){
                    case HIGH:
                        highPriorityQueue.enqueue(event);
                        break;
                    case MEDIUM:
                        medPriorityQueue.enqueue(event);
                        break;
                    case LOW:
                        lowPriorityQueue.enqueue(event);
                        break;
                }
                resourceLock.unlock();
            }
        });
    }

    public static void reConfigure(){
        Log.v(TAG, "Re Configuring Analytics Manager based on new parameters from server");
        executorService.execute(new Runnable(){
            @Override
            public void run(){
                resourceLock.lock();
                highPriorityQueue.reSize(Config.highPriorityQueueSize);
                medPriorityQueue.reSize(Config.medPriorityQueueSize);
                lowPriorityQueue.reSize(Config.lowPriorityQueueSize);
                resourceLock.unlock();
            }
        });
    }

    public static boolean shutdown(){
        if(!initialized){
            return false;
        }
        Log.v(TAG, "Initiation Analytics Manager Shutdown");
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
