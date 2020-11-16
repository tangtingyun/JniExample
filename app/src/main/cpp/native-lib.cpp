#include <jni.h>
#include <string>
#include "md5.h"
#include "utils/androidlog.h"

#include <android/bitmap.h>
#include <malloc.h>

extern "C"
{
#include "jpeglib.h"
}
#define true 1
#define false 0

typedef uint8_t BYTE;

// 额外附加的字符串
static const char *EXTRA_SIGNATURE = "JNI_PINK";
// 校验签名
static int is_verify = 0;

static const char *PACKAGE_NAME = "com.step.jniexample";

static const char *APP_SIGNATURE = "308202e4308201cc020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b30090603550406130255533020170d3230303330333037343031305a180f32303530303232343037343031305a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330820122300d06092a864886f70d01010105000382010f003082010a0282010100a719c85dbcc75312ab9f6d819b03dbf7518167e31b9a4bc37109d98c884c36bcf4395f9fa4a47fccc2a6b26abee86853ebc4b04e2bf7441129393066b292afcea514b6b33abdd64cd77488ef068d8a6b3816c337a09a82208230a8a10690d670c5555e67174311b355c9e08c853ba208070a4b51a35b0937fec653776eb3ae5569137d1cc5f4c5b3fd31a3e87b93ba448cd9cca432268363c09d61e30d25ce7fba6dc5b1652bf5b64b8f77d82d6aa64b3d85ca8acc3f1fa4677dd326775645fd24a2465b4f79927e3e9102b839a17f63ea2ed2335dc59247d90ea25badb965c53a465de7946ec0c738f983e3f48d6acc8ea7943c0e4b3ec6a91b99d54ab3402d0203010001300d06092a864886f70d010105050003820101000321f8b63c2aee3135eb66fee97a8e3d2896b5d9417cd0bd5bd909f25da9574062762973bacd62e7a7bf7c0897ed8be3eb791fc190760ad579459e0009d53aea05e98b736dff793c40d1df3a4ee7be9aee84d0b9f00240faa6747aa41396fb3855069a52ba3a049c8fa9e32b5f4cd2a24eaf160d891f339c474eecb31f5ccf158acf928bf27b1184682cc0588c077cfe484ff6827beb03df9e7fa79e31228cba31beca81039882929c35d1faa7447c20ceab6dd6ea905ca22c9b5cf3d968b3552efa4809a93179e2baddb6b107039651beae9cd63b39ff9f01a3e2886478c5bd63d56e6fc3ab264b103930af466acd588f5918f666991c8a0106916eab272fbb";

extern "C"
JNIEXPORT jstring JNICALL
Java_com_step_jniexample_SignatureUtils_signatureParams(JNIEnv *env, jclass clazz,
                                                        jstring paramsStr) {

    if (is_verify == 0) {
        return env->NewStringUTF("error_signature");
    }

    const char *params = env->GetStringUTFChars(paramsStr, JNI_FALSE);

    //MD5 签名规则 加点料
    std::string signature_str(params);
    // 1. 字符串前面加点料
    signature_str.insert(0, EXTRA_SIGNATURE);
    //2. 后面去掉两位
    signature_str = signature_str.substr(0, signature_str.length() - 2);

    std::string md5Sign = MD5(signature_str).toStr();


    LOGD("原始字符串 %s ", signature_str.c_str());
    LOGD("加密后字符串 %s ", md5Sign.c_str());


    // 释放资源
    env->ReleaseStringUTFChars(paramsStr, params);
    return env->NewStringUTF(signature_str.c_str());
}


/*
     PackageInfo packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] apkSigners = packageInfo.signatures;
            for (int i = 0; i < apkSigners.length; i++) {
                Log.e(TAG, "index:" + i + " sign: " + apkSigners[i].toCharsString());
            }
 * */
extern "C"
JNIEXPORT void JNICALL
Java_com_step_jniexample_SignatureUtils_signatureVerify(JNIEnv *env, jclass clazz,
                                                        jobject context) {

    // 1. 获取包名
    jclass j_clz = env->GetObjectClass(context);
    jmethodID j_method = env->GetMethodID(j_clz, "getPackageName", "()Ljava/lang/String;");
    jstring j_package_name = static_cast<jstring>(env->CallObjectMethod(context, j_method));

    // 2. 对比包名是否一样
    const char *native_package_name = env->GetStringUTFChars(j_package_name, JNI_FALSE);
    LOGD("package_name: %s", native_package_name);
    if (strcmp(native_package_name, PACKAGE_NAME) != 0) {
        env->ReleaseStringUTFChars(j_package_name, native_package_name);
        return;
    }
    // 3.获取签名
    // 3.1 获取  PackageManager
    j_method = env->GetMethodID(j_clz, "getPackageManager",
                                "()Landroid/content/pm/PackageManager;");
    jobject pack_manager = env->CallObjectMethod(context, j_method);
    // 3.2 获取 PackageInfo
    j_clz = env->GetObjectClass(pack_manager);
    j_method = env->GetMethodID(j_clz, "getPackageInfo",
                                "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject package_info = env->CallObjectMethod(pack_manager, j_method, j_package_name,
                                                 0x00000040);
    // 3.3 获取signatures数组
    j_clz = env->GetObjectClass(package_info);
    jfieldID j_filed_id = env->GetFieldID(j_clz, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatures = static_cast<jobjectArray>(env->GetObjectField(package_info,

                                                                            j_filed_id));
    // 3.4 获取signatures[0]
    jobject signatures_first = env->GetObjectArrayElement(signatures, 0);

    // 3.5 调用 signatures[0].toCharsString()
    j_clz = env->GetObjectClass(signatures_first);
    j_method = env->GetMethodID(j_clz, "toCharsString", "()Ljava/lang/String;");


    jstring j_signature_str = static_cast<jstring>(env->CallObjectMethod(signatures_first,
                                                                         j_method));
    const char *native_signature_str = env->GetStringUTFChars(j_signature_str, JNI_FALSE);
    //比对签名是否一致

    if (strcmp(native_signature_str, APP_SIGNATURE) != 0) {
        return;
    }
    LOGD("签名校验成功: %s", native_signature_str);
    // 签名认证成功
    is_verify = 1;
}


// jpeg    源码找到解决方案
int writeImg(BYTE *data, const char *path, int w, int h) {

//jpeg引擎   绕过 skia  来实现压缩
//信使
    struct jpeg_compress_struct jpeg_struct;
    //    设置错误处理信息
    jpeg_error_mgr err;
    jpeg_struct.err = jpeg_std_error(&err);
//    给结构体分配内存
    jpeg_create_compress(&jpeg_struct);

    FILE *file = fopen(path, "wb");

    if (file == NULL) {
        return 0;
    }

//    设置输出路径
    jpeg_stdio_dest(&jpeg_struct, file);

    jpeg_struct.image_width = w;
    jpeg_struct.image_height = h;
//    初始化  初始化
//改成FALSE   ---》 开启hufuman算法
    jpeg_struct.arith_code = FALSE;
    jpeg_struct.optimize_coding = TRUE;
    jpeg_struct.in_color_space = JCS_RGB;

    jpeg_struct.input_components = 3;
//    其他的设置默认
    jpeg_set_defaults(&jpeg_struct);
    jpeg_set_quality(&jpeg_struct, 20, true);
    jpeg_start_compress(&jpeg_struct, TRUE);
    JSAMPROW row_pointer[1];
//    一行的rgb
    int row_stride = w * 3;
    while (jpeg_struct.next_scanline < h) {
        row_pointer[0] = &data[jpeg_struct.next_scanline * w * 3];
        jpeg_write_scanlines(&jpeg_struct, row_pointer, 1);
    }
    jpeg_finish_compress(&jpeg_struct);

    jpeg_destroy_compress(&jpeg_struct);
    fclose(file);

    return JNI_TRUE;
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_step_jniexample_CompressUtils_compress(JNIEnv *env, jobject thiz, jobject bitmap,
                                                jstring path_) {
//    jpeg  压缩---》    rggb
//工厂整体搬迁  图片  zip   rar 1     分开打包搬迁2  每一个元数据 像素  1    r  g   b 2

    const char *path = env->GetStringUTFChars(path_, 0);

    LOGD("outfile path is %s", path);
//     宽高
    AndroidBitmapInfo bitmapInfo;
    if (AndroidBitmap_getInfo(env, bitmap, &bitmapInfo) < 0) {
        LOGD("AndroidBitmap_getInfo error");
        env->ReleaseStringUTFChars(path_, path);
        return false;
    }
    BYTE *pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels) < 0) {
        LOGD("AndroidBitmap_lockPixels error");
        env->ReleaseStringUTFChars(path_, path);
        return false;
    }
    int h = bitmapInfo.height;
    int w = bitmapInfo.width;
//
    BYTE *data, *tmpData;
    data = (BYTE *) malloc(w * h * 3);
    tmpData = data;
    BYTE r, g, b;
    int color;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            color = *((int *) pixels);
            r = ((color & 0x00FF0000) >> 16);
            g = ((color & 0x0000FF00) >> 8);
            b = ((color & 0x000000FF));

            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data += 3;
            pixels += 4;
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    int result = writeImg(tmpData, path, w, h);

    // 释放堆内存
    free(tmpData);

    env->ReleaseStringUTFChars(path_, path);

    if (result == 0) {
        return false;
    } else {
        return true;
    }

}





