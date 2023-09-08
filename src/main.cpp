// 以下の行がないとコンパイルが通りません
#pragma comment(linker, "/subsystem:windows")
#include "DxLib.h"
#include "utils.h"
#include "vector.h"

//## 環境に応じて設定
#define WIDTH 1280
#define HEIGHT 720
#define SPEED_DEFAULT 8 // 移動速度 60Hz=8, 300Hz=1
#define SPEED_WIRE 4.0f // ワイヤーの移動速度 60Hz=4.0f, 300Hz=0.5f
using namespace std;

//## 構造体親元宣言
void calc();     // 計算関数
void keyInput(); // キー入力管理

//## 型宣言
// 色の設定
const unsigned int colorYellow = GetColor(255, 255, 0);   // アーム本体（黄色）
const unsigned int colorWhite  = GetColor(255, 255, 255); // ワイヤー（白）
const unsigned int colorRed    = GetColor(255, 0, 0);     // 磁石（赤）

// その他の変数
int         intGoalCount            = 0;      // ゴールカウント
float       floatStaticWireLength   = 100.0f; // ワイヤー長格納
float       floatWireLength         = 0.0f;   // ワイヤーの長さ格納
const int   intArmHeight            = 30;     // アーム本体の高さ
const int   intItemSize             = 50;     // アイテムサイズ
const float floatGravityAttenuation = 0.99f;  // 重力ベクター減衰率
const float floatWireMaxLength      = HEIGHT; // ワイヤー最大長

// Vector2構造体の宣言
Vector2 vectorArmPos    = Vector2(WIDTH / 2, 20); // アーム本体の座標
Vector2 vectorGravity   = Vector2(0, 0.2f);       // 重力のベクトル
Vector2 vectorItemPos   = Vector2(0, 0);          // アイテムの座標
Vector2 vectorMagnetPos = Vector2(WIDTH / 2, 50); // 磁石の座標

// Vector2 運動・相対・座標等格納
Vector2 vectorPendulumPos    = Vector2(0, 0); // 振り子（マグネット）の座標
Vector2 vectorPendulumMotion = Vector2(0, 0); // 振り子（マグネット）の運動ベクトル
Vector2 vectorWireRelative   = Vector2(0, 0); // ワイヤーの相対ベクトル

// プログラムの最初はWinMainで始める
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //##### 初期設定 #####//
    ChangeWindowMode(TRUE); // ウィンドウモード設定
    SetGraphMode(WIDTH, HEIGHT, 32); // 画面サイズ設定
    SetWaitVSyncFlag(TRUE); // 垂直同期を有効にする

    // アイテムの座標をランダムに設定する
    vectorItemPos.x = GetRand(WIDTH - intItemSize);  // 0 ? (WIDTH - intItemSize) の範囲の乱数を取得
    vectorItemPos.y = GetRand(HEIGHT - intItemSize); // 0 ? (HEIGHT - intItemSize) の範囲の乱数を取得

    // DXライブラリ初期化処理
    if (DxLib_Init() == -1) {
        return -1; // エラーが起きたら直ちに終了
    }

    //##### メインループ（描画処理） #####//
    while (ProcessMessage() == 0) {
        ClearDrawScreen(); // 画面の更新

        // ゴール回数が5回未満ならContinue
        if (intGoalCount < 5) {
            // いずれかのキー入力があった場合
            keyInput();

            // 運動ベクタ実験関数
            calc ();

            // アーム
            DrawBox(
                    vectorArmPos.x, vectorArmPos.y,
                    WIDTH, vectorArmPos.y + intArmHeight,
                    colorYellow,
                    TRUE
            );
            // ワイヤー
            DrawLine(
                    vectorArmPos.x, vectorArmPos.y + 30,
                    vectorMagnetPos.x, vectorMagnetPos.y,
                    colorWhite
            );
            // 磁石
            DrawCircle(
                    vectorMagnetPos.x, vectorMagnetPos.y,
                    10,
                    colorRed,
                    TRUE
            );
            // アイテム
            DrawBox(
                    vectorItemPos.x, vectorItemPos.y,
                    vectorItemPos.x + intItemSize, vectorItemPos.y + intItemSize,
                    colorWhite,
                    TRUE
            );

            // ゴールカウントを描画
            printfDx ("ゴールカウント = %d\n", intGoalCount);
        }
        // ゴール回数が5回以上ならゲームクリアを描画してBreak
        else {
            DrawString(WIDTH / 2, HEIGHT / 2, "ゲームクリア", colorWhite);
            DrawString(WIDTH / 2, HEIGHT / 2 + 20, "Enterキーで終了", colorWhite);

            // エンターキーが押されたらBreak
            if (CheckHitKey(KEY_INPUT_RETURN)) {
                return 0;
            }
        }

        // 画面の更新（必須）
        ScreenFlip(); // 画面を反転させる処理
        clsDx();      // デバッグログ画面をクリアする処理
    }

    DxLib_End(); // DX Libraryの使用を終了する処理
    return 0;    // ソフトを正常終了
}

void calc() {
    // 運動ベクトルに重力を加算
    vectorPendulumMotion = {
        vectorPendulumMotion.x + vectorGravity.x, // 運動ベクトルXに重力ベクトルXを加算
        vectorPendulumMotion.y + vectorGravity.y  // 運動ベクトルYに重力ベクトルYを加算
    };

    // 振り子の座標計算
    vectorPendulumPos = {
        vectorMagnetPos.x + vectorPendulumMotion.x, // 磁石の座標Xに運動ベクトルXを加算
        vectorMagnetPos.y + vectorPendulumMotion.y  // 磁石の座標Yに運動ベクトルYを加算
    };

    // ワイヤーの相対ベクトルを計算
    vectorWireRelative = {
        vectorPendulumPos.x - vectorArmPos.x, // 振り子の座標Xからアーム本体の座標Xを引く
        vectorPendulumPos.y - vectorArmPos.y  // 振り子の座標Yからアーム本体の座標Yを引く
    };

    // ワイヤーの始点から移動地点までの距離を相対ベクトル(sqrt)にする
    floatWireLength = sqrt((vectorWireRelative.x * vectorWireRelative.x) + (vectorWireRelative.y * vectorWireRelative.y));

    // ワイヤーの長さを計算して最大長を超えた場合
    if (floatStaticWireLength < floatWireLength) {
        // ワイヤーの長さ（Relative）をワイヤーの長さで割って正規化
        Vector2 normTmp = {
            vectorWireRelative.x / floatWireLength, // ワイヤーの長さXをワイヤーの長さで割る
            vectorWireRelative.y / floatWireLength  // ワイヤーの長さYをワイヤーの長さで割る
        };
        // 正規化したベクトルにワイヤーの長さをかける
        Vector2 scaleTmp = {
            normTmp.x * floatStaticWireLength, // 正規化したベクトルXにワイヤーの長さをかける
            normTmp.y * floatStaticWireLength  // 正規化したベクトルYにワイヤーの長さをかける
        };
        // ワイヤーが届く位置に戻す
        vectorPendulumPos = {
            vectorArmPos.x + scaleTmp.x, // アーム本体の座標Xにワイヤーの長さXを加算
            vectorArmPos.y + scaleTmp.y  // アーム本体の座標Yにワイヤーの長さYを加算
        };

        // 位置を戻したら運動ベクタに補正をかける
        vectorPendulumMotion = {
            vectorPendulumPos.x - vectorMagnetPos.x, // 振り子の座標Xから磁石の座標Xを引く
            vectorPendulumPos.y - vectorMagnetPos.y  // 振り子の座標Yから磁石の座標Yを引く
        };

        // 運動ベクトルを減衰させる
        vectorPendulumMotion = {
            vectorPendulumMotion.x *= floatGravityAttenuation, // 運動ベクトルXに重力ベクトルXを加算
            vectorPendulumMotion.y *= floatGravityAttenuation  // 運動ベクトルYに重力ベクトルYを加算
        };
    }

    // マグネットの座標を更新
    vectorMagnetPos = vectorPendulumPos;

    // アイテムの描画座標内にマグネットがある場合
    if (vectorMagnetPos.x >= vectorItemPos.x && vectorMagnetPos.x <= vectorItemPos.x + intItemSize &&
        vectorMagnetPos.y >= vectorItemPos.y && vectorMagnetPos.y <= vectorItemPos.y + intItemSize) {
        // アイテムの座標をランダムに設定する
        vectorItemPos.x = GetRand(WIDTH - intItemSize);  // 0 ? (WIDTH - intItemSize) の範囲の乱数を取得
        vectorItemPos.y = GetRand(HEIGHT - intItemSize); // 0 ? (HEIGHT - intItemSize) の範囲の乱数を取得

        // ゴールカウントを増やす
        intGoalCount++;
    }
}

// キー入力管理
void keyInput() {
    // キー入力の取得
    char key[256];                         // キー入力を格納する配列
    GetHitKeyStateAll(key); // 全てのキーの入力状態を得る

    // キー入力があった場合
    // 十字キーとWASDで機能を分離
    // キー"↑"が押された場合 && アーム座標Yが0より大きい場合
    if (key[KEY_INPUT_UP] == 1 && vectorArmPos.y > 0) {
        vectorArmPos.y -= SPEED_DEFAULT; // アーム本体を上に移動させる
    }
    // キー"↓"が押された場合 && アーム座標Yが画面の高さ + ワイヤー長より小さい場合
    if (key[KEY_INPUT_DOWN] == 1 && vectorArmPos.y < (HEIGHT - floatStaticWireLength)) {
        vectorArmPos.y += SPEED_DEFAULT; // アーム本体を下に移動させる
    }
    // キー"←"が押された場合 && アーム座標Xが0より大きい場合
    if (key[KEY_INPUT_LEFT] == 1 && vectorArmPos.x > 0) {
        vectorArmPos.x -= SPEED_DEFAULT; // アーム本体を左に移動させる
    }
    // キー"→"が押された場合 && アーム座標Xが画面の幅より小さい場合
    if (key[KEY_INPUT_RIGHT] == 1 && vectorArmPos.x < WIDTH) {
        vectorArmPos.x += SPEED_DEFAULT; // アーム本体を右に移動させる
    }
    // キー"W"が押された場合 && ワイヤー長が0より大きい場合
    if (key[KEY_INPUT_W] == 1 && floatStaticWireLength > 0) {
        floatStaticWireLength -= SPEED_WIRE; // アーム本体を上に移動させる
    }
    // キー"S"が押された場合 && ワイヤー長がワイヤー最大長 - ワイヤー速度より小さい場合
    if (key[KEY_INPUT_S] == 1 && floatStaticWireLength < floatWireMaxLength - SPEED_DEFAULT) {
        floatStaticWireLength += SPEED_WIRE; // アーム本体を下に移動させる
    }
}