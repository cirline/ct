package com.chenqiwei.cctools;

import android.content.Context;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;

/**
 * Created by chqw on 16-8-18.
 */
public class CCLocation {
    public static final String TAG = "CCLocation";
    static CCLocation mInstance;
    LocationManager locationMgr;

    public CCLocation(Context context) {
        locationMgr = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
    }

    public static CCLocation getInstance(Context context) {
        if(mInstance == null) {
            mInstance = new CCLocation(context);
        }
        return mInstance;
    }

    public void registerLocationChangeListener() {
        Criteria criteria = new Criteria();
        criteria.setAccuracy(Criteria.ACCURACY_FINE);
        criteria.setAltitudeRequired(false);
        criteria.setBearingRequired(false);
        criteria.setCostAllowed(false);
        criteria.setPowerRequirement(Criteria.POWER_LOW);
        criteria.setSpeedRequired(false);

        String provider = locationMgr.getBestProvider(criteria, true);

        locationMgr.requestLocationUpdates(provider, 0, 0, ccll);
    }

    CCLocationListener ccll = new CCLocationListener();

    class CCLocationListener implements LocationListener {
        @Override
        public void onLocationChanged(Location location) {
            double latitude = location.getLatitude();
            double longitude = location.getLongitude();

            String s = String.format("%f, %f", latitude, longitude);
            Message msg = new Message();
            msg.what = MainActivity.HDLR_LOCATION_INFO;
            msg.obj = s;
            MainActivity.hdlr.sendMessage(msg);

            Log.i(TAG, "onLocationChanged: " + s);
        }

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {
            Log.i(TAG, "onStatusChanged: ");
        }

        @Override
        public void onProviderEnabled(String provider) {
            Log.i(TAG, "onProviderEnabled: ");
        }

        @Override
        public void onProviderDisabled(String provider) {
            Log.i(TAG, "onProviderDisabled: ");
        }
    }

    public void unRegisterLocationChangeListener() {
        locationMgr.removeUpdates(ccll);
    }
}
