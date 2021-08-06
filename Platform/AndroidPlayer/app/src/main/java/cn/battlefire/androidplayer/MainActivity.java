package cn.battlefire.androidplayer;
import android.app.Activity;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.net.Uri;

import com.esafirm.imagepicker.features.ImagePicker;
import com.esafirm.imagepicker.model.Image;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class MainActivity extends Activity {
    GLView mView;
    RelativeLayout mLayout;

    public class TouchInfo {
        public float x;
        public float y;
    }
    Map mFingers;
    String mExtraData;
    int mScreenWidth;
    int mScreenHeight;
    @Override
    protected void onCreate(Bundle savedInstanceState) {//ui thread
        super.onCreate(savedInstanceState);
        if(getIntent().getExtras()!=null){
            mExtraData=getIntent().getDataString();
        }
        mFingers=new HashMap();
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);//设置成为竖屏显示
        requestWindowFeature(getWindow().FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        mView = new GLView(getApplication());
        mView.mRenderer.mActivity=this;
        JNIUtils.mMainActivity=this;
        JNIUtils.mMainView=mView;
        //setContentView(mView);
        mLayout = new RelativeLayout(this);
        //创建一个LayoutParams对象
        RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        //动态添加EditText
        mLayout.addView(mView,layoutParams);
        AliceInputManager.Init(this,mLayout);
        //设置布局
        setContentView(mLayout);
        JNIUtils.InitDeviceInfo();
        Display display =    getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);
        mScreenWidth = size.x;
        mScreenHeight = size.y;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            Log.i("Alice","Init Notification");
            // Create channel to show notifications.
            String channelId  = "AliceNotification";
            String channelName = "Chat";
            NotificationManager notificationManager =
                    getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(new NotificationChannel(channelId,
                    channelName, NotificationManager.IMPORTANCE_LOW));
        }
        AliceSensorManager sensorHelper = new AliceSensorManager(this);
        sensorHelper.setOnShakeListener(new AliceSensorManager.OnShakeListener() {
            @Override
            public void onShake() {
            }
        });
        Intent i_getvalue = getIntent();
        String action = i_getvalue.getAction();
        if(Intent.ACTION_VIEW.equals(action)){
            Uri uri = i_getvalue.getData();
            if(uri != null){
                String name = uri.getQueryParameter("name");
                String age= uri.getQueryParameter("age");
                Log.i("Alice","地址："+uri.toString()+"\n姓名"+name+"\t年龄"+age);
            }else{
                Log.i("Alice","uri is null");
            }
        }else{
            Log.i("Alice","no intent");
        }
    }
    @Override
    public void onBackPressed(){
        Log.i("Alice","on back pressed");
    }
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event){
        switch(keyCode){
            case KeyEvent.KEYCODE_BACK:
                return true;
        }
        return super.onKeyDown(keyCode,event);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        int pointerIndex=event.getActionIndex();
        int pointerID=event.getPointerId(pointerIndex);
        int action = event.getActionMasked();
        //Log.i("Alice","finger count pointer index "+pointerIndex+"/"+nCount+" id : "+pointerID+" action index "+index+" action is "+action);
        switch(action) {
            case (MotionEvent.ACTION_DOWN) :
            {
                //AliceInputManager.ShowSoftKeyboard();
                pointerIndex = event.findPointerIndex(pointerID);
                TouchInfo ti=new TouchInfo();
                ti.x=event.getX(pointerIndex);
                ti.y=event.getY(pointerIndex);
                mFingers.put(pointerID,ti);
                //Log.i("Alice","Action was DOWN "+pointerID);
            }
            return true;
            case (MotionEvent.ACTION_MOVE) :
                Iterator it = mFingers.entrySet().iterator();
                while (it.hasNext())
                {
                    Map.Entry entry=(Map.Entry)it.next();
                    pointerID=Integer.parseInt(entry.getKey().toString());
                    pointerIndex = event.findPointerIndex(pointerID);
                    float x = event.getX(pointerIndex);
                    float y = event.getY(pointerIndex);
                    TouchInfo tix=(TouchInfo) mFingers.get(entry.getKey());
                    if (x!=tix.x||y!=tix.y)
                    {
                        tix.x=x;
                        tix.y=y;
                        //Log.i("Alice","Action was MOVE "+pointerID +" : "+ x+","+y);
                    }
                }
                return true;
            case (MotionEvent.ACTION_UP) :
                if(mFingers.containsKey(pointerID))
                {
                    //Log.i("Alice","Action was UP "+pointerID);
                    float x = event.getX(pointerIndex);
                    float y = event.getY(pointerIndex);
                    mFingers.remove(pointerID);
                }
                return true;
            case (MotionEvent.ACTION_CANCEL) :
                if(mFingers.containsKey(pointerID))
                {
                    //Log.i("Alice","Action was CANCEL "+pointerID);
                    float x = event.getX(pointerIndex);
                    float y = event.getY(pointerIndex);
                    mFingers.remove(pointerID);
                }
                return true;
            case (MotionEvent.ACTION_OUTSIDE) :
                //Log.i("Alice","Movement occurred outside bounds " +"of current screen element");
                return true;
            case MotionEvent.ACTION_POINTER_DOWN:
                pointerIndex = event.findPointerIndex(pointerID);
                TouchInfo tix=new TouchInfo();
                tix.x=event.getX(pointerIndex);
                tix.y=event.getY(pointerIndex);
                mFingers.put(pointerID,tix);
                //Log.i("Alice","Action was DOWN "+pointerID);
                return true;
            case MotionEvent.ACTION_POINTER_UP:
                if(mFingers.containsKey(pointerID))
                {
                    //Log.i("Alice","Action was UP "+pointerID);
                    float x = event.getX(pointerIndex);
                    float y = event.getY(pointerIndex);
                    mFingers.remove(pointerID);
                }
                return true;
            default :
                return super.onTouchEvent(event);
        }
    }

    @Override protected void onPause() {
        Iterator it = mFingers.entrySet().iterator();
        while (it.hasNext())
        {
            Map.Entry entry=(Map.Entry)it.next();
            int pointerID=Integer.parseInt(entry.getKey().toString());
            TouchInfo tix=(TouchInfo) mFingers.get(entry.getKey());
        }
        mFingers.clear();
        super.onPause();
        mView.onPause();
    }
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        if(newConfig.orientation==Configuration.ORIENTATION_PORTRAIT){
            //do what you want
        }
        else if(newConfig.orientation==Configuration.ORIENTATION_LANDSCAPE){
            //do what you want
        }
        else if (newConfig.keyboardHidden==Configuration.KEYBOARDHIDDEN_YES) {
            //do what you want
        }
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    int mResultType=0;
    public void GetQRCodeInfo(){
        mResultType=1;
    }
    public void GetImageFromAlbum(){
        mResultType=2;
    }
    public void GetImageDataFromAlbum(){
        mResultType=3;
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(mResultType==1){
            if (resultCode == RESULT_OK) {
                Bundle bundle = data.getExtras();
                String scanResult = bundle.getString("result");
            }else{
            }
        }else if(mResultType==2){
            if (ImagePicker.shouldHandle(requestCode, resultCode, data)) {
                Log.i("Alice","ImagePicker.shouldHandle");
                ArrayList<Image> images=(ArrayList<Image>) ImagePicker.getImages(data);
                if(images!=null){
                    StringBuilder stringBuffer = new StringBuilder();
                    Log.i("Alice","image count "+images.size());
                    for (int i = 0, l = images.size(); i < l; i++) {
                        stringBuffer.append(images.get(i).getPath()).append("\n");
                    }
                    Log.i("Alice",stringBuffer.toString());
                }
            }else{
            }
        }else if(mResultType==3){
            if (ImagePicker.shouldHandle(requestCode, resultCode, data)) {
                Log.i("Alice","ImagePicker.shouldHandle");
                ArrayList<Image> images=(ArrayList<Image>) ImagePicker.getImages(data);
                if(images!=null){
                    StringBuilder stringBuffer = new StringBuilder();
                    Log.i("Alice","image count "+images.size());
                    for (int i = 0, l = images.size(); i < l; i++) {
                        stringBuffer.append(images.get(i).getPath()).append("\n");
                    }
                    Log.i("Alice",stringBuffer.toString());
                }
            }else{
            }
        }
        super.onActivityResult(requestCode, resultCode, data);
    }
}

