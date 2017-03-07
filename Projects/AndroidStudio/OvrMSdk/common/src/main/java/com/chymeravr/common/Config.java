package com.chymeravr.common;

import lombok.Getter;
import lombok.Setter;

/**
 * Created by robin_chimera on 11/30/2016.
 * Contains Configuration Options to be used by projects : adclient & analytics
 */

// TODO: 2/3/2017 Get Configuration Parameters from Server
public class Config {
    public static final String TAG = "Config";

    public static final int SdkVersion = 1;

    public static final int hmdId = 1;

    public static final String osId = "Android";

    @Getter
    static final String fontPath = "chymeraSDKAssets/fonts/";

    public static final String Image360AdAssetDirectory = "chymeraSDKAssets/image360/";

    public static final String configServer = "http://13.64.245.176:8080/api/v1/getConfig/";//"http://amock.io/api/v1/robin/sdkConfigOptions";

    @Setter
    public static String adServer = "http://serving.chymeravr.com:8080/api/v1/ads/";
    @Setter
    public static String analyticsServer = "http://www.serving.chymeravr.com:8081/api/v1/events/"; //"http://www.amock.io/api/v1/robin/analyticsServer/";
    @Setter
    public static int androidVersionNo = 23;

    @Setter
    public static int highPriorityQueueSize = 1;
    @Setter
    public static int medPriorityQueueSize = 5;
    @Setter
    public static int lowPriorityQueueSize = 10;

    // no of threads to allocate for analytics manager
    @Setter
    public static int analyticsManagerThreadPoolSize = 1;

    // period between calls for HMD parameters, initial delay for sampling
    @Setter
    public static long hmdSamplingPeriod = 1;

    @Setter
    public static long hmdSamplingDelay = 1;

    // todo - fix this private public
    public static void setParam(String key, String value){
        switch(key) {
            case "adServer": setAdServer(value);
                break;
            case "analyticsServer": setAnalyticsServer(value);
                break;
            case "androidVersionNo": setAndroidVersionNo(Integer.parseInt(value));
                break;
            case "highPriorityQueue": setHighPriorityQueueSize(Integer.parseInt(value));
                break;
            case "medPriorityQueue": setMedPriorityQueueSize(Integer.parseInt(value));
                break;
            case "lowPriorityQueue": setLowPriorityQueueSize(Integer.parseInt(value));
                break;
            case "analyticsManagerThreadPoolSize": setAnalyticsManagerThreadPoolSize(Integer.parseInt(value));
                break;
            case "hmdSamplingPeriod": setHmdSamplingPeriod(Long.parseLong(value));
                break;
            case "hmdSamplingDelay": setHmdSamplingDelay(Long.parseLong(value));
                break;
            default:
                break;
        }
    }

}
