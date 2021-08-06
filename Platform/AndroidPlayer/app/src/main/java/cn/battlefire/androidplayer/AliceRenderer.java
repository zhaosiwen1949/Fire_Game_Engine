package cn.battlefire.androidplayer;

import android.graphics.SurfaceTexture;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.view.Surface;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.opengl.GLES11Ext.GL_TEXTURE_EXTERNAL_OES;

public class AliceRenderer implements GLSurfaceView.Renderer,SurfaceTexture.OnFrameAvailableListener {//not ui thread
    public GLSurfaceView mView;
    public MainActivity mActivity;
    public void onDrawFrame(GL10 gl) {
        synchronized (this){
            AliceRuntime.Render();
        }
    }
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        AliceRuntime.OnViewportChanged(width,height);
    }
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    }
    public void onFrameAvailable(SurfaceTexture surfaceTexture){
        synchronized (this){
        }
    }
}