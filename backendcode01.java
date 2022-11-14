package com.example2.anti;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity implements ValueEventListener {
    TextView textView, textView1, textView2;
    Button btn;

    private FirebaseDatabase firebaseDatabase = FirebaseDatabase.getInstance();
    private DatabaseReference databaseReference = firebaseDatabase.getReference().child("Detected").child("6");;
    private DatabaseReference firstDatabase = databaseReference.child("Time");
    //private DatabaseReference root = db.getReference()
    private DatabaseReference secondDatabase = databaseReference.child("VehcileNo");
    private DatabaseReference thirdDatabase = databaseReference.child("VehcileStatus");
   // private DatabaseReference forthDatabase = databaseReference.child("");
    //private ArrayList<Model> list;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textView = (TextView) findViewById(R.id.textView12);
        textView1 = (TextView) findViewById(R.id.textView14);
        textView2 = (TextView) findViewById(R.id.textView16);
        btn=(Button)findViewById(R.id.button3);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                OpenActivity();
            }
        });

    }
    public void OpenActivity()
    {
        Intent intent=new Intent(this, MainActivity.class);
        startActivity(intent);
    }
 //   public void onDataChange(@NonNull DataSnapshot snapshot) {
//        Object dataSnapshot;
        /*if (snapshot.getValue(Object.class) != null) {
            Object key = snapshot.getKey();
            if (key.equals("Time")) {
                String train = snapshot.getValue(String.class);
                textView.setText(train);
            }
            if (key.equals("VehcileNo")) {
                Integer adc = snapshot.getValue(Integer.class);
                textView1.setText("" + adc);
            }
            if (key.equals("VehcileStatus")) {
                Integer efg = snapshot.getValue(Integer.class);
                textView2.setText("" + efg);

            }*/

 //       @Override
        public void onDataChange(@NonNull DataSnapshot snapshot) {
        //Object dataSnapshot;
            if (snapshot.getValue(Object.class) != null) {
                Object key = snapshot.getKey();
               // for (DataSnapshot data : snapshot.getChildren()) {
                /*Model model = dataSnapshot.getValue(Model.class);
                list.add(model);*/

             /* if (key.equals("Time")) {
                    String train = snapshot.getValue(String.class);
                    textView.setText(train);
                }*/
                    if (key.equals("Time")) {
                        //  String ex = toString(String.class)
                        String tm = snapshot.getValue(String.class);
                        textView.setText("" + tm);
                    }
                    if (key.equals("VehcileNo")) {
                        String adc = snapshot.getValue(String.class);
                        textView1.setText("" + adc);
                    }
                    if (key.equals("VehcileStatus")) {
                        String efg = snapshot.getValue(String.class);
                        textView2.setText("" + efg);
                    }
                }

            }



    @Override
    public void onCancelled (DatabaseError databaseerror)
    {


    }
    protected void onStart () {

        super.onStart();
        firstDatabase.addValueEventListener(this);
        secondDatabase.addValueEventListener(this);
        thirdDatabase.addValueEventListener(this);

    }

}
