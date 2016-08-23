package com.chenqiwei.cctools;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.support.annotation.Nullable;
import android.util.Log;

/**
 * Created by chqw on 16-8-19.
 */
public class AidlSvr extends Service {
    public static final String TAG = "AidlSvr";
    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return aidlstub;
    }

    cctoolsSvr.Stub aidlstub = new cctoolsSvr.Stub() {
        @Override
        public void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat, double aDouble, String aString) throws RemoteException {
            Log.i(TAG, String.format("basicTypes: int %d, %s", anInt, aString));
        }

        @Override
        public int add(int a, int b) throws RemoteException {
            Log.i(TAG, String.format("add: %d + %d = %d", a, b, a + b));
            return a + b;
        }
    };

}
