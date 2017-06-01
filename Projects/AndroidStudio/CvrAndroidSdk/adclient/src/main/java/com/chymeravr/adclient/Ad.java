package com.chymeravr.adclient;

import android.content.Context;

import com.chymeravr.analytics.EventPriority;
import com.chymeravr.schemas.eventreceiver.EventType;
import com.chymeravr.schemas.serving.AdFormat;

import java.util.Map;

import lombok.Getter;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

/**
 * Created by robin_chimera on 12/5/2016.
 * an abstract ad class to hold common ad attributes and methods
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
public abstract class Ad {

    @Getter
    @NonNull
    private final AdFormat adFormat;

    @Getter
    @Setter
    private String advertisingId;

    @Getter
    @Setter
    private String placementId;

    @Getter
    @Setter
    private String servingId;

    @Getter
    @Setter
    private int instanceId;

    @Getter
    @NonNull
    private final Context context;

    @Getter
    @NonNull
    private VrAdListener vrAdListener;

    @Getter
    @Setter
    private String clickUrl;

    // these variables needed to be guarded by a lock & synchronized at all times
    @Getter
    @Setter
    private volatile boolean isLoading = false;

    @Getter
    @Setter
    private volatile boolean isLoaded = false;

    // below member functions have to be implemented by other derived classes
    protected abstract void emitEvent(EventType eventType, EventPriority priority,
                                      Map<String, String> map);

    public abstract void loadAd(VrAdRequest vrAdRequest);

    public abstract void show();

}
