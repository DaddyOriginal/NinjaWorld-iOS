#!/bin/bash
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

echo "=========================================================="
echo "   NINJA WORLD - AUTO SETUP FOR XCODE (MACOS / IOS)"
echo "=========================================================="

# 1. Check or clone Cocos2d-x 2.2.6
if [ ! -d "cocos2d-x-2.2.6" ]; then
    echo "[1/4] Dang tai bo thu vien Cocos2d-x 2.2.6 (ho tro 64-bit arm64)..."
    git clone https://github.com/cocos2d/cocos2d-x.git --branch cocos2d-x-2.2.6 --depth 1 cocos2d-x-2.2.6
else
    echo "[1/4] Bo thu vien Cocos2d-x 2.2.6 da co san."
fi

# 2. Check if project template exists
TARGET_PROJ="cocos2d-x-2.2.6/projects/NinjaWorld"
if [ ! -d "$TARGET_PROJ" ]; then
    echo "[2/4] Khoi tao template du an NinjaWorld bang cocos tool..."
    python cocos2d-x-2.2.6/tools/project-creator/create-project.py -project NinjaWorld -package com.ninja.world -language cpp || \
    python2 cocos2d-x-2.2.6/tools/project-creator/create-project.py -project NinjaWorld -package com.ninja.world -language cpp || \
    python3 cocos2d-x-2.2.6/tools/project-creator/create-project.py -project NinjaWorld -package com.ninja.world -language cpp
fi

# 3. Synchronize Classes and Resources
echo "[3/4] Dang sao chep Classes va 5,204 tai nguyen goc vao du an..."
cp -R Classes/* "$TARGET_PROJ/Classes/"
cp -R Resources/* "$TARGET_PROJ/Resources/"

echo "[4/4] Hoan tat cai dat! Dang mo Xcode..."
open "$TARGET_PROJ/proj.ios_mac/NinjaWorld.xcodeproj"

echo "=========================================================="
echo "Du an da duoc mo tren Xcode!"
echo "Chi can chon Team (Apple ID) -> Cam iPhone -> Nhan Run hoac Archive len TestFlight!"
echo "=========================================================="
