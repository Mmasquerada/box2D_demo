package com.bilibili.esleylance.myapplication;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.ImageView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

/**
 * @author: masque
 * @date: 2022/1/12 12:12
 * @email: wangjixiang@bilibili.com
 */
public class BoxActivity extends AppCompatActivity implements SensorEventListener {
    private CollisionView collisionView;
    private SensorManager sensorManager;
    private Sensor defaultSensor;
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_box);


        initView();

        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        defaultSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
    }

    public static int dip2px(Context context, float dpValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dpValue * scale + 0.5f);
    }

    private void initView() {
        collisionView = findViewById(R.id.collision_view);

        FrameLayout.LayoutParams layoutParams;

        for (int i = 0; i < 10; i++) {
            ImageView imageView = new ImageView(this);
            if ((i & 0x1) == 0) {
                layoutParams = new FrameLayout.LayoutParams(dip2px(this, 50),
                        dip2px(this, 50));
                layoutParams.gravity = Gravity.TOP;
                imageView.setImageResource(R.drawable.bird_circle);
                imageView.setTag(1);
            } else {
                layoutParams = new FrameLayout.LayoutParams(dip2px(this, 58),
                        dip2px(this, 36));
                layoutParams.gravity = Gravity.CENTER;
                imageView.setImageResource(R.drawable.bird_rec);
                imageView.setTag(2);
            }
            layoutParams.setMarginStart(i * 2);

            collisionView.addView(imageView, layoutParams);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        sensorManager.registerListener(this, defaultSensor, SensorManager.SENSOR_DELAY_UI);
    }

    @Override
    protected void onPause() {
        super.onPause();
        sensorManager.unregisterListener(this);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float x = event.values[0];
            float y = event.values[1];
            collisionView.onSensorChanged(-x, y);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

}
