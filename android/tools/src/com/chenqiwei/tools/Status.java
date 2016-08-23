package com.chenqiwei.tools;

import android.content.Context;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

public class Status {
	private WifiManager wifiManager;
	private WifiInfo wifiInfo;
	
	private String ip = null;
	
	public Status(Context context) {
		super();
		wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
		if(wifiManager.isWifiEnabled()) {
			wifiInfo = wifiManager.getConnectionInfo();
			int ipaddr = wifiInfo.getIpAddress();
			ip = String.format("%d.%d.%d.%d", ipaddr & 0xff, (ipaddr >> 8) & 0xff, (ipaddr >> 16) & 0xff, (ipaddr >> 24) & 0xff); 
		}
	}

	public String getIP() {
		return ip == null ? "null" : ip;
	}

}
