package com.chymeravr.analytics;

import java.util.Map;

import lombok.Getter;
import lombok.NonNull;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

/**
 * Created by robin_chimera on 1/31/2017.
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
public final class Event {

    public enum EventType {ERROR, AD_SHOW, AD_CLOSE, AD_CLICK, AD_REQUEST,
                           AD_LOAD, AD_VIEW_METRICS};

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
    private Map<String, Object> params;

    @NonNull
    private String servingId;

    @NonNull
    private int instanceId = -1;

    public Event(Long timestamp, EventType eventType, Priority priority,
                 Map<String, Object> params, String servingId,
                 int instanceId){

        this.timestamp = timestamp;
        this.eventType = eventType;
        this.priority = priority;
        this.params = params;

        this.servingId = servingId;
        this.instanceId = instanceId;
    }
}
