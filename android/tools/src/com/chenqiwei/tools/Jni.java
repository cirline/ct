package com.chenqiwei.tools;

import android.util.Log;

public class Jni {
	public static final String tag = "ToolsJniClass";
	
	static {
		System.loadLibrary("tools_jni");
	}
	
	public static native String version();
	
	public static native int enable(boolean enable);
	
	public static native int exec(String s);
	
	public static void toString(int n) {
		Log.i(tag, "count = " + n);
	}

	public static native void fileSync();
}
