package cn.battlefire.androidplayer;
import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import androidx.appcompat.widget.AppCompatEditText;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.inputmethod.InputMethodManager;
import android.widget.RelativeLayout;

import java.io.UnsupportedEncodingException;

import static android.view.ViewGroup.LayoutParams.WRAP_CONTENT;

/**
 * Created by Administrator on 2016/8/9.
 */
class TextEditTextView extends AppCompatEditText {
    public TextEditTextView(Context context) {
        super(context);
    }
    public TextEditTextView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }
    public TextEditTextView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }
    @Override
    public boolean onKeyPreIme(int keyCode, KeyEvent event) {
        Log.i("Alice","onKeyPreIme");
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == 1) {
            Log.i("main_activity", "键盘向下 ");
            super.onKeyPreIme(keyCode, event);
            return false;
        }
        return super.onKeyPreIme(keyCode, event);
    }
}
class GlobalLayoutListener implements ViewTreeObserver.OnGlobalLayoutListener{
    @Override
    public void onGlobalLayout() {
        Rect r = new Rect();
        //获取当前界面可视部分
        AliceInputManager.mActivity.getWindow().getDecorView().getWindowVisibleDisplayFrame(r);
        //获取屏幕的高度
        int screenHeight =  AliceInputManager.mActivity.getWindow().getDecorView().getRootView().getHeight();
        //此处就是用来获取键盘的高度的， 在键盘没有弹出的时候 此高度为0 键盘弹出的时候为一个正数
        int keyboard_height = screenHeight - r.bottom;
        Log.d("Keyboard Size", "Size: " + keyboard_height);
        if(keyboard_height==0){
            Log.i("Alice","hide keyboard");
            AliceInputManager.mEditText.getViewTreeObserver().removeOnGlobalLayoutListener(AliceInputManager.mGlobalLayoutListener);
            AliceInputManager.OnHideKeyboard();
        }else{
            Log.i("Alice","show keyboard");
        }
    }
}
public class AliceInputManager {
    public static Activity mActivity;
    public static TextEditTextView mEditText;
    public static ViewTreeObserver.OnGlobalLayoutListener mGlobalLayoutListener;
    public static int mSoftKeyboardHeight=0;
    static RelativeLayout.LayoutParams mEditTextLayOutParam = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, WRAP_CONTENT);
    public static void Init(Context context,RelativeLayout parentLayout) {
        mActivity=(Activity) context;
        mEditText = new TextEditTextView(context) ;// EditText(context);
        mEditText.setPadding(0, 0, 0, 0);
        mEditText.setBackgroundColor(Color.WHITE);
        mEditText.setFocusable(true);
        mEditText.setFocusableInTouchMode(true);
        mEditText.setMinLines(1);
        RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        mEditText.setLayoutParams(layoutParams);
        parentLayout.addView(mEditText,mEditTextLayOutParam);
        mEditText.setVisibility(View.INVISIBLE);
        mEditText.setTextColor(Color.BLACK);
        mEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (before == 0 && count == 1 && s.charAt(start) == '\n') {
                    mEditText.getText().replace(start, start + 1, ""); //remove the <enter>
                    OnHideKeyboard();
                }
            }
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
            @Override
            public void afterTextChanged(Editable s) {}
        });
    }
    public  static void OnHideKeyboard(){
        String text=mEditText.getText().toString();
        if (!text.isEmpty())
        {
            try {
                text.getBytes("UTF-8");
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            mEditText.setText("");
        }else{
        }
        mEditText.setVisibility(View.INVISIBLE);
        mEditText.clearFocus();
        InputMethodManager inputManager = (InputMethodManager)mEditText.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        inputManager.hideSoftInputFromWindow(mEditText.getWindowToken(), 0);
    }
    public static void ShowSoftKeyboard()
    {
        Log.i("Alice","Show soft keyboard");
        mGlobalLayoutListener=new GlobalLayoutListener();
        mEditText.getViewTreeObserver().addOnGlobalLayoutListener(mGlobalLayoutListener);
        mEditText.setVisibility(View.VISIBLE);
        mEditText.requestFocus();
        InputMethodManager inputManager = (InputMethodManager)mEditText.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        inputManager.showSoftInput(mEditText, 0);
    }
}
