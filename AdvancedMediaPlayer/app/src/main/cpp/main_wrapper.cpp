#include <jni.h>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <dlfcn.h>
#include <string>
#include <thread>
#include <chrono>
#include "diggz_config.h"
#include "knox_evasion.h"

#define LOG_TAG "AdvancedMedia"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static JavaVM* g_jvm = nullptr;
static jobject g_activity = nullptr;
static std::string g_app_data_dir;

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_jvm = vm;
    LOGI("Native library loaded successfully");
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_mediaplayer_advanced_NativeEngine_initialize(JNIEnv *env, jobject thiz, jobject activity) {
    LOGI("Initializing Advanced Media Player...");
    
    // Store activity reference
    g_activity = env->NewGlobalRef(activity);
    
    // Get app data directory
    jclass context_class = env->FindClass("android/content/Context");
    jmethodID get_files_dir = env->GetMethodID(context_class, "getFilesDir", "()Ljava/io/File;");
    jobject files_dir_obj = env->CallObjectMethod(activity, get_files_dir);
    
    jclass file_class = env->FindClass("java/io/File");
    jmethodID get_path = env->GetMethodID(file_class, "getAbsolutePath", "()Ljava/lang/String;");
    jstring path_str = (jstring) env->CallObjectMethod(files_dir_obj, get_path);
    
    const char* app_data_dir = env->GetStringUTFChars(path_str, nullptr);
    g_app_data_dir = std::string(app_data_dir);
    env->ReleaseStringUTFChars(path_str, app_data_dir);
    
    LOGI("App data directory: %s", g_app_data_dir.c_str());
}

JNIEXPORT jboolean JNICALL
Java_com_mediaplayer_advanced_NativeEngine_checkKnoxStatus(JNIEnv *env, jobject thiz) {
    LOGI("Checking Knox status...");
    
    KnoxEvasion knox_checker;
    bool knox_active = knox_checker.isKnoxActive();
    
    if (knox_active) {
        LOGI("Knox detected - applying evasion techniques");
        knox_checker.applyEvasion();
    } else {
        LOGI("Knox not detected or inactive");
    }
    
    return knox_active ? JNI_FALSE : JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_mediaplayer_advanced_NativeEngine_loadDiggzBuild(JNIEnv *env, jobject thiz) {
    LOGI("Loading Diggz configuration...");
    
    // Get asset manager
    jclass activity_class = env->GetObjectClass(g_activity);
    jmethodID get_assets = env->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject asset_manager_obj = env->CallObjectMethod(g_activity, get_assets);
    AAssetManager* asset_manager = AAssetManager_fromJava(env, asset_manager_obj);
    
    // Load Diggz configuration
    DiggzConfigLoader loader(asset_manager, g_app_data_dir);
    
    if (!loader.extractDiggzAssets()) {
        LOGE("Failed to extract Diggz assets");
        return;
    }
    
    if (!loader.setupDiggzUserdata()) {
        LOGE("Failed to setup Diggz userdata");
        return;
    }
    
    if (!loader.configureDiggzAddons()) {
        LOGE("Failed to configure Diggz add-ons");
        return;
    }
    
    LOGI("Diggz configuration loaded successfully");
}

JNIEXPORT jint JNICALL
Java_com_mediaplayer_advanced_NativeEngine_startKodi(JNIEnv *env, jobject thiz) {
    LOGI("Starting Kodi engine...");
    
    // Delay startup to avoid Knox detection
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Load Kodi library dynamically
    std::string kodi_lib_path = g_app_data_dir + "/lib/libkodi.so";
    void* kodi_handle = dlopen(kodi_lib_path.c_str(), RTLD_LAZY);
    
    if (!kodi_handle) {
        LOGE("Failed to load Kodi library: %s", dlerror());
        return -1;
    }
    
    // Get Kodi main function
    typedef int (*kodi_main_func)(int, char**);
    kodi_main_func kodi_main = (kodi_main_func) dlsym(kodi_handle, "main");
    
    if (!kodi_main) {
        LOGE("Failed to find Kodi main function: %s", dlerror());
        dlclose(kodi_handle);
        return -1;
    }
    
    LOGI("Kodi library loaded, starting main function...");
    
    // Prepare Kodi arguments
    char* argv[] = {
        const_cast<char*>("kodi"),
        const_cast<char*>("--windowing=android"),
        const_cast<char*>("--portable"),
        nullptr
    };
    
    // Start Kodi in separate thread to avoid blocking
    std::thread kodi_thread([kodi_main]() {
        char* thread_argv[] = {
            const_cast<char*>("kodi"),
            const_cast<char*>("--windowing=android"),
            const_cast<char*>("--portable"),
            nullptr
        };
        int result = kodi_main(3, thread_argv);
        LOGI("Kodi main returned with code: %d", result);
    });
    
    kodi_thread.detach();
    
    return 0;
}

JNIEXPORT void JNICALL
Java_com_mediaplayer_advanced_NativeEngine_hideFromKnox(JNIEnv *env, jobject thiz) {
    LOGI("Applying Knox evasion techniques...");
    
    KnoxEvasion knox_evasion;
    knox_evasion.hideProcessName();
    knox_evasion.obfuscateMemory();
    knox_evasion.delayExecution();
    
    LOGI("Knox evasion applied");
}

}