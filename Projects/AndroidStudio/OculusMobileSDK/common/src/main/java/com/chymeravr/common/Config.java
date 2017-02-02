package com.chymeravr.common;//package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/30/2016.
 */

public class Config {
    public static final String adServer = "http://www.amock.io/api/v1/robin/adServer/";            // Figure out the correct name for this
    public static final String analyticsServer = "http://www.amock.io/api/v1/robin/analyticsServer/";            // Figure out the correct name for this
    public static final int androidVersionNo = 23;

    public static int HIGHPRIORITYQUEUESIZE = 1;
    public static int MEDPRIORITYQUEUESIZE = 5;
    public static int LOWPRIORITYQUEUESIZE = 1000;

    public static int analyticsManagerThreadPoolSize = 1;
}
