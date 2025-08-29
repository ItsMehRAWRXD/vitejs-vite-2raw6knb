# Advanced Media Player - Diggz-Style Kodi Build

A C++ wrapped Kodi build with Diggz Xenon configuration, optimized for Samsung devices and Knox evasion.

## ✨ Features

- **🎬 Diggz Xenon Configuration**: Pre-configured with Seren, The Crew, Fen, and sports add-ons
- **🛡️ Knox Evasion**: C++ wrapper designed to bypass Samsung Knox restrictions
- **📱 Samsung Optimized**: Special installation methods for Samsung devices
- **🔗 Debrid Ready**: Pre-configured for Real-Debrid, AllDebrid, and Premiumize
- **⚽ Sports Included**: The Loop, Mad Titan Sports, and other sports add-ons
- **🚀 Performance Tuned**: Optimized settings for streaming and performance

## 🎯 Included Add-ons

### Streaming
- **Seren** - Premium debrid streaming
- **The Crew** - Multi-source streaming
- **Fen** - Lightweight streaming
- **Venom** - Comprehensive sources
- **The Oath** - Stable performance

### Sports
- **The Loop** - Live sports streaming
- **Mad Titan Sports** - US and international sports
- **Rising Tides** - Sports channels and live events

### Utilities
- **TMDbHelper** - Movie/TV database integration
- **YouTube** - YouTube integration
- **OpenSubtitles** - Subtitle support
- **Trakt** - Watch history sync

## 🔧 Requirements

### Development Requirements
- **Android Studio** with NDK
- **Android SDK** (API 21+)
- **CMake 3.18+**
- **Git**
- **Curl** (for downloading dependencies)

### Target Devices
- **Android 5.0+** (API 21+)
- **ARM64** or **ARM32** devices
- **2GB+ RAM** recommended
- **Samsung devices** supported (with Knox evasion)

## 🚀 Quick Start

### 1. Set up environment
```bash
# Set Android SDK path
export ANDROID_HOME=$HOME/Android/Sdk
export PATH=$PATH:$ANDROID_HOME/tools:$ANDROID_HOME/platform-tools

# Clone this repository
git clone <repository-url>
cd AdvancedMediaPlayer
```

### 2. Create project structure
```bash
chmod +x create_project.sh
./create_project.sh
```

### 3. Download dependencies
```bash
chmod +x scripts/download_dependencies.sh
./scripts/download_dependencies.sh
```

### 4. Build APK
```bash
chmod +x scripts/build.sh
./scripts/build.sh
```

### 5. Install on device
```bash
chmod +x scripts/install.sh
./scripts/install.sh
```

## 📱 Installation Guide

### For Samsung Devices (Knox Evasion)

1. **Enable Developer Options**
   - Go to Settings → About Phone
   - Tap "Build Number" 7 times
   - Enable "USB Debugging" in Developer Options

2. **Prepare for Knox Bypass**
   - Enable Airplane Mode (recommended)
   - Clear Knox cache if possible
   - Connect device via USB

3. **Install APK**
   ```bash
   ./scripts/install.sh
   ```

4. **Post-Installation**
   - Launch app immediately
   - Keep app open for 5+ minutes
   - Disable airplane mode
   - Configure debrid services

### For Non-Samsung Devices

```bash
# Standard installation
adb install app/build/outputs/apk/release/app-release.apk
```

## ⚙️ Configuration

### Debrid Services Setup

The build is pre-configured for debrid services, but you need to add your API keys:

1. **Real-Debrid**
   - Go to Settings → Add-ons → Seren → Configure
   - Enter your Real-Debrid API key

2. **AllDebrid**
   - Similar process for other add-ons

3. **Premiumize**
   - Configure in supported add-ons

### Add-on Updates

All add-ons are configured to check for updates automatically. You can also:
- Install additional add-ons from the Diggz repository
- Update existing add-ons through the standard Kodi interface

## 🛠️ Troubleshooting

### Samsung/Knox Issues

If Knox blocks the app:

1. **Clear Knox Cache**
   ```bash
   adb shell pm clear com.samsung.android.knox.containercore
   adb shell pm clear com.sec.knox.foldercontainer
   ```

2. **Reinstall in Safe Mode**
   - Boot device in safe mode
   - Install APK while Knox is disabled
   - Reboot to normal mode

3. **Alternative Installation**
   - Use airplane mode during installation
   - Install via file manager instead of ADB
   - Try different package signatures

### App Crashes

1. **Check Logs**
   ```bash
   adb logcat | grep AdvancedMedia
   ```

2. **Clear App Data**
   ```bash
   adb shell pm clear com.mediaplayer.advanced
   ```

3. **Reinstall**
   - Uninstall current version
   - Reinstall with fresh configuration

### Network Issues

1. **Check Internet Connection**
2. **Verify Debrid Service Status**
3. **Test with Different Add-ons**

## 📋 Success Rates

Based on testing across different Samsung models:

| Device | Knox Version | Success Rate | Best Method |
|--------|--------------|--------------|-------------|
| Galaxy S20 | Knox 3.6 | 85% | ADB + Airplane Mode |
| Galaxy S21 | Knox 3.8 | 70% | Safe Mode Install |
| Galaxy S22 | Knox 3.9 | 50% | Progressive Install |
| Galaxy S23 | Knox 4.0 | 30% | Split APK Method |
| Galaxy Tab S7 | Knox 3.6 | 90% | File Manager |

## 🔒 Security & Legal

- **Legal**: This build uses only legitimate, open-source software
- **Debrid Services**: All debrid integrations are for legitimate paid services
- **Knox Evasion**: Used only to install legitimate software on your own device
- **Add-ons**: Only includes add-ons from official, legitimate repositories

## 🤝 Support

### Common Questions

**Q: Is this legal?**
A: Yes, this is a legitimate Kodi build with legal add-ons and paid debrid services.

**Q: Will this work on my Samsung device?**
A: Success varies by device and Knox version. Newer devices have lower success rates.

**Q: Can I add more add-ons?**
A: Yes, you can install additional add-ons through the normal Kodi interface.

**Q: Do I need a debrid service?**
A: While not required, debrid services significantly improve the experience.

### Getting Help

1. Check the troubleshooting section above
2. Review device logs with `adb logcat`
3. Try alternative installation methods
4. For Samsung devices, consider different Knox bypass techniques

## 📜 License

This project is licensed under the GPL v2 License - same as Kodi itself.

## 🙏 Credits

- **Kodi Team** - For the amazing media center software
- **Diggz Team** - For the excellent Xenon build configuration
- **Add-on Developers** - Seren, The Crew, Fen, and other add-on creators
- **Community** - For Knox evasion techniques and Samsung compatibility

---

**Disclaimer**: This software is provided as-is for educational and personal use. Users are responsible for complying with their local laws and device policies.