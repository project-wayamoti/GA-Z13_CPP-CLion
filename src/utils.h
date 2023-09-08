//
// Created by waya on 2023/07/14.
//

#ifndef UTILS_CLASS
#define UTILS_CLASS

#pragma once
#include <cmath>
#include <iostream>
#include <random>
#include "vector.h"
using namespace std;

// ##### ランダムな値を返却する関数 #####
// 参考文献 : http://vivi.dyndns.org/tech/cpp/random.html
// 引数     : min, max
// 戻り値   : min以上max以下のランダムな値
// 注意     : random関数を使用するため、randomをインクルードする必要がある
int random(int min, int max) {
    random_device rnd;     // 非決定的な乱数生成器を生成
    mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    uniform_int_distribution<> rand100(min, max); // 範囲指定の乱数
    // ランダムな値を返却する
    return rand100(mt);
}

// ##### ベクトルの加算 #####
// 参考文献 : なし
// 引数     : V1, V2
// 戻り値   : 加算したベクトル
// 注意     : なし
Vector2 addVector(const Vector2& V1, const Vector2& V2) {
    return {V1.x + V2.x, V1.y + V2.y};
};

// ##### ベクトルの減算 #####
// 参考文献 : なし
// 引数     : V1, V2
// 戻り値   : 減算したベクトル
// 注意     : なし
Vector2 subVector(const Vector2& V1, const Vector2& V2) {
    return {V1.x - V2.x, V1.y - V2.y};
};

// ##### ベクトルの乗算 #####
// 参考文献 : なし
// 引数     : V1, V2
// 戻り値   : 乗算したベクトル
// 注意     : なし
Vector2 mulVector(const Vector2& V1, const Vector2& V2) {
    return {V1.x * V2.x, V1.y * V2.y};
};

// ##### 外積計算関数 #####
// 参考文献 : ゲームアルゴリズム Z07 7ページ目資料
// 引数     : x1, y1, x2, y2
// 戻り値   : 外積
// 注意     : なし
float crossProduct(const Vector2& V1, const Vector2& V2) {
    return V1.x * V2.y - V1.y * V2.x;
}

// ##### 内積計算関数 #####
// 参考文献 : ゲームアルゴリズム Z07 12ページ目資料
// 引数     : Vector2 V1, Vector2 V2
// 戻り値   : 内積
// 注意     : なし
float dotProduct(const Vector2& V1, const Vector2& V2) {
    return V1.x * V2.x + V1.y * V2.y;
}

// ##### 相対ベクター計算関数 #####
// 参考文献 : ゲームアルゴリズム Z07 7ページ目資料
// 引数     : x1, y1, x2, y2
// 戻り値   : 相対ベクター
// 注意     : なし
Vector2 relative(const Vector2& V1, const Vector2& V2) {
    // 2つの円の中心座標の差分を計算（相対ベクトル）
    // 目的の地点 - 現在の地点
    return {V1.x - V1.y, V2.x - V2.y};
};

// ##### 2つの円の中心座標(Vector)の距離を計算する関数 #####
// 参考文献 : https://www.geisya.or.jp/~mwm48961/math/distance1j.htm
// 引数     : Vector2 V1, Vector2 V2
// 戻り値   : 2つの円の中心座標の距離
// 注意     : sqrt関数を使用するため、math.h or cmathをインクルードする必要がある
float getVectorDistance(const Vector2& V1, const Vector2& V2) {
    // 2つの円の中心座標の差分を計算（相対ベクトル）
    // 目的の地点 - 現在の地点
    Vector2 diff = subVector(V1, V2);
    // 2つの円の中心座標の距離を計算
    return sqrtf(powf(diff.x, 2) + powf(diff.y, 2));
};

// ##### 相対ベクターの長さを計算する関数 #####
// 参考文献 : なし
// 引数     : Vector2 V1
// 戻り値   : 相対ベクターの長さ
// 注意     : sqrt関数を使用するため、math.h or cmathをインクルードする必要がある
float getVectorLength(const Vector2& V1) {
    // 相対ベクトルの計算
    Vector2 relativeVector = mulVector(V1, V1);
    // 相対ベクトルの長さを計算
    return sqrtf(relativeVector.x + relativeVector.y);
};

// ##### 相対ベクター正規化 #####
// 参考文献 : ゲームアルゴリズム Z07 7ページ目資料
// 引数     : V1
// 戻り値   : 相対ベクター
// 注意     : なし
Vector2 normalized(const Vector2& V1) {
    // 正規化した相対ベクトルを返却する
    //return {V1.x / getVectorLength(V1), V1.y / getVectorLength(V1)};
    return {
        V1.x / sqrt((V1.x * V1.x) + (V1.y * V1.y)),
        V1.y / sqrt((V1.x * V1.x) + (V1.y * V1.y))
    };
};

// ##### 内積と外積を計算して点と点の間だったらtrueを返す #####
// 参考文献 : なし
// 引数     : Vector2 V1, Vector2 V2, Vector2 V3
// 戻り値   : 点と点の間だったらtrue
// 注意     : なし
bool pointBetween(const Vector2& V1, const Vector2& V2, const Vector2& V3) {
    // 点と点の間かどうかを判定する
    // 2つの円の中心座標の差分を計算（相対ベクトル）
    // 目的の地点 - 現在の地点
    Vector2 V4 = subVector(V2, V1);
    Vector2 V5 = subVector(V3, V1);
    // 点と点の間の幅前後100に侵入してきたらtrueを返す
    if (dotProduct (V4, V5) >= -10 || crossProduct (V4, V5) <= 10 && dotProduct (V4, V5) >= 0) {
        return true;
    }
    // 内積と外積を計算して点と点の間だったらtrueを返す
    else return false;
}

// 判定処理を共通化
Vector2 findDistance(const Vector2& V1, const Vector2& V2) {
    // 相対ベクトルを正規化
    Vector2 relativeVector = {
        (V1.x - V2.x) / getVectorDistance (V1, V2),
        (V1.y - V2.y) / getVectorDistance (V1, V2)
    };

    // 中央物体（球体）の進行方向ベクトルを返却
    return { relativeVector.y, -relativeVector.x };
};

// ##### 最近傍点を探す #####
// 参考文献 : ゲームアルゴリズム Z10 7ページ目資料
// 引数     : V1 目的の地点, V2 現在の地点
// 戻り値   : 最近傍点(Vector2)
// 注意     : なし
Vector2 dotNearestNeighborSearch(const Vector2& V1, const Vector2& V2) {
    // 内積を計算する
    float dot = dotProduct (V1, V2);
    Vector2 V3 = normalized (V1);
    // 最近傍点を計算する
    Vector2 nearestNeighbor = {V1.x + (V3.x * dot), V1.y + (V3.y * dot)};
    // 内積を返却する
    return nearestNeighbor;
};

// ##### 操作オブジェクト移動処理 #####
// 参考文献 : なし
// 引数     : pPos, vec2, p
// 戻り値   : なし
// 注意     : pには1か2を入れる必要がある。1はpPos1、2はpPos2を操作する
void moveCross(Vector2 *pVec2, const Vector2& vec2, int p) {
    if (p == 1) {
        // 十字キーでpPos2を移動させる
        if (CheckHitKey(KEY_INPUT_UP)) {
            pVec2->y -= vec2.y;
        }
        if (CheckHitKey(KEY_INPUT_DOWN)) {
            pVec2->y += vec2.y;
        }
        if (CheckHitKey(KEY_INPUT_LEFT)) {
            pVec2->x -= vec2.x;
        }
        if (CheckHitKey(KEY_INPUT_RIGHT)) {
            pVec2->x += vec2.x;
        }
    }
    else if (p == 2) {
        // WASDキーでpPos1を移動させる
        if (CheckHitKey(KEY_INPUT_W)) {
            pVec2->y -= vec2.y;
        }
        if (CheckHitKey(KEY_INPUT_S)) {
            pVec2->y += vec2.y;
        }
        if (CheckHitKey(KEY_INPUT_A)) {
            pVec2->x -= vec2.x;
        }
        if (CheckHitKey(KEY_INPUT_D)) {
            pVec2->x += vec2.x;
        }
    }
}

#endif //UTILS_H
