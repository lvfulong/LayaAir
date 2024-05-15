-libraryjars 'C:\Users\Administrator\AppData\Local\Android\Sdk\platforms\android-28\android.jar'

-dontusemixedcaseclassnames
-keepattributes *Annotation*

-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider
-keep public class * extends android.app.backup.BackupAgentHelper
-keep public class * extends android.preference.Preference
-keep public class * extends android.app.Fragment
-keep public class com.android.vending.licensing.ILicensingService
# 保留support下的所有类及其内部类
-keep class android.support.** {*;}
-keep interface android.support.** {*;}
-keep public class * extends android.support.**
-dontwarn android.support.**



-keep class layaair.game.PlatformInterface.** {*;}
-keep class layaair.game.wrapper.** {*;}
-keep class layaair.game.device.DevID {*;}
-keep class layaair.game.browser.ConchJNI{*;}
-keep class layaair.game.browser.ExportJavaFunction {*;}
-keep class layaair.game.utility.ProcessInfo {*;}
-keep class layaair.game.utility.LayaAudioMusic {*;}
-keep class layaair.game.Notifycation.LayaNotifyManager {*;}
-keep class layaair.game.conch.ILayaEventListener {*;}
-keep class layaair.game.conch.ILayaGameEgine {*;}
-keep class layaair.game.conch.LayaConch5 {*;}
-keep class layaair.game.config.config {*;}
-keep class layaair.game.browser.LayaVideoPlayer {*;}
-keep class layaair.game.browser.Video.IVideoPlayer {*;}
-keep class layaair.game.browser.Picture.bean.* {*;}
-keep class layaair.game.browser.Picture.** {*;}
-dontwarn layaair.game.browser.LayaVideoPlayer
-dontwarn layaair.game.browser.Video.IVideoPlayer
-dontwarn layaair.game.browser.Picture.MultiImageSelectorActivity
# 保留R下面的资源
-keep class **.R$* {*;}
# 不混淆R类里及其所有内部static类中的所有static变量字段，$是用来分割内嵌类与其母体的标志
-keep public class **.R$*{
   public static final int *;
}
-keepclassmembers class **.R$* {
    public static <fields>;
}
-dontwarn **.R$*
-dontwarn **.R

-keepclasseswithmembernames class *{
    native <methods>;
}

-keepclasseswithmembers class *{
    public <init>(android.content.Context,android.util.AttributeSet);
}

-keepclasseswithmembers class *{
    public <init>(android.content.Context,android.util.AttributeSet, int);
}

-keepclassmembers class * extends android.app.Activity{
   public void *(android.view.View);
}

#-------glide
-keep public class * implements com.bumptech.glide.module.GlideModule
-keep class * extends com.bumptech.glide.module.AppGlideModule {
 <init>(...);
}
-keep public enum com.bumptech.glide.load.ImageHeaderParser$** {
  **[] $VALUES;
  public *;
}
-keep class com.bumptech.glide.load.data.ParcelFileDescriptorRewinder$InternalRewinder {
  *** rewind();
}
-dontwarn com.bumptech.glide.**
-keep class com.bumptech.glide.**{*;}
# for DexGuard only
#-keepresourcexmlelements manifest/application/meta-data@value=GlideModule
#------------

##---------------Begin: proguard configuration for Gson  ----------
# Gson uses generic type information stored in a class file when working with fields. Proguard
# removes such information by default, so configure it to keep all of it.
-keepattributes Signature
# For using GSON @Expose annotation
-keepattributes *Annotation*
# Gson specific classes
-dontwarn sun.misc.**
#-keep class com.google.gson.stream.** { *; }
# Application classes that will be serialized/deserialized over Gson
#-keep class com.google.gson.examples.android.model.** { <fields>; }
# Prevent proguard from stripping interface information from TypeAdapter, TypeAdapterFactory,
# JsonSerializer, JsonDeserializer instances (so they can be used in @JsonAdapter)
-keep class * extends com.google.gson.TypeAdapter
-keep class * implements com.google.gson.TypeAdapterFactory
-keep class * implements com.google.gson.JsonSerializer
-keep class * implements com.google.gson.JsonDeserializer
# Prevent R8 from leaving Data object members always null
-keepclassmembers,allowobfuscation class * {
  @com.google.gson.annotations.SerializedName <fields>;
}
-keep class com.google.gson.** {*;}
-dontwarn com.google.gson.**
##---------------End: proguard configuration for Gson  ----------