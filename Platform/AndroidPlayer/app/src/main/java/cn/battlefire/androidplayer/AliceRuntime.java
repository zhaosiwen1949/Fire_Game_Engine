package cn.battlefire.androidplayer;

import android.content.res.AssetManager;

public class AliceRuntime {
    static {
        System.loadLibrary("AliceRuntime");
    }
    //初始化Alice运行时核心
    public static native void Init(AssetManager assetMgr, String outerRootPath, int width, int height, int core_version, String entry_script_bundle, String entry_script);
    public static native void OnViewportChanged(int width, int height);
    public static native int Render();
    public static native void SetVideoTexture(int texture);
    public static native void InitJNIUtils();
    public static native void OnSurfaceCreated();
    public static native void Pause();
    public static native void Resume();
    public static native void OnReturn();//show exit ui
    public static native void OnShake();//show exit ui
    //QRCode
    public static native void OnGetInfoFromQRCode(String info);
    public static native void OnGetImageFromAlbum(String info);
    public static native void OnGetImageDataFromAlbum(String info);
    public static native void SetDeviceInfo(int cpu_count,String deviceName,String cpu_info,String system_info,int memory_size);
    public static native void SetScreenInfo(int width,int height,float scale,float xdpi,float ydpi,float dpi);
    //事件接口
    public static native void OnTouchBegin(long touchID,float x,float y);
    public static native void OnTouchEnd(long touchID,float x,float y);
    public static native void OnTouchCancel(long touchID,float x,float y);
    public static native void OnTouchMove(long touchID,float x,float y);
    public static native void OnIMEChar(byte[] utf8Str);
    public static native void OnIMECompositionString(byte[] utf8Str,boolean isResult);
    public static native void OnEditEnd();
    //Google Admob SDK
    public static native void SetDeviceToken(String token);
    //reward video
    public static native void OnRewardVideoLoaded();
    public static native void OnRewardVideoLoadFailed();
    public static native void OnRewardVideoOpen();
    public static native void OnRewardVideoStartPlaying();
    public static native void OnRewardVideoClose();
    public static native void OnRewardVideoCompletePlaying();
    public static native void OnRewardVideoLeaveApplication();
    public static native void OnRewardVideoReward(String name,int count);
    //banner
    public static native void OnBannerLoaded();
    public static native void OnBannerLoadFailed();
    public static native void OnBannerWillPresent();
    public static native void OnBannerWillDismiss();
    public static native void OnBannerDidDismiss();
    public static native void OnBannerLeaveApplication();
    //interstitial
    public static native void OnInterstitialLoaded();
    public static native void OnInterstitialLoadFailed();
    public static native void OnInterstitialWillPresent();
    public static native void OnInterstitialFailedToPresent();
    public static native void OnInterstitialWillDismiss();
    public static native void OnInterstitialDidDismiss();
    public static native void OnInterstitialLeaveApplication();
}
