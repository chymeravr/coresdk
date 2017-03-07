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

-keep class com.chymeravr.adclient.AdRequest{*;}
-keep public enum com.chymeravr.adclient.AdRequest$** {
    **[] $VALUES;
    public *;
}
-keep class com.chymeravr.adclient.AdRequest$AdRequestBuilder{*;}
-keep class com.chymeravr.adclient.Image360Ad{*;}
-keep class com.chymeravr.adclient.ChymeraVrSdk{*;}
-keep interface com.chymeravr.adclient.AdListener{*;}

# Activity classes are automatically kept

#-keep class     android.** {*;}
#-keep interface android.** {*;}
#-keep enum      android.** {*;}

#-keep class     java.** {*;}
#-keep interface java.** {*;}
#-keep enum      java.** {*;}

#-keep class     com.android.** {*;}
#-keep interface com.android.** {*;}
#-keep enum      com.android.** {*;}

#-keep class     org.mockito.** {*;}
#-keep interface org.mockito.** {*;}
#-keep enum      org.mockito.** {*;}

#-keep class     com.google.** {*;}
#-keep interface com.google.** {*;}
#-keep enum      com.google.** {*;}

#-keep class     org.projectlombok.** {*;}
#-keep interface org.projectlombok.** {*;}
#-keep enum      org.projectlombok.** {*;}

#-keep class     org.apache.** {*;}
#-keep interface org.apache.** {*;}
#-keep enum      org.apache.** {*;}

#-keep class     javax.annotation.** {*;}
#-keep interface javax.annotation.** {*;}
#-keep enum      javax.annotation.** {*;}