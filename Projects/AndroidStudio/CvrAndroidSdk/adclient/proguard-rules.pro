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

# keep all external library
#-keep class org.projectlombok.* { *; }
#-keep interface org.projectlombok.* { *; }
#-keep class lombok.* {*;}
#-keep interface lombok.* {*;}
#
#-keep class com.android.volley.* {*;}
#-keep interface com.android.volley.* {*;}
#
#-keep class org.json.* {*;}
#-keep interface org.json.* {*;}
#
#-keep class java.io.* {*;}
#-keep interface java.io.* {*;}
#-keep class java.util.* {*;}
#-keep interface java.util.* {*;}
#-keep class java.sql.* {*;}
#
#-keep class android.app.* {*;}
#-keep interface android.app.* {*;}
#-keep class android.content.* {*;}
#-keep interface android.content.* {*;}
#-keep class android.net.* {*;}
#-keep interface android.net.* {*;}
#-keep class android.os.* {*;}
#-keep interface android.os.* {*;}
#-keep class android.util.* {*;}
#-keep interface android.util.* {*;}
#-keep class android.location.* {*;}
#-keep interface android.location.* {*;}
#
#
############ keep internal library intact ###########
#-keep class com.chymeravr.analytics.* {*;}
#-keep interface com.chymeravr.analytics.* {*;}
#-keep class com.chymeravr.common.* {*;}
#-keep interface com.chymeravr.common.* {*;}
#-keep class com.chymeravr.schemas.* {*;}
#-keep interface com.chymeravr.schemas.* {*;}
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