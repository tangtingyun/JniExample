package com.step.jniexample;

import android.content.Context;

public class SignatureUtils {


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * @param params
     * @return
     */
    public static native String signatureParams(String params);

    public static native void signatureVerify(Context context);
}
