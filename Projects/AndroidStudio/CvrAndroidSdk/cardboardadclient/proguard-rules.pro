# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in C:\Users\robin_chimera\AppData\Local\Android\Sdk/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile
-keep public class com.chymeravr.adclient.* {
    public <methods>;
}
-keep public class com.chymeravr.adclient.Ad {
    public <methods>;
    protected <methods>;
}

-keepclassmembers class * extends java.lang.Enum {
    <fields>;
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

-keep public class com.chymeravr.analytics.* {
    public <methods>;
}
-keep public class com.chymeravr.common.* {
    public <methods>;
}
-keep public class com.chymeravr.cardboardadclient.* {
    public <methods>;
}

# schemas.jar is already minified
-keep class com.chymeravr.schemas.** {*;}