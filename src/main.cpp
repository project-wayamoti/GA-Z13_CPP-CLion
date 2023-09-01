// 以下の行がないとコンパイルが通りません
#pragma comment(linker, "/subsystem:windows")
#include "DxLib.h"
#include "utils.h"
#include "vector.h"

//## 環境に応じて設定
#define WIDTH 1280
#define HEIGHT 720
#define SPEED 1 // 移動速度 60Hz=8, 300Hz=1
using namespace std;

//## 型宣言
// 色の設定
unsigned int armBody = GetColor(255, 255, 0); // アーム本体（黄色）
unsigned int wire = GetColor(255, 255, 255);  // ワイヤー（白）
unsigned int magnet = GetColor(255, 0, 0);    // 磁石（赤）

// Vector2構造体の宣言
Vector2 armPosA, armPosB, armMove;
Vector2 magnetPos, magnetPendulum, magnetMove, magnetMovePos;
Vector2 gravity;

void keyInput();

// プログラムの最初はWinMainで始める
[[maybe_unused]] int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //##### 初期設定 #####//
    ChangeWindowMode(TRUE); // ウィンドウモード設定
    SetGraphMode(WIDTH, HEIGHT, 32); // 画面サイズ設定
    SetWaitVSyncFlag(TRUE); // 垂直同期を有効にする

    // Vector2構造体の初期設定
    armPosA = { WIDTH / 2, 20 };           // アーム本体の座標
    armPosB = { WIDTH, 50 };               // アーム本体の座標
    armMove = { SPEED, SPEED };            // アームの移動量
    magnetPos = { WIDTH / 2, HEIGHT / 2 }; // 磁石の座標
    magnetPendulum = { 0, 0 };             // マグネットの遠心力
    magnetMove = { SPEED, SPEED };         // マグネットの移動量

    // DXライブラリ初期化処理
    if (DxLib_Init() == -1) {
        return -1; // エラーが起きたら直ちに終了
    }

    //##### メインループ（描画処理） #####//
    while (ProcessMessage() == 0) {
        ClearDrawScreen(); // 画面の更新

        // ふりこの運動ベクタに重力を加算
        magnetPendulum = addVector(magnetPendulum, gravity);

        // ふりこの移動地点を計算する
        magnetMovePos = addVector(magnetPos, magnetPendulum);

        // ワイヤーの視点から移動地点までの相対ベクタを取得してその距離を測る
        float wireLength = getDistance(magnetMovePos, armPosA);

        // ふりこの移動地点をワイヤーが届く位置に補正する
        // 正規化する 始点（アーム本体側のワイヤ起点）→移動地点（磁石の遠心力）のベクトルを正規化する
        // あとで書く



        // アーム本体の描画
        DrawBox(armPosB.x, armPosB.y, armPosA.x, armPosA.y, armBody, TRUE);

        // ワイヤーの描画 +30はアーム本体の高さ
        DrawLine(armPosA.x, armPosA.y + 30, magnetPos.x, magnetPos.y, wire);

        // 磁石の描画
        DrawCircle(magnetPos.x, magnetPos.y, 10, magnet, TRUE);

        // いずれかのキー入力があった場合
        keyInput();

        // 画面の更新（必須）
        ScreenFlip(); // 画面を反転させる処理
        clsDx();      // デバッグログ画面をクリアする処理
    }

    DxLib_End(); // DX Libraryの使用を終了する処理
    return 0;    // ソフトを正常終了
}

// キー入力管理
void keyInput() {
    // キー入力の取得
    char key[256];
    GetHitKeyStateAll(key);

    // キー入力があった場合
    if (key[KEY_INPUT_UP] == 1 || key[KEY_INPUT_W] == 1) {
        // アーム本体を上に移動させる
        armPosA.y -= SPEED;
        armPosB.y -= SPEED;
    }
    if (key[KEY_INPUT_DOWN] == 1 || key[KEY_INPUT_S] == 1) {
        // アーム本体を下に移動させる
        armPosA.y += SPEED;
        armPosB.y += SPEED;
    }
    if (key[KEY_INPUT_LEFT] == 1 || key[KEY_INPUT_A] == 1) {
        // アーム本体を左に移動させる
        armPosA.x -= SPEED;
        magnetPos.x -= SPEED;
    }
    if (key[KEY_INPUT_RIGHT] == 1 || key[KEY_INPUT_D] == 1) {
        // アーム本体を右に移動させる
        armPosA.x += SPEED;
        magnetPos.x += SPEED;
    }
}