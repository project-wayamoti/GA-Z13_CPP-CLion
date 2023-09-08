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

        // コンストラクタ
        Vector2(float x = 0, float y = 0) : x(x), y(y) {
            cout << "calling constructor." << endl;
        }
        ~Vector2() {
            cout << "calling destructor." << endl;
        }

        // x, yの値を設定する関数
        [[nodiscard]]
        inline float X() const {
            return x;
        }
        inline float X(float d) {
            return this->x = d;
        }
        [[nodiscard]]
        inline float Y() const {
            return y;
        }
        inline float Y(float d) {
            return this->y = d;
        }

        // いつかどんどん増やしていくのでその準備
        [[nodiscard]]
        float Length() const {
            return sqrtf(x * x + y * y);
        }

        const Vector2 &Normalize() {
            float len = Length();
            if (len > 0.0f) {
                float invLen = 1.0f / len;
                x *= invLen;
                y *= invLen;
            }
            return *this;
        }
};

#endif //VECTOR_H
