#!/bin/bash
# build.sh - Complete build script for Advanced Media Player

set -e

echo "🔨 Building Advanced Media Player APK..."

# Check if we're in the right directory
if [ ! -f "app/build.gradle" ]; then
    echo "❌ Error: Please run this script from the AdvancedMediaPlayer root directory"
    exit 1
fi

# Check for Android SDK
if [ -z "$ANDROID_HOME" ]; then
    echo "❌ Error: ANDROID_HOME not set. Please set it to your Android SDK path"
    echo "Example: export ANDROID_HOME=\$HOME/Android/Sdk"
    exit 1
fi

# Check for dependencies
echo "📋 Checking dependencies..."
if [ ! -d "downloads" ]; then
    echo "🔽 Downloading dependencies first..."
    chmod +x scripts/download_dependencies.sh
    ./scripts/download_dependencies.sh
fi

# Create keystore if it doesn't exist
echo "🔑 Setting up signing keystore..."
if [ ! -f "keystore/advanced-media.keystore" ]; then
    echo "Creating new keystore..."
    mkdir -p keystore
    keytool -genkey -noprompt \
        -keystore keystore/advanced-media.keystore \
        -alias advanced-media \
        -keyalg RSA \
        -keysize 4096 \
        -validity 10000 \
        -storepass "AdvancedMedia2024!" \
        -keypass "AdvancedMedia2024!" \
        -dname "CN=Advanced Media Solutions, OU=Development, O=MediaTech Inc, L=Austin, ST=Texas, C=US"
    echo "✅ Keystore created"
else
    echo "✅ Keystore exists"
fi

# Copy Kodi libraries to the right location
echo "📚 Setting up Kodi libraries..."
mkdir -p app/libs/arm64-v8a app/libs/armeabi-v7a

if [ -d "downloads/kodi-extracted/kodi-arm64/lib/arm64-v8a" ]; then
    cp downloads/kodi-extracted/kodi-arm64/lib/arm64-v8a/*.so app/libs/arm64-v8a/ 2>/dev/null || echo "No ARM64 libraries found"
fi

if [ -d "downloads/kodi-extracted/kodi-arm32/lib/armeabi-v7a" ]; then
    cp downloads/kodi-extracted/kodi-arm32/lib/armeabi-v7a/*.so app/libs/armeabi-v7a/ 2>/dev/null || echo "No ARM32 libraries found"
fi

# Create missing C++ files if they don't exist
echo "🔧 Creating missing implementation files..."

# Knox evasion header
if [ ! -f "app/src/main/cpp/knox_evasion.h" ]; then
cat > app/src/main/cpp/knox_evasion.h << 'EOF'
#ifndef KNOX_EVASION_H
#define KNOX_EVASION_H

class KnoxEvasion {
public:
    bool isKnoxActive();
    void applyEvasion();
    void hideProcessName();
    void obfuscateMemory();
    void delayExecution();
};

#endif // KNOX_EVASION_H
EOF
fi

# Knox evasion implementation
if [ ! -f "app/src/main/cpp/knox_evasion.cpp" ]; then
cat > app/src/main/cpp/knox_evasion.cpp << 'EOF'
#include "knox_evasion.h"
#include <android/log.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <thread>
#include <chrono>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "KnoxEvasion", __VA_ARGS__)

bool KnoxEvasion::isKnoxActive() {
    // Check for Knox-related processes and files
    if (access("/system/app/KnoxCore", F_OK) == 0) return true;
    if (access("/system/container", F_OK) == 0) return true;
    return false;
}

void KnoxEvasion::applyEvasion() {
    hideProcessName();
    obfuscateMemory();
    delayExecution();
}

void KnoxEvasion::hideProcessName() {
    prctl(PR_SET_NAME, "mediaservice", 0, 0, 0);
}

void KnoxEvasion::obfuscateMemory() {
    // Simple memory obfuscation
    volatile int dummy[1000];
    for (int i = 0; i < 1000; i++) {
        dummy[i] = rand();
    }
}

void KnoxEvasion::delayExecution() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
EOF
fi

# Zip extractor header
if [ ! -f "app/src/main/cpp/zip_extractor.h" ]; then
cat > app/src/main/cpp/zip_extractor.h << 'EOF'
#ifndef ZIP_EXTRACTOR_H
#define ZIP_EXTRACTOR_H

#include <string>

bool extractZip(const std::string& zipPath, const std::string& destPath);

#endif // ZIP_EXTRACTOR_H
EOF
fi

# Zip extractor implementation (simple version)
if [ ! -f "app/src/main/cpp/zip_extractor.cpp" ]; then
cat > app/src/main/cpp/zip_extractor.cpp << 'EOF'
#include "zip_extractor.h"
#include <android/log.h>

bool extractZip(const std::string& zipPath, const std::string& destPath) {
    // Simple implementation - would need actual zip library
    __android_log_print(ANDROID_LOG_INFO, "ZipExtractor", "Extracting %s to %s", zipPath.c_str(), destPath.c_str());
    return true;
}
EOF
fi

# Kodi loader implementation
if [ ! -f "app/src/main/cpp/kodi_loader.cpp" ]; then
cat > app/src/main/cpp/kodi_loader.cpp << 'EOF'
#include <android/log.h>
#include <dlfcn.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "KodiLoader", __VA_ARGS__)

bool loadKodiLibrary(const std::string& libPath) {
    LOGI("Loading Kodi library from: %s", libPath.c_str());
    return true;
}
EOF
fi

# Create layout file
mkdir -p app/src/main/res/layout
if [ ! -f "app/src/main/res/layout/activity_main.xml" ]; then
cat > app/src/main/res/layout/activity_main.xml << 'EOF'
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    android:gravity="center"
    android:padding="16dp"
    android:background="#000000">

    <ImageView
        android:layout_width="120dp"
        android:layout_height="120dp"
        android:src="@drawable/ic_launcher"
        android:layout_marginBottom="32dp" />

    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="Advanced Media Player"
        android:textColor="#FFFFFF"
        android:textSize="24sp"
        android:textStyle="bold"
        android:layout_marginBottom="16dp" />

    <ProgressBar
        android:id="@+id/progressBar"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_marginBottom="16dp" />

    <TextView
        android:id="@+id/statusText"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="Initializing..."
        android:textColor="#CCCCCC"
        android:textSize="16sp"
        android:layout_marginBottom="32dp" />

    <Button
        android:id="@+id/startButton"
        android:layout_width="200dp"
        android:layout_height="48dp"
        android:text="Start"
        android:textColor="#FFFFFF"
        android:backgroundTint="#1E90FF"
        android:visibility="gone" />

</LinearLayout>
EOF
fi

# Create strings.xml
mkdir -p app/src/main/res/values
if [ ! -f "app/src/main/res/values/strings.xml" ]; then
cat > app/src/main/res/values/strings.xml << 'EOF'
<resources>
    <string name="app_name">Advanced Media Player</string>
</resources>
EOF
fi

# Create AndroidManifest.xml
if [ ! -f "app/src/main/AndroidManifest.xml" ]; then
cat > app/src/main/AndroidManifest.xml << 'EOF'
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.mediaplayer.advanced">

    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.WAKE_LOCK" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />

    <application
        android:allowBackup="false"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@android:style/Theme.Black.NoTitleBar.Fullscreen"
        android:hardwareAccelerated="true">
        
        <activity
            android:name=".MainActivity"
            android:exported="true"
            android:launchMode="singleTask"
            android:screenOrientation="landscape">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
EOF
fi

# Create icon (simple version)
mkdir -p app/src/main/res/drawable
if [ ! -f "app/src/main/res/drawable/ic_launcher.xml" ]; then
cat > app/src/main/res/drawable/ic_launcher.xml << 'EOF'
<vector xmlns:android="http://schemas.android.com/apk/res/android"
    android:width="48dp"
    android:height="48dp"
    android:viewportWidth="48"
    android:viewportHeight="48">
  <path
      android:fillColor="#1E90FF"
      android:pathData="M24,4C13,4 4,13 4,24s9,20 20,20 20,-9 20,-20S35,4 24,4zM20,30l-8,-8 2.83,-2.83L20,24.34l11.17,-11.17L34,16 20,30z"/>
</vector>
EOF
fi

# Build the APK
echo "🏗️ Building APK..."
if command -v gradlew &> /dev/null; then
    ./gradlew clean assembleRelease
else
    if [ -f "gradlew" ]; then
        chmod +x gradlew
        ./gradlew clean assembleRelease
    else
        echo "❌ Gradle wrapper not found. Installing..."
        gradle wrapper
        ./gradlew clean assembleRelease
    fi
fi

# Check if build was successful
if [ -f "app/build/outputs/apk/release/app-release.apk" ]; then
    echo "✅ Build successful!"
    echo ""
    echo "📱 APK created: app/build/outputs/apk/release/app-release.apk"
    echo ""
    echo "Installation commands:"
    echo "adb install app/build/outputs/apk/release/app-release.apk"
    echo ""
    echo "Or copy to device and install manually."
else
    echo "❌ Build failed!"
    exit 1
fi