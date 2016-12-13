package com.chymeravr.adclient;

import android.content.Context;
import android.location.Location;

import java.util.Date;
import java.util.Set;

/**
 * Created by robin_chimera on 11/28/2016.
 */

/*
    AdRequest encapsulates targeting information for ads.
    Use Builder to create these objects
 */
public final class AdRequest {

    public enum ERROR_CODE{
        INTERNAL_ERROR,
        INVALID_REQUEST,
        NETWORK_ERROR,
        NO_FILL                         // no fill - no ad available
    }

    public enum Gender{
        MALE("MALE"),
        FEMALE("FEMALE"),
        OTHER("OTHER"),
        UNKNOWN("UNKNOWN");

        private String value;
        public String getValue(){
            return value;
        }
        private Gender(String value){
            this.value  = value;
        }
    }

    private Gender gender;
    private Date birthday;
    private Location location;

    public static class Builder{
        private Gender gender;
        private Date birthday;
        private Location location;

        public Builder(){}

        public Builder setGender(Gender gender){
            this.gender = gender;
            return this;
        }
        public Builder setBirthday(Date birthday){
            this.birthday = birthday;
            return this;
        }

        /* Do we need any additional sanity check on location? */
        public Builder setLocation(Location location){
            this.location = location;
            return this;
        }
        public AdRequest build(){
            return new AdRequest(this);
        }

    }

    // private constructor to enforce building of objects through builder
    private AdRequest(Builder builder){
        this.gender = builder.gender;
        this.birthday = builder.birthday;
        this.location = builder.location;
    }

    public static final String DEVICE_ID_EMULATOR="";

    public Date getBirthday(){
        return this.birthday;
    }

    public Gender getGender(){
        return this.gender;
    }

    public Location getLocation(){
        return this.location;
    }

    public boolean isTestDevice(Context context){
        return false;
    }

    public Set<String> getKeywords(){
        return null;
    }

}
