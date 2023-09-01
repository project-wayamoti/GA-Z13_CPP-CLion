//
// Created by waya on 2023/06/10.
//
#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include <cmath>
#include <iostream>
using namespace std;

// Vector2構造体
// 【できること】
// ・Vector2構造体のインスタンスを作成
// ・Vector2構造体のインスタンスのx, yの値を設定
// ・Vector2構造体のインスタンスのx, yの値を取得
// ・2つの円の中心座標の距離を計算
class Vector2 {
public:
    // x, yの値を保持する変数
    float x, y;
    float size;

    // コンストラクタ
    Vector2(float x = 0, float y = 0, float size = 0) : x(x), y(y), size(size) {
        cout << "calling constructor." << endl;
    }
    ~Vector2() {
        cout << "calling destructor." << endl;
    }

    // x, yの値を設定する関数
    inline float X() const {
        return x;
    }
    inline float X(float x) {
        return this->x = x;
    }
    inline float Y() const {
        return y;
    }
    inline float Y(float y) {
        return this->y = y;
    }
    inline float Size() const {
        return size;
    }
    inline float Size(float size) {
        return this->size = size;
    }
    // Q. これはなんですか？
    // A. これは、インライン関数と呼ばれるものです。
    //    この関数は、関数の中身を呼び出し元に展開してくれる関数です。
    //    つまり、この関数を呼び出すと、
    //    「return x;」と「return this->x = x;」が呼び出し元に展開されます。
    //    この関数は、関数の呼び出しに伴うオーバーヘッドを削減するために使用されます。
    //    また、関数の呼び出しに伴うオーバーヘッドを削減することで、
    //    プログラムの実行速度を向上させることができます。
    //    なお、インライン関数は、関数の中身が短い場合に使用することが推奨されます。
    //    なぜなら、関数の中身が長い場合、
    //    インライン関数を使用することで、プログラムの実行速度が低下する可能性があるからです。
    //    なお、インライン関数は、関数の前に「inline」を付けることで、
    //    インライン関数として定義することができます。
};

#endif //VECTOR_H
