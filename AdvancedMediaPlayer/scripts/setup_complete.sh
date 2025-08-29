#!/bin/bash
# setup_complete.sh - Complete setup script from scratch

echo "🚀 Advanced Media Player - Complete Setup"
echo "========================================"

# Step 1: Create project structure
echo "📁 Creating project structure..."
cd /workspace
chmod +x create_project.sh
./create_project.sh

# Step 2: Copy all files to the project
echo "📋 Setting up project files..."
cd AdvancedMediaPlayer

# Copy all the created files
cp -r /workspace/AdvancedMediaPlayer/* .

# Make scripts executable
chmod +x scripts/*.sh

echo "✅ Project setup complete!"
echo ""
echo "🔄 Next steps:"
echo "1. cd AdvancedMediaPlayer"
echo "2. ./scripts/download_dependencies.sh"
echo "3. ./scripts/build.sh"
echo "4. ./scripts/install.sh"
echo ""
echo "📱 This will create a Knox-safe Kodi APK with Diggz configuration!"