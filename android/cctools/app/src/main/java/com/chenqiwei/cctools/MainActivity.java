package com.chenqiwei.cctools;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = "cctools";
    public static final int HDLR_LOCATION_INFO = 1;

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
    }


}
