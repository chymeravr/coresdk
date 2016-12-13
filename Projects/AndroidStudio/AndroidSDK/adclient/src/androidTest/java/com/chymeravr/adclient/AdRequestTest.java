package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.location.Location;

import static com.chymeravr.adclient.TestUtils.createDoB;
import static com.chymeravr.adclient.TestUtils.createLocation;
import static junit.framework.Assert.assertEquals;
import static org.hamcrest.MatcherAssert.assertThat;

import org.mockito.runners.MockitoJUnitRunner;


import org.junit.Test;
import org.junit.runner.RunWith;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
//@RunWith(AndroidJUnit4.class)
@RunWith(MockitoJUnitRunner.class)
public class AdRequestTest {

    @Test
    public void AdRequestBuilderSetGenderTest() throws Exception {
        AdRequest genderBuild = new AdRequest.Builder().setGender(AdRequest.Gender.FEMALE).build();
        assertEquals(AdRequest.Gender.FEMALE, genderBuild.getGender());
    }

    @Test
    public void AdRequestBuilderSetBirthdayTest() throws Exception {
        Date testDate = createDoB();

        AdRequest dobBuild = new AdRequest.Builder().setBirthday(testDate).build();
        assertEquals(testDate, dobBuild.getBirthday());
    }

    @Test
    public void AdRequestBuilderSetLocationTest() throws Exception {
        Location location = createLocation();

        AdRequest locationBuild = new AdRequest.Builder().setLocation(location).build();
        assertEquals(location, locationBuild.getLocation());
    }

    @Test
    public void AdRequestBuilderBuildTest() throws Exception {
        Location location = createLocation();
        Date dob = createDoB();
        AdRequest adRequest = new AdRequest.Builder().setLocation(location).setBirthday(dob)
                .setGender(AdRequest.Gender.MALE).build();
        assert (adRequest.getBirthday() == dob);
        assert (adRequest.getLocation() == location);
        assert (adRequest.getGender() == AdRequest.Gender.MALE);
    }


}