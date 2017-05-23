#!/bin/bash
# bash script to simplify building native ndk libraries for chymeravr C++ libraries
# usage : ./buildNativeLibs.sh soil/libjpeg/ ... etc.

libName=$1
hmdName=$2
function buildNativeLib() {
    localLibName=$1
    cd ${localLibName}/
    echo "Cleaning $localLibName existing build . . ."
    # some weird android studio update causes permission errors unless you clean them
    ../gradlew -q clean
    echo "Building $localLibName NDK Libraries . . ."
    ../gradlew -q build
    cd ..
}

function buildAndroidLibs(){
    buildNativeLib "common"
    buildNativeLib "analytics"
    buildNativeLib "adclient"
}

if [ "$libName" = "all" ]; then
    buildAndroidLibs &
    echo "Building All Native Libraries. . . this may take a few minutes!!"
    buildNativeLib "soil" &
    buildNativeLib "libjpeg" &
    buildNativeLib "freetype" &
    wait;
    buildNativeLib "coreEngine"

    buildNativeLib "glImplementation" &
    buildNativeLib "image360" &
    wait;
    if [ "$hmdName" = "daydream" ]; then
        buildNativeLib "daydreamimplementation"
        buildNativeLib "daydreamadclient"
    elif [ "$hmdName" = "cardboard" ]; then
        buildNativeLib "cardboardimplementation"
        buildNativeLib "cardboardadclient"
    else
        buildNativeLib "gearvrimplementation"
        buildNativeLib "gearvradclient"
    fi

else
    buildNativeLib "$libName"
fi
