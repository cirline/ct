package com.chenqiwei.tools;

import android.util.Log;

public class Jni {
	public static final String tag = "ToolsJniClass";
	
	public static native String version();
	
	public static native int enable(boolean enable);
	
	public static void toString(int n) {
		Log.i(tag, "count = " + n);
		
	}
	
	static {
		System.loadLibrary("tools_jni");
	}

}
