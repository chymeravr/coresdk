#!/bin/bash
# bash script to simplify building native ndk libraries for chymeravr C++ libraries
# usage : ./buildNativeLibs.sh soil/libjpeg/ ... etc.

libName=$1
hmdName=$2
function buildNativeLib() {
    localLibName=$1
    echo "Building $localLibName NDK Libraries . . ."
    cd ${localLibName}/
    ../gradlew -q build
    cd ..
}

if [ "$libName" = "all" ]; then
    echo "Building All Native Libraries. . . this may take a few minutes!!"
    buildNativeLib "soil"
    buildNativeLib "libjpeg"
    buildNativeLib "freetype"
    buildNativeLib "coreEngine"
    buildNativeLib "glImplementation"
    buildNativeLib "image360"
    if [ "$hmdName" = "gvr" ]; then
        buildNativeLib "image360gvr"
        buildNativeLib "gvradclient"
    else
        buildNativeLib "image360Ovrm"
        buildNativeLib "OvrMAdClient"
    fi
else
    buildNativeLib "$libName"
fi