package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.location.Location;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.Date;

import static com.chymeravr.adclient.TestUtils.createDoB;
import static com.chymeravr.adclient.TestUtils.createLocation;
import static junit.framework.Assert.assertEquals;

/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(MockitoJUnitRunner.class)
public class AdRequestTest {

    @Test
    public void AdRequestBuilderSetGenderTest() throws Exception {
        AdRequest genderBuild = AdRequest.builder().gender(AdRequest.Gender.FEMALE).build();
        assertEquals(AdRequest.Gender.FEMALE, genderBuild.getGender());
    }

    @Test
    public void AdRequestBuilderSetBirthdayTest() throws Exception {
        Date testDate = createDoB();
        AdRequest dobBuild = AdRequest.builder().birthday(testDate).build();
        assertEquals(testDate, dobBuild.getBirthday());
    }

    @Test
    public void AdRequestBuilderSetLocationTest() throws Exception {
        Location location = createLocation();
        AdRequest locationBuild = AdRequest.builder().location(location).build();
        assertEquals(location, locationBuild.getLocation());
    }

    @Test
    public void AdRequestBuilderBuildTest() throws Exception {
        Location location = createLocation();
        Date dob = createDoB();
        AdRequest adRequest = AdRequest.builder().location(location).birthday(dob).gender(AdRequest.Gender.MALE).build();
        assert (adRequest.getBirthday() == dob);
        assert (adRequest.getLocation() == location);
        assert (adRequest.getGender() == AdRequest.Gender.MALE);
    }


}