package com.step.jniexample;

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
}
