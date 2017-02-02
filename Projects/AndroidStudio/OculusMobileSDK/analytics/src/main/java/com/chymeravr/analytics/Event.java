package com.chymeravr.analytics;

import java.util.HashMap;

import lombok.Getter;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

/**
 * Created by robin_chimera on 1/31/2017.
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
public final class Event {

    public enum EventType {ERROR, ADSHOW, ADCLOSE, ADCLICK, ADREQUEST,
                           ADLOAD, ADVIEWMETRICS};

    public enum Priority {HIGH, LOW, MEDIUM};

    @NonNull
    private final long timestamp;

    @NonNull
    private final EventType eventType;

    @NonNull
    @Getter
    private final Priority priority;

    @Getter
    @Setter
    private HashMap<String, String> params;

//    public Event(Long timestamp, EventType eventType, HashMap<String, String> params){
//        this.timestamp = timestamp;
//        this.eventType = eventType;
//        this.params = params;
//    }
}
