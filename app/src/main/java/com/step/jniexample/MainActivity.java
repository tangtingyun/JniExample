package com.step.jniexample;

import androidx.appcompat.app.AppCompatActivity;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "JNI_EXAMPLE";
    private TextView mTv;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTv = findViewById(R.id.sample_text);
        getJavaSig();
        SignatureUtils.signatureVerify(MainActivity.this);
        mTv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mTv.setText(SignatureUtils.signatureParams("user=abc"));
            }
        });
    }

    private void getJavaSig() {
        try {
            PackageInfo packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] apkSigners = packageInfo.signatures;
            for (int i = 0; i < apkSigners.length; i++) {
                Log.e(TAG, "index:" + i + " sign: " + apkSigners[i].toCharsString());
            }
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
    }

}
