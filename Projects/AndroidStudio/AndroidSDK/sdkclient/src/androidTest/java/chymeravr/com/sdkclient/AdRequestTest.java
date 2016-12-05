package chymeravr.com.sdkclient;

/**
 * Created by robin_chimera on 11/29/2016.
 */

import android.location.Location;

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
        assert(adRequest.getBirthday() == dob);
        assert(adRequest.getLocation() == location);
        assert(adRequest.getGender() == AdRequest.Gender.MALE);
    }


    /* Creates a dummy location to pass around and test get set methods */
    public Location createLocation() {
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
    public Date createDoB() throws Exception {
        String date = 1989 + "/" + 7 + "/" + 15;
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy/MM/dd");
        Date testDate = formatter.parse(date);
        return testDate;
    }

}