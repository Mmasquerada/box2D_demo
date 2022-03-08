package com.bilibili.esleylance.myapplication;

/**
 * @author: masque
 * @date: 2022/1/12 15:43
 * @email: wangjixiang@bilibili.com
 */
class JniInterface {
    static {
        System.loadLibrary("native-lib");
    }

    private static CollisionView.OnUpdateParamsListener mListener;

    public static void setListener(CollisionView.OnUpdateParamsListener listener) {
        mListener = listener;
    }

    public static void release() {
        mListener = null;
    }

    public static void notifyEvent(int index, float centerX, float centerY, float angle) {
        if (mListener != null) {
            mListener.update(index, centerX, centerY, angle);
        }
    }

    public static native void createModel();

    public static native void updateBounds(int width, int height, int count);

    public static native void bindBody(float x, float y, float width, float height, int index, int type);

    public static native void startWorld();

    public static native void drawView(int index);

    public static native void applyLinearImpulse(float x, float y);
}
