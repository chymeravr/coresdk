package com.chymeravr.adclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.location.Location;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.Date;

import static junit.framework.Assert.assertEquals;

/**
 * Instrumentation test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(MockitoJUnitRunner.class)
public class VrAdRequestTest {

    @Test
    public void AdRequestBuilderSetGenderTest() throws Exception {
        VrAdRequest genderBuild = VrAdRequest.builder().gender(VrAdRequest.Gender.FEMALE).build();
        assertEquals(VrAdRequest.Gender.FEMALE, genderBuild.getGender());
    }

    @Test
    public void AdRequestBuilderSetBirthdayTest() throws Exception {
        Date testDate = TestUtils.createDoB();
        VrAdRequest dobBuild = VrAdRequest.builder().birthday(testDate).build();
        assertEquals(testDate, dobBuild.getBirthday());
    }

    @Test
    public void AdRequestBuilderSetLocationTest() throws Exception {
        Location location = TestUtils.createLocation();
        VrAdRequest locationBuild = VrAdRequest.builder().location(location).build();
        assertEquals(location, locationBuild.getLocation());
    }

    @Test
    public void AdRequestBuilderBuildTest() throws Exception {
        Location location = TestUtils.createLocation();
        Date dob = TestUtils.createDoB();
        VrAdRequest vrAdRequest = VrAdRequest.builder().location(location).birthday(dob).gender(VrAdRequest.Gender.MALE).build();
        assertEquals (vrAdRequest.getBirthday(), dob);
        assertEquals (vrAdRequest.getLocation(), location);
        assertEquals (vrAdRequest.getGender(), VrAdRequest.Gender.MALE);
    }


}