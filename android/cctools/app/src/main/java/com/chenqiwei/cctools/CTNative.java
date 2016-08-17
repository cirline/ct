package com.chenqiwei.cctools;

/**
 * Created by chqw on 16-8-17.
 */
public class CTNative {
    public static final String TAG = "CTNative";

    static {
        System.loadLibrary("cctools");
    }

    public static native String version();
}
