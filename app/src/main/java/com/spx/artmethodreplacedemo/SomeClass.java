package com.spx.artmethodreplacedemo;

public class SomeClass {
    // 要被热修的方法
    public final static int f1() {
        return 100;
    }
    // 模拟一个要进行热修复对老方法进行替换的新方法, 现实中多半是要通过打包到一个新的dex中下发
    public final static int f2() {
        return 200;
    }
}
