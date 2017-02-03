package com.chymeravr.common;//package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/30/2016.
 */

// TODO: 2/3/2017 Get Configuration Parameters from Server
public class Config {
    public static final String adServer = "http://www.amock.io/api/v1/robin/adServer/";
    public static final String analyticsServer = "http://www.amock.io/api/v1/robin/analyticsServer/";
    public static final int androidVersionNo = 23;

    public static int HIGHPRIORITYQUEUESIZE = 1;
    public static int MEDPRIORITYQUEUESIZE = 5;
    public static int LOWPRIORITYQUEUESIZE = 10;

    // no of threads to allocate for analytics manager
    public static int analyticsManagerThreadPoolSize = 1;

    // period between calls for HMD parameters, initial delay for sampling
    public static long hmdSamplingPeriod = 1;
    public static long hmdSamplingDelay = 1;
}
