package com.chymeravr.common;

import android.content.Context;

/**
 * Created by robin_chimera on 5/29/2017.
 * This is not really a factory ~ just a wrapper to prevent initialization of
 * webrequest queue by client
 */

public final class WebRequestQueueFactory {

    public WebRequestQueueFactory(Context context){
        WebRequestQueueImplementation.setInstance(context);
    }
    public WebRequestQueue getDefaultWebRequestQueue(){
            return WebRequestQueueImplementation.getInstance();
    }
}
