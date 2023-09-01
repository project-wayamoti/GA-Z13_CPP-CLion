// �ȉ��̍s���Ȃ��ƃR���p�C�����ʂ�܂���
#pragma comment(linker, "/subsystem:windows")
#include "DxLib.h"
#include "utils.h"
#include "vector.h"

//## ���ɉ����Đݒ�
#define WIDTH 1280
#define HEIGHT 720
#define SPEED 1 // �ړ����x 60Hz=8, 300Hz=1
using namespace std;

//## �^�錾
// �F�̐ݒ�
unsigned int armBody = GetColor(255, 255, 0); // �A�[���{�́i���F�j
unsigned int wire = GetColor(0, 0, 0);        // ���C���[�i���j
unsigned int magnet = GetColor(255, 0, 0);    // ���΁i�ԁj

// Vector2�\���̂̐錾
Vector2 armPosA, armPosB, armMove;             // �A�[���{��
Vector2 magnetPos, magnetPendulum, magnetMove; // ����

void keyInput();

// �v���O�����̍ŏ���WinMain�Ŏn�߂�
[[maybe_unused]] int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //##### �����ݒ� #####//
    ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ݒ�
    SetGraphMode(WIDTH, HEIGHT, 32); // ��ʃT�C�Y�ݒ�
    SetWaitVSyncFlag(TRUE); // ����������L���ɂ���

    // Vector2�\���̂̏����ݒ�
    armPosA = { WIDTH / 2, 20 };   // �A�[���{�̂̍��W
    armPosB = { WIDTH, 50 };   // �A�[���{�̂̍��W
    armMove = { SPEED, SPEED };            // �A�[���̈ړ���
    magnetPos = { WIDTH / 2, HEIGHT / 2 }; // ���΂̍��W
    magnetPendulum = { 0, 0 };             // �}�O�l�b�g�̉��S��
    magnetMove = { SPEED, SPEED };         // �}�O�l�b�g�̈ړ���

    // DX���C�u��������������
    if (DxLib_Init() == -1) {
        return -1; // �G���[���N�����璼���ɏI��
    }

    //##### ���C�����[�v�i�`�揈���j #####//
    while (ProcessMessage() == 0) {
        ClearDrawScreen(); // ��ʂ̍X�V

        // �A�[���{�̂̕`��
        DrawBox(armPosB.x, armPosB.y, armPosA.x, armPosA.y, armBody, TRUE);

        // �����ꂩ�̃L�[���͂��������ꍇ
        keyInput();

        // ��ʂ̍X�V�i�K�{�j
        ScreenFlip(); // ��ʂ𔽓]�����鏈��
        clsDx();      // �f�o�b�O���O��ʂ��N���A���鏈��
    }

    DxLib_End(); // DX Library�̎g�p���I�����鏈��
    return 0;    // �\�t�g�𐳏�I��
}

// �L�[���͊Ǘ�
void keyInput() {
    // �L�[���͂̎擾
    char key[256];
    GetHitKeyStateAll(key);

    // �L�[���͂��������ꍇ
    if (key[KEY_INPUT_UP] == 1 || key[KEY_INPUT_W] == 1) {
        // �A�[���{�̂���Ɉړ�������
        armPosA.y -= SPEED;
        armPosB.y -= SPEED;
    }
    if (key[KEY_INPUT_DOWN] == 1 || key[KEY_INPUT_S] == 1) {
        // �A�[���{�̂����Ɉړ�������
        armPosA.y += SPEED;
        armPosB.y += SPEED;
    }
    if (key[KEY_INPUT_LEFT] == 1 || key[KEY_INPUT_A] == 1) {
        // �A�[���{�̂����Ɉړ�������
        armPosA.x -= SPEED;
    }
    if (key[KEY_INPUT_RIGHT] == 1 || key[KEY_INPUT_D] == 1) {
        // �A�[���{�̂��E�Ɉړ�������
        armPosA.x += SPEED;
    }
}