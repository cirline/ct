package com.chenqiwei.cctools;

import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = "cctools";
    public static final int HDLR_LOCATION_INFO = 1;

    cctoolsSvr aidlsvr;

    static TextView tv_location_info;

    public static Handler hdlr = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case HDLR_LOCATION_INFO:
                    tv_location_info.setText((String)msg.obj);
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.i(TAG, "onCreate,a,a: " + CTNative.version());

        Log.i(TAG, "onCreate: CCLocation test");
        CCLocation ccl = CCLocation.getInstance(this);
        ccl.registerLocationChangeListener();

        tv_location_info = (TextView)findViewById(R.id.info_location);

        Intent intent = new Intent(this, AidlSvr.class);
        startService(intent);

        bindService(intent, sc, BIND_AUTO_CREATE);
    }

    /**
     * AIDL test
     */
    ServiceConnection sc = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            aidlsvr = cctoolsSvr.Stub.asInterface(service);
            try {
                aidlsvr.basicTypes(1, 2, true, 3, 4, "hello aidl");

                int a = aidlsvr.add(5, 6);
                Log.i(TAG, "onServiceConnected: 5 + 6 = " + a);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {

        }
    };

}
