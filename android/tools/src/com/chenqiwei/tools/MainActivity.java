package com.chenqiwei.tools;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
	public static final String tag = "chenqiwei.tools";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {
    	private TextView tv_phone_ip;
    	private TextView tv_version;
    	
    	private Button btn_open, btn_close;
    	private Button btn_socket;
    	
    	private Button btn_rb_recovery;
    	
    	private Jni jni;
    	
		private OnClickListener l = new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				switch(arg0.getId()) {
				case R.id.btn_jni_start:
					Jni.enable(true);
					break;
				case R.id.btn_jni_stop:
					Jni.enable(false);
					break;
				case R.id.btn_filesync:
					Jni.fileSync();
					break;
				case R.id.btn_reboot_recovery:
					Jni.exec("reboot recovery");
				}
			}
		};

        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_main, container, false);
            
            tv_phone_ip = (TextView) rootView.findViewById(R.id.tv_phone_ip);
            tv_version = (TextView) rootView.findViewById(R.id.tv_version);
            
            btn_open = (Button) rootView.findViewById(R.id.btn_jni_start);
            btn_close = (Button) rootView.findViewById(R.id.btn_jni_stop);
            
            btn_socket = (Button) rootView.findViewById(R.id.btn_filesync);
            
            btn_rb_recovery = (Button) rootView.findViewById(R.id.btn_reboot_recovery);
            
            Status status = new Status(this.getActivity());
            Log.i(tag, "ip = " + status.getIP());
            tv_phone_ip.setText(status.getIP());
            
            tv_version.setText(Jni.version());
            
            btn_open.setOnClickListener(l);
            btn_close.setOnClickListener(l);
            btn_socket.setOnClickListener(l);
            
            btn_rb_recovery.setOnClickListener(l);
            
            jni = new Jni();
            
            return rootView;
        }
    }

}
