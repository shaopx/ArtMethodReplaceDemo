package com.spx.artmethodreplacedemo;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.spx.artmethodreplacedemo.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("artmethodreplacedemo");
    }

    private ActivityMainBinding binding;
    private NativeArtMethodReplace replace;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        replace = new NativeArtMethodReplace();
        // 模拟显示一个错误结果
        updateTextView();

        binding.doReplace.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 模拟热修
                replace.hotfix();
                // 热修完成后,再次刷新页面显示
                updateTextView();
            }
        });
    }

    // 更新显示页面显示
    private void updateTextView() {
        TextView tv = binding.sampleText;
        tv.setText("100+100=" + SomeClass.f1());
    }
}