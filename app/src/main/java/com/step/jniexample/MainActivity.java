package com.step.jniexample;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "JNI_EXAMPLE";
    private TextView mTv;
    private ImageView mIv;

    public static boolean checkPermission(
            Activity activity) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && activity.checkSelfPermission(
                Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            activity.requestPermissions(new String[]{
                    Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE
            }, 1);

        }
        return false;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTv = findViewById(R.id.sample_text);
        mIv = findViewById(R.id.sample_iv);
        checkPermission(this);
//        getJavaSig();
        SignatureUtils.signatureVerify(MainActivity.this);
        mTv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                mTv.setText(SignatureUtils.signatureParams("user=abc"));
                onLoad();
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


    public void onLoad() {
        File dir = getExternalCacheDir();
        if (!dir.exists()) {
            dir.mkdirs();
        }

        try {
            InputStream open = getAssets().open("test.jpg");
            Bitmap inputBitmap = BitmapFactory.decodeStream(open);

            File outfile = new File(dir, "微信压缩.jpg");
            /**
             *   哈夫曼压缩  图片被压缩了 5倍
             */
            boolean compress = new CompressUtils().compress(inputBitmap, outfile.getAbsolutePath());
            mIv.setImageBitmap(BitmapFactory.decodeFile(outfile.getAbsolutePath()));

            //        质量压缩
            File output1 = new File(dir, "质量压缩.jpg");
            compressQuality(inputBitmap, output1);

            //        尺寸压缩
            File output2 = new File(dir, "尺寸压缩.jpg");
            compressSize(inputBitmap, output2);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    /**
     * 1. 质量压缩
     * 设置bitmap options属性，降低图片的质量，像素不会减少
     * 第一个参数为需要压缩的bitmap图片对象，第二个参数为压缩后图片保存的位置
     * 设置options 属性0-100，来实现压缩
     *
     * @param bmp
     * @param file
     */
    public void compressQuality(Bitmap bmp, File file) {
        // 0-100 100为不压缩
        int options = 10;
        Canvas canvas;
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        // 把压缩后的数据存放到baos中
        bmp.compress(Bitmap.CompressFormat.JPEG, 10, baos);
        try {
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(baos.toByteArray());
            fos.flush();
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 2. 尺寸压缩
     * 通过缩放图片像素来减少图片占用内存大小
     *
     * @param bmp
     * @param file
     */

    public void compressSize(Bitmap bmp, File file) {
        // 尺寸压缩倍数,值越大，图片尺寸越小
        int ratio = 6;
        // 压缩Bitmap到对应尺寸
        Bitmap result = Bitmap.createBitmap(bmp.getWidth() / ratio, bmp.getHeight() / ratio, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(result);
        Rect rect = new Rect(0, 0, bmp.getWidth() / ratio, bmp.getHeight() / ratio);
        canvas.drawBitmap(bmp, null, rect, null);

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        // 把压缩后的数据存放到baos中
        result.compress(Bitmap.CompressFormat.JPEG, 100, baos);
        try {
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(baos.toByteArray());
            fos.flush();
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
