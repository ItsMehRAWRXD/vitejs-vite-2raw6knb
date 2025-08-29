#!/bin/bash
# download_dependencies.sh - Downloads Kodi and Diggz components

set -e

echo "🔽 Downloading Kodi and Diggz components..."

# Create downloads directory
mkdir -p downloads
cd downloads

# Download Kodi APK
echo "Downloading Kodi APK..."
KODI_VERSION="20.2"
KODI_ARM64_URL="https://mirrors.kodi.tv/releases/android/arm64-v8a/kodi-${KODI_VERSION}-Nexus-arm64-v8a.apk"
KODI_ARM32_URL="https://mirrors.kodi.tv/releases/android/arm/kodi-${KODI_VERSION}-Nexus-armeabi-v7a.apk"

if [ ! -f "kodi-arm64.apk" ]; then
    echo "Downloading Kodi ARM64..."
    curl -L -o kodi-arm64.apk "$KODI_ARM64_URL"
fi

if [ ! -f "kodi-arm32.apk" ]; then
    echo "Downloading Kodi ARM32..."
    curl -L -o kodi-arm32.apk "$KODI_ARM32_URL"
fi

# Extract Kodi libraries
echo "Extracting Kodi libraries..."
if [ ! -d "kodi-extracted" ]; then
    mkdir -p kodi-extracted
    cd kodi-extracted
    
    # Use apktool to extract APK
    if command -v apktool &> /dev/null; then
        echo "Using apktool to extract..."
        apktool d ../kodi-arm64.apk -o kodi-arm64
        apktool d ../kodi-arm32.apk -o kodi-arm32
    else
        echo "Installing apktool..."
        wget https://raw.githubusercontent.com/iBotPeaches/Apktool/master/scripts/linux/apktool
        chmod +x apktool
        wget https://bitbucket.org/iBotPeaches/apktool/downloads/apktool_2.9.3.jar
        
        ./apktool d ../kodi-arm64.apk -o kodi-arm64
        ./apktool d ../kodi-arm32.apk -o kodi-arm32
    fi
    
    cd ..
fi

# Download Diggz Repository
echo "Downloading Diggz Repository..."
if [ ! -f "diggz-repo.zip" ]; then
    curl -L -o diggz-repo.zip "https://diggzrepo.github.io/repository.diggz/repository.diggz-1.0.0.zip"
fi

# Download popular add-ons
echo "Downloading add-ons..."
mkdir -p addons

# Seren
if [ ! -f "addons/seren.zip" ]; then
    echo "Downloading Seren..."
    curl -L -o addons/seren.zip "https://nixgates.github.io/packages/plugin.video.seren/plugin.video.seren-2.0.20.zip"
fi

# The Crew
if [ ! -f "addons/thecrew.zip" ]; then
    echo "Downloading The Crew..."
    curl -L -o addons/thecrew.zip "https://team-crew.github.io/plugin.video.thecrew/plugin.video.thecrew-1.0.0.zip"
fi

# TMDbHelper
if [ ! -f "addons/tmdbhelper.zip" ]; then
    echo "Downloading TMDbHelper..."
    curl -L -o addons/tmdbhelper.zip "https://jurialmunkey.github.io/repository.jurial/plugin.video.themoviedb.helper/plugin.video.themoviedb.helper-4.10.0.zip"
fi

echo "✅ Downloads completed!"

# Create asset packages
echo "📦 Creating asset packages..."
cd ..

# Create userdata package
echo "Creating userdata package..."
mkdir -p temp_assets/userdata
cp -r ../userdata_template/* temp_assets/userdata/ 2>/dev/null || echo "No userdata template found"
cd temp_assets
zip -r ../app/src/main/assets/diggz_userdata.zip userdata/
cd ..

# Create addons package
echo "Creating addons package..."
mkdir -p temp_assets/addons
cp downloads/addons/*.zip temp_assets/addons/
cd temp_assets
zip -r ../app/src/main/assets/diggz_addons.zip addons/
cd ..

# Create libraries package
echo "Creating libraries package..."
mkdir -p temp_assets/lib
if [ -d "downloads/kodi-extracted/kodi-arm64/lib" ]; then
    cp -r downloads/kodi-extracted/kodi-arm64/lib/* temp_assets/lib/
fi
cd temp_assets
zip -r ../app/src/main/assets/kodi_libs.zip lib/
cd ..

# Cleanup
rm -rf temp_assets

echo "🎉 All dependencies downloaded and packaged!"
echo ""
echo "Next steps:"
echo "1. Run ./scripts/build.sh to build the APK"
echo "2. Install the APK on your device"