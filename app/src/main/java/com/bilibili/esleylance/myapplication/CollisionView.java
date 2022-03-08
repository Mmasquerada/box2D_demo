package com.bilibili.esleylance.myapplication;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

/**
 * 碰撞view
 */
public class CollisionView extends FrameLayout {

    public CollisionView(@NonNull Context context) {
        this(context, null);
    }

    public CollisionView(@NonNull Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public CollisionView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        setWillNotDraw(false);

        // Used to load the 'native-lib' library on application startup.
        JniInterface.createModel();
        JniInterface.setListener((index, centerX, centerY, angel) -> {
            int childCount = getChildCount();
            if (index >= 0 && index < childCount) {
                View view = getChildAt(index);
                view.setX(centerX - (view.getWidth() >> 1));
                view.setY(centerY - (view.getHeight() >> 1));
                view.setRotation(angel);
            }
        });
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        if (changed) {
            JniInterface.updateBounds(getMeasuredWidth(), getMeasuredHeight(), getChildCount());
            //子view设置body
            int childCount = getChildCount();
            for (int i = 0; i < childCount; i++) {
                View view = getChildAt(i);
                int type = (int) view.getTag();
                JniInterface.bindBody(view.getX(), view.getY(), view.getWidth(), view.getHeight(), i, type);
            }
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        JniInterface.startWorld();

        int childCount = getChildCount();
        for (int i = 0; i < childCount; i++) {
            JniInterface.drawView(i);
        }
        invalidate();
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();

        JniInterface.release();
    }

    public void onSensorChanged(float x, float y) {
        int childCount = getChildCount();
        for (int i = 0; i < childCount; i++) {
            JniInterface.applyLinearImpulse(x, y);
        }
    }

    interface OnUpdateParamsListener {
        void update(int index, float centerX, float centerY, float angel);
    }
}
