package com.example.nov16app;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.os.Build;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


    private void notification() {
            if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.O) {
                NotificationChannel channel =
                        new NotificationChannel("n", "n", NotificationManager.IMPORTANCE_DEFAULT);

                NotificationManager manager = getSystemService(NotificationManager.class);
                manager.createNotificationChannel(channel);
            }
        NotificationCompat.Builder builder = new NotificationCompat.Builder(this,"n")
                .setContentText("Sphere Code")
                .setSmallIcon(R.drawable.ic_notification)
                .setAutoCancel(true)
                .setContentText("Data is added");
        NotificationManagerCompat managerCompat = NotificationManagerCompat.from(this);
        managerCompat.notify(999,builder.build());
    }

}
