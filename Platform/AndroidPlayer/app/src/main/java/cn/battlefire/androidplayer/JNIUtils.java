package cn.battlefire.androidplayer;
import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.media.AudioManager;
import android.net.Uri;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.os.Build;
import android.util.DisplayMetrics;
import android.util.Log;

import com.esafirm.imagepicker.features.ImagePicker;
import com.esafirm.imagepicker.features.IpCons;
import com.esafirm.imagepicker.features.ReturnMode;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import com.xys.libzxing.zxing.activity.CaptureActivity;

import static android.content.Context.AUDIO_SERVICE;

public class JNIUtils {
    public static MainActivity mMainActivity;
    public static GLView mMainView;
    //gl thread ok
    public static void OpenURL(String url){
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
        mMainActivity.startActivity(browserIntent);
    }
    //gl thread not ok,must invoke in ui thread
    public static void ShowSoftKeyboard(String content){
        mMainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AliceInputManager.ShowSoftKeyboard();
            }
        });
    }
    public static void HideSoftKeyboard(){

    }
    //gl thread ok
    public static void SetOrientation(int orientation) {
        mMainActivity.setRequestedOrientation(orientation);
    }
    //gl thread ok
    public static void InstallApk(String apkPath){
        Intent install = new Intent(Intent.ACTION_VIEW);
        install.setDataAndType(Uri.fromFile(new File(apkPath)), "application/vnd.android.package-archive");
        install.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        mMainActivity.startActivity(install);
    }
    //gl thread ok
    public static void ScanQRCode(String string){
        if (ContextCompat.checkSelfPermission(mMainActivity, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED){ //表示未授权时
            //进行授权
            ActivityCompat.requestPermissions(mMainActivity,new String[]{Manifest.permission.CAMERA},1);
        }else{
            mMainActivity.GetQRCodeInfo();
            Intent openCameraIntent = new Intent(mMainActivity, CaptureActivity.class);
            mMainActivity.startActivityForResult(openCameraIntent, 0);
        }
    }
    //
    public static void ScheduleNotification(int id,String title,String body,int delay_time_in_second){
        Log.i("Alice","ScheduleNotification "+id+" "+title+" "+body+" "+delay_time_in_second);
    }
    public static boolean IsSilentMode(){
        AudioManager audioManager=(AudioManager) mMainActivity.getSystemService(AUDIO_SERVICE);
        return audioManager.getRingerMode() !=AudioManager.RINGER_MODE_NORMAL;
    }
    public static void CancelNotification(int id){

    }
    public static void CancelAllNotification(){

    }
    public static void GetImageFromAlbum(){
        Log.i("Alice","get image from album");
        mMainActivity.GetImageFromAlbum();
        mMainActivity.startActivityForResult(ImagePicker.create(mMainActivity)
                .single()
                .returnMode(ReturnMode.ALL)
                .getIntent(mMainActivity), IpCons.RC_IMAGE_PICKER);
    }
    public static void GetImageDataFromAlbum(){
        Log.i("Alice","get image data from album");
        mMainActivity.GetImageDataFromAlbum();
        mMainActivity.startActivityForResult(ImagePicker.create(mMainActivity)
                .single()
                .returnMode(ReturnMode.ALL)
                .getIntent(mMainActivity), IpCons.RC_IMAGE_PICKER);
    }
    //google admob part
    public static void InitGoogleAdmob(String applicationID){
    }
    public static void SetTestDevice(String deviceToken){
    }
    public static void LoadRewardVideo(String unit_id){
    }
    public static void ShowRewardVideo(){
    }
    public static void LoadBanner(String unit_id){
    }
    public static void LoadInterstitial(String unit_id){
    }
    public static void ShowInterstitial(){
    }
    public static String getCpuName(){
        String str1 = "/proc/cpuinfo";
        String str2 = "";
        try {
            FileReader fr = new FileReader(str1);
            BufferedReader localBufferedReader = new BufferedReader(fr);
            while ((str2=localBufferedReader.readLine()) != null) {
                if (str2.contains("Hardware")) {
                    return str2.split(":")[1];
                }
            }
            localBufferedReader.close();
        } catch (IOException e) {
        }
        return "Unkown";
    }
    public static int getTotalMemory() {
        String str1 = "/proc/meminfo";
        String str2="";
        try {
            FileReader fr = new FileReader(str1);
            BufferedReader localBufferedReader = new BufferedReader(fr, 8192);
            while ((str2 = localBufferedReader.readLine()) != null) {
                if (str2.contains("MemTotal")) {
                    String memory_str=str2.split(":")[1].replace("kB","").replaceAll(" ","");
                    double memory_in_mb=Double.parseDouble(memory_str)/1024.0;
                    return new Double(Math.floor(memory_in_mb)).intValue();
                }
            }
        } catch (IOException e) {
        }
        return 0;
    }
    public static void InitDeviceInfo(){
        DisplayMetrics dm=new DisplayMetrics();
        mMainActivity.getWindowManager().getDefaultDisplay().getMetrics(dm);
        double x = Math.pow(dm.widthPixels/dm.xdpi,2);
        double y = Math.pow(dm.heightPixels/dm.ydpi,2);
        double screenInches = Math.sqrt(x+y);
        String cpu_info=getCpuName()+"(";
        if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.LOLLIPOP){
            try {
                for(int i=0;i<Build.SUPPORTED_ABIS.length;++i){
                    if(i+1==Build.SUPPORTED_ABIS.length){
                        cpu_info += Build.SUPPORTED_ABIS[i]+")";
                    }else{
                        cpu_info += Build.SUPPORTED_ABIS[i]+",";
                    }
                }
            }catch (Exception e){
            }
        }else{
            cpu_info+=Build.CPU_ABI;
            if (Build.CPU_ABI2.length()>0){
                cpu_info+=","+Build.CPU_ABI2+")";
            }else{
                cpu_info+=")";
            }
        }
    }
}
