#include <jni.h>
#include <string>
#include <android/log.h>
#include <dlfcn.h>
#include "fake_dlfcn.h"
#include "art.h"

#define TAG_NAME    "my_native"
#define log_info(fmt, args...) __android_log_print(ANDROID_LOG_INFO, TAG_NAME, (const char *) fmt, ##args)

void *(*JniIdManager_DecodeMethodId_)(void *, jlong) = nullptr;

extern "C" JNIEXPORT void JNICALL
Java_com_spx_artmethodreplacedemo_NativeArtMethodReplace_hotfix(JNIEnv *env, jobject thiz) {
    jclass someClass = env->FindClass("com/spx/artmethodreplacedemo/SomeClass");

    char api_level_str[5];
    __system_property_get("ro.build.version.sdk", api_level_str);
    int api_level = atoi(api_level_str);
    log_info("api level: %d", api_level);
    ArtHelper::init(env, api_level);
    void *handle = mydlopen("libart.so", RTLD_NOW);

    if (handle == nullptr) {
        log_info("Failed to open libart.so");
        return;
    }

    JniIdManager_DecodeMethodId_ = reinterpret_cast<void *(*)(void *, jlong)>(mydlsym(handle,
                                                                                      "_ZN3art3jni12JniIdManager14DecodeMethodIdEP10_jmethodID"));

    size_t src_art_method = reinterpret_cast<size_t> (env->GetStaticMethodID(someClass, "f1",
                                                                             "()I"));
    size_t dest_art_method = reinterpret_cast<size_t> (env->GetStaticMethodID(someClass, "f2",
                                                                              "()I"));

    log_info("src_art_method:%zu", src_art_method);
    log_info("dest_art_method:%zu", dest_art_method);
    log_info("JniIdManager_DecodeMethodId_:%p", JniIdManager_DecodeMethodId_);
    if (src_art_method % 2 == 1) {
        src_art_method = reinterpret_cast<jlong>(JniIdManager_DecodeMethodId_(
                ArtHelper::getJniIdManager(), src_art_method));
    }
    log_info("after parse src_art_method:%zu", src_art_method);

    if (dest_art_method % 2 == 1) {
        dest_art_method = reinterpret_cast<jlong>(JniIdManager_DecodeMethodId_(
                ArtHelper::getJniIdManager(), dest_art_method));
    }
    log_info("after parse dest_art_method:%zu", dest_art_method);
    size_t art_method_length = dest_art_method - src_art_method;
    log_info(" art_method_length:%d", art_method_length);
    void *smeth =(void *) src_art_method;
    void *dmeth =(void *) dest_art_method;
    memcpy(smeth, dmeth, art_method_length);
}
