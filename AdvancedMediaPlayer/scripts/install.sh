#!/bin/bash
# install.sh - Installation script for Samsung devices

set -e

APK_PATH="app/build/outputs/apk/release/app-release.apk"

echo "📱 Installing Advanced Media Player on Samsung device..."

# Check if APK exists
if [ ! -f "$APK_PATH" ]; then
    echo "❌ APK not found at $APK_PATH"
    echo "Please run ./scripts/build.sh first"
    exit 1
fi

# Check if device is connected
if ! adb devices | grep -q "device$"; then
    echo "❌ No Android device connected"
    echo "Please connect your device and enable USB debugging"
    exit 1
fi

# Check if it's a Samsung device
MANUFACTURER=$(adb shell getprop ro.product.manufacturer | tr -d '\r')
echo "Device manufacturer: $MANUFACTURER"

if [[ "$MANUFACTURER" == *"samsung"* ]]; then
    echo "🔍 Samsung device detected - using Knox-safe installation"
    
    # Knox evasion installation
    echo "1. Enabling airplane mode recommended (disconnect from network)"
    echo "2. Installing with Knox bypass techniques..."
    
    # Try multiple installation methods
    echo "Attempting Method 1: Standard installation..."
    if adb install -r "$APK_PATH" 2>/dev/null; then
        echo "✅ Installation successful with Method 1"
    else
        echo "Method 1 failed, trying Method 2: Force installation..."
        if adb install -r -d -g "$APK_PATH" 2>/dev/null; then
            echo "✅ Installation successful with Method 2"
        else
            echo "Method 2 failed, trying Method 3: External storage..."
            adb push "$APK_PATH" /sdcard/advanced-media-player.apk
            echo "APK copied to device. Please install manually:"
            echo "1. Open file manager on device"
            echo "2. Navigate to internal storage"
            echo "3. Tap 'advanced-media-player.apk'"
            echo "4. Install when prompted"
            exit 0
        fi
    fi
else
    echo "📱 Non-Samsung device - using standard installation"
    adb install -r "$APK_PATH"
fi

# Launch the app immediately to establish legitimacy
echo "🚀 Launching app to establish legitimacy..."
adb shell am start -n com.mediaplayer.advanced/.MainActivity

echo "✅ Installation complete!"
echo ""
echo "📋 Next steps:"
echo "1. Keep the app open for a few minutes"
echo "2. Configure your debrid services (Real-Debrid, etc.)"
echo "3. Enjoy your Diggz-style Kodi build!"
echo ""
echo "⚠️  Samsung users: If the app gets blocked later:"
echo "1. Clear Knox cache: Settings → Apps → Knox → Storage → Clear Cache"
echo "2. Reinstall in airplane mode"
echo "3. Launch immediately after installation"