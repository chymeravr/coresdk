package com.chymeravr.adclient;

import android.content.Context;
import android.location.Location;

import java.util.Date;
import java.util.Set;

import lombok.Builder;
import lombok.Getter;

/**
 * Created by robin_chimera on 11/28/2016.
 */

/*
    AdRequest encapsulates targeting information for ads.
    Use Builder to create these objects (lombok annotations)
 */

// TODO: 2/6/2017 include email id as well 
@Builder
public final class AdRequest {

    public enum ERROR_CODE {
        INTERNAL_ERROR,
        INVALID_REQUEST,
        NETWORK_ERROR,
        NO_FILL                         // no fill - no ad available
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

        private Gender(String value) {
            this.value = value;
        }
    }

    @Getter
    private final Gender gender;

    @Getter
    private final Date birthday;

    @Getter
    private final Location location;

    @Getter
    private final String email;

    public static final String DEVICE_ID_EMULATOR = "";

    public boolean isTestDevice(Context context) {
        return false;
    }

    public Set<String> getKeywords() {
        return null;
    }

}