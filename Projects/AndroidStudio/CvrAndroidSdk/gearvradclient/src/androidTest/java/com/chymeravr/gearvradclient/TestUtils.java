package com.chymeravr.gearvradclient;

import android.location.Location;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by robin_chimera on 12/12/2016.
 */

public final class TestUtils {

    /* Creates a dummy location to pass around and test get set methods */
    public static Location createLocation() {
        final String PROVIDER = "flp";
        final double lat = 37.377166;
        final double lng = -122.086966;
        final float accuracy = 3.0f;
        /*
         * From input arguments, create a single Location with provider set to
         * "flp"
         */

        // Create a new Location
        Location newLocation = new Location(PROVIDER);
        newLocation.setLatitude(lat);
        newLocation.setLongitude(lng);
        newLocation.setAccuracy(accuracy);
        return newLocation;
    }

    /* creates a dummy DOB to test getters and setters */
    public static Date createDoB() throws Exception {
        String date = 1989 + "/" + 7 + "/" + 15;
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy/MM/dd");
        Date testDate = formatter.parse(date);
        return testDate;
    }
}
