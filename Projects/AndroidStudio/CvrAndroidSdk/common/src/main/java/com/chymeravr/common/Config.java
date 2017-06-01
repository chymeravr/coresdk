package com.chymeravr.common;

import lombok.Getter;
import lombok.Setter;

/**
 * Created by robin_chimera on 11/30/2016.
 * Contains Configuration Options to be used by projects : adclient & analytics
 */

// TODO: 2/3/2017 Get Configuration Parameters from Server
// TODO: 5/10/2017 How to have different default configurations for different headsets? 
public final class Config {

    // TODO: 5/26/2017 use maven to do automatic versioning - we are not using int but int.int.int
    @Getter
    private static final int SdkVersion = 1;

    // TODO: 5/26/2017 use enums to encapsulate hmd ids
    @Getter
    private static final int hmdId = 1;

    @Getter
    private static final String osId = "Android";

    @Getter
    private static int daydreamAndroidVersionNo = 24;       // daydream works only with Android 7 +

    @Getter
    private static int cardboardAndroidVersionNo = 19;

    @Getter
    private static int gearVrAndroidVersionNo = 23;

    // directory structure deployed on client apps
    @Getter
    private static final String chymeraFolder = "chymeraSDKAssets/";
    @Getter
    private static final String fontAssetPath = chymeraFolder + "fonts/";
    @Getter
    private static final String image360AdAssetDirectory = chymeraFolder + "image360/";

    // asset directory from adclient - for copying into client
    @Getter
    private static final String fontDir = "fonts";
    // test image360 jpeg images
    @Getter
    private static final String image360Dir = "image360";

    // server addresses
    @Getter
    private static final String configServer = "http://13.64.245.176:8080/api/v1/getConfig/";//"http://amock.io/api/v1/robin/sdkConfigOptions";

    @Getter
    @Setter
    private static String adServer = "http://serving.chymeravr.com:8080/api/v1/ads/";

    @Getter
    @Setter
    private static String analyticsServer = "http://www.serving.chymeravr.com:8081/api/v1/events/"; //"http://www.amock.io/api/v1/robin/analyticsServer/";

    @Getter
    @Setter
    private static int highPriorityQueueSize = 1;

    @Getter
    @Setter
    private static int medPriorityQueueSize = 5;

    @Getter
    @Setter
    private static int lowPriorityQueueSize = 10;

    // no of threads to allocate for analytics manager
    @Getter
    @Setter
    private static int analyticsManagerThreadPoolSize = 5;

    // period between calls for HMD parameters, initial delay for sampling
    @Getter
    @Setter
    private static long hmdSamplingPeriod = 1;

    @Getter
    @Setter
    private static long hmdSamplingDelay = 1;

    // todo - fix this private public
    // TODO: 5/26/2017 this is probably not the right way to update configurations
    // todo - i don't think we need to change thread pool sizes too often
    public synchronized static void setParam(String key, String value){
        switch(key) {
            case "adServer": setAdServer(value);
                break;
            case "analyticsServer": setAnalyticsServer(value);
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
