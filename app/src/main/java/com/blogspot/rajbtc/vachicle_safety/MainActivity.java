package com.blogspot.rajbtc.vachicle_safety;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.blogspot.rajbtc.vachicle_safety.R;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    double x, y,temp,lat,lon;
    MediaPlayer mediaPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mediaPlayer=MediaPlayer.create(getApplicationContext(),R.raw.alarm);
        try{
            loadData();
        }catch (Exception e){

        }
    }

    void loadData(){
        DatabaseReference fd=FirebaseDatabase.getInstance().getReference("Data");

        fd.child("X").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                x=Double.parseDouble(snapshot.getValue(String.class));
                update();
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
        fd.child("Y").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                y=Double.parseDouble(snapshot.getValue(String.class));
                update();
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

        fd.child("Temp").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                temp=Double.parseDouble(snapshot.getValue(String.class));
                update();
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });


        fd.child("Danger").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {

                if(snapshot.getValue(String.class).toString().contains("1"))
                {
                    if(!mediaPlayer.isPlaying())
                        mediaPlayer.start();;
                }else
                    mediaPlayer.stop();
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });


        fd.child("Location").child("Lat").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                lat=Double.parseDouble(snapshot.getValue(String.class));
                update();
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });


        fd.child("Location").child("Lon").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                lon=Double.parseDouble(snapshot.getValue(String.class));
                update();
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });







    }


    void update(){
       ((TextView) findViewById(R.id.xTv)).setText("X: "+x);
       ((TextView) findViewById(R.id.yTv)).setText("Y: "+y);
       ((TextView) findViewById(R.id.latTv)).setText("Latitude: "+lat);
       ((TextView) findViewById(R.id.lonTv)).setText("longitude: "+lon);
       ((TextView) findViewById(R.id.tempTv)).setText("temperate: "+temp);

    }

    public void gotoLocation(View view) {

        String url = "https://google.com/maps/place/"+lat+","+lon;
        Intent i = new Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse(url));
        startActivity(i);
    }
}