package com.chenqiwei.tools;

public class Jni {
	
	public static native String version();
	
	static {
		System.loadLibrary("tools_jni");
	}

}
