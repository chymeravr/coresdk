#!/bin/bash
# bash script to simplify building native ndk libraries for chymeravr C++ libraries
# usage : ./buildNativeLibs.sh soil/libjpeg/ ... etc.

libName=$1
hmdName=$3
# in addition to debug and release builds we have a demo build which does not communicate
# with the server for ads but uses bundled test images to show ads for demo purposes
# these build types are only for adclients ~ rest all revert back to debug or release
buildType=$2
buildTypeMain=""
if ([ "$buildType" = "debugDemo" ] || [ "$buildType" = "debug" ]); then
    buildTypeMain="debug"
elif ([ "$buildType" = "releaseDemo" ] || [ "$buildType" = "release" ]); then
    buildTypeMain="release"
fi
function buildNativeLib() {
    localLibName=$1
    localBuildType=$2
    cd ${localLibName}/
    echo "Cleaning $localLibName existing build . . ."
    # some weird android studio update causes permission errors unless you clean them
    ../gradlew -q clean
    echo "Building $localLibName NDK Libraries . . ."
    if [ "$localBuildType" = "debug" ]; then
        ../gradlew -q assembleDebug
    elif [ "$localBuildType" = "debugDemo" ]; then
        ../gradlew -q assembleDebugDemo
    elif [ "$localBuildType" = "release" ]; then
        ../gradlew -q assembleRelease
    elif [ "$localBuildType" = "releaseDemo" ]; then
        ../gradlew -q assembleReleaseDemo
    else
        ../gradlew -q build
    fi
    cd ..
}

function buildAndroidLibs(){
    buildNativeLib "common"
    buildNativeLib "analytics"
    buildNativeLib "adclient"
}

if [ "$libName" = "all" ]; then
    # buildAndroidLibs &
    echo "Building All Native Libraries. . . this may take a few minutes!!"
    # soil libjpeg and freetype are external dependencies that we compile ~ always use in release
    # build these once and reuse ~ save time compiling for dev cycles
#    buildNativeLib "soil" "release" &
#    buildNativeLib "libjpeg" "release" &
#    buildNativeLib "freetype" "release" &
    wait;

    buildNativeLib "coreEngine" "$buildTypeMain"
    buildNativeLib "glImplementation" "$buildTypeMain" &
    buildNativeLib "image360" "$buildTypeMain" &
    wait;

    buildNativeLib "common" "$buildTypeMain"
    buildNativeLib "analytics" "$buildTypeMain"
    buildNativeLib "adclient" "$buildTypeMain"


    if [ "$hmdName" = "daydream" ]; then
        buildNativeLib "daydreamimplementation" "$buildTypeMain"
        buildNativeLib "daydreamadclient" "$buildType"
    elif [ "$hmdName" = "cardboard" ]; then
        buildNativeLib "cardboardimplementation" "$buildTypeMain"
        buildNativeLib "cardboardadclient" "$buildType"
    else
        buildNativeLib "gearvrimplementation" "$buildTypeMain"
        buildNativeLib "gearvradclient" "$buildType"
    fi
elif [ "$libName" = "allandroid" ]; then
    buildAndroidLibs
else
    buildNativeLib "$libName" "$buildType"
fi
