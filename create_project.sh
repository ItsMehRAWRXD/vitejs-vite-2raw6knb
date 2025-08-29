#!/bin/bash
# create_project.sh - Creates the complete C++ Kodi project structure

echo "🚀 Creating Advanced Media Player C++ Project..."

# Create main project directory
mkdir -p AdvancedMediaPlayer
cd AdvancedMediaPlayer

# Create Android project structure
mkdir -p app/src/main/{cpp,java/com/mediaplayer/advanced,assets,res/{values,drawable,layout}}
mkdir -p app/libs/{arm64-v8a,armeabi-v7a}
mkdir -p keystore
mkdir -p scripts
mkdir -p downloads

echo "📁 Project structure created!"

# Create directory listing
echo "Project structure:"
find . -type d | sort