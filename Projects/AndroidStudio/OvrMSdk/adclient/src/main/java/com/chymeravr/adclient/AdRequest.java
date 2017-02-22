package com.chymeravr.adclient;

import android.location.Location;

import java.util.Date;

import lombok.Builder;
import lombok.Getter;

/**
 * Created by robin_chimera on 11/28/2016.
 * AdRequest encapsulates targeting information for ads.
 */

// TODO: 2/6/2017 include email id as well 
@Builder
public final class AdRequest {
    public enum Error{
        ADSERVER_FAILURE("ADSERVER_FAILURE"),               // Serious Error At the Server End - Contact support team to resolve
        INVALID_REQUEST("INVALID_REQUEST"),                 // Submitted an invalid AdRequest
        NO_AD_TO_SHOW("NO_AD_TO_SHOW"),                     // Server responded with no ad to show
        NETWORK_FAILURE("NETWORK_FAILURE"),                 // Internet connection is not working,
        UNKNOWN_FAILURE("UNKNOWN_FAILURE");                 // Screwed. Contact Support tream;

        private String value;

        public String getValue(){ return value; }

        Error(String value) { this.value = value;}
    }

    public enum Gender {
        MALE("MALE"),
        FEMALE("FEMALE"),
        OTHER("OTHER"),
        UNKNOWN("UNKNOWN");
        private String value;

        public String getValue() {
            return value;
        }

        Gender(String value) {this.value = value;}
    }

    @Getter
    private final Gender gender;

    @Getter
    private final Date birthday;

    @Getter
    private final Location location;

    @Getter
    private final String email;

}