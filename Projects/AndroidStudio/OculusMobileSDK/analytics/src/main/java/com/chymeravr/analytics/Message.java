package com.chymeravr.analytics;

import java.util.HashMap;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

/**
 * Created by robin_chimera on 1/31/2017.
 */

@RequiredArgsConstructor(suppressConstructorProperties = true)
public class Message {

    public enum Priority {HIGH, LOW, MEDIUM};

    public enum EventType {ERROR, ADSHOW, ADCLOSE, ADCLICK, ADREQUEST,
                           ADLOAD, ADVIEWMETRICS};

    private final long timestamp;

    private EventType eventType;

    @Getter
    private HashMap<String, String> params;

    public Message(Long timestamp, EventType eventType, HashMap<String, String> params){
        this.timestamp = timestamp;
        this.eventType = eventType;
        this.params = params;
    }
}
