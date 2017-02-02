package com.chymeravr.analytics;

import android.content.Context;
import android.util.Log;

import com.chymeravr.common.Config;
import com.chymeravr.common.WebRequestQueue;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by robin_chimera on 2/1/2017.
 */

public final class AnalyticsManager {
    public static final String TAG = "ChymeraVR:AnalyticsMgr";

    private static boolean initialized = false;

    private static ExecutorService executorService;

    private static AnalyticsManager manager;

    private static EventQueue highPriorityQueue;
    private static EventQueue medPriorityQueue;
    private static EventQueue lowPriorityQueue;

    private AnalyticsManager(Context context, WebRequestQueue webRequestQueue) {
        highPriorityQueue = new ArrayEventQueue(Config.HIGHPRIORITYQUEUESIZE, webRequestQueue);
        medPriorityQueue = new ArrayEventQueue(Config.MEDPRIORITYQUEUESIZE, webRequestQueue);
        lowPriorityQueue = new ArrayEventQueue(Config.LOWPRIORITYQUEUESIZE, webRequestQueue);
    }

    public static AnalyticsManager getInstance(Context context, WebRequestQueue webRequestQueue){
        if (manager == null) {
            manager = new AnalyticsManager(context, webRequestQueue);
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

    public static void push(final Event event){
        Log.v(TAG, "pushing message into analytics manager");
        executorService.execute(new Runnable(){
            @Override
            public void run() {
                switch(event.getPriority()){
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
                highPriorityQueue.flush();
                medPriorityQueue.flush();
                lowPriorityQueue.flush();
            }
        });

        executorService.shutdown();
        initialized = false;
        return true;
    }
}
