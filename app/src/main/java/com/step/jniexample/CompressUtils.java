package com.step.jniexample;

import android.graphics.Bitmap;

public class CompressUtils {

    public CompressUtils() {
    }

    static {
        System.loadLibrary("luban");
        System.loadLibrary("native-lib");
    }

    public native boolean compress(Bitmap bitmap, String path);

}
