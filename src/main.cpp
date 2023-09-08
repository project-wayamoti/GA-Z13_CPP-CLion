// �ȉ��̍s���Ȃ��ƃR���p�C�����ʂ�܂���
#pragma comment(linker, "/subsystem:windows")
#include "DxLib.h"
#include "utils.h"
#include "vector.h"

//## ���ɉ����Đݒ�
#define WIDTH 1280
#define HEIGHT 720
#define SPEED_DEFAULT 8 // �ړ����x 60Hz=8, 300Hz=1
#define SPEED_WIRE 4.0f // ���C���[�̈ړ����x 60Hz=4.0f, 300Hz=0.5f
using namespace std;

//## �\���̐e���錾
void calc();     // �v�Z�֐�
void keyInput(); // �L�[���͊Ǘ�

//## �^�錾
// �F�̐ݒ�
const unsigned int colorYellow = GetColor(255, 255, 0);   // �A�[���{�́i���F�j
const unsigned int colorWhite  = GetColor(255, 255, 255); // ���C���[�i���j
const unsigned int colorRed    = GetColor(255, 0, 0);     // ���΁i�ԁj

// ���̑��̕ϐ�
int         intGoalCount            = 0;      // �S�[���J�E���g
float       floatStaticWireLength   = 100.0f; // ���C���[���i�[
float       floatWireLength         = 0.0f;   // ���C���[�̒����i�[
const int   intArmHeight            = 30;     // �A�[���{�̂̍���
const int   intItemSize             = 50;     // �A�C�e���T�C�Y
const float floatGravityAttenuation = 0.99f;  // �d�̓x�N�^�[������
const float floatWireMaxLength      = HEIGHT; // ���C���[�ő咷

// Vector2�\���̂̐錾
Vector2 vectorArmPos    = Vector2(WIDTH / 2, 20); // �A�[���{�̂̍��W
Vector2 vectorGravity   = Vector2(0, 0.2f);       // �d�͂̃x�N�g��
Vector2 vectorItemPos   = Vector2(0, 0);          // �A�C�e���̍��W
Vector2 vectorMagnetPos = Vector2(WIDTH / 2, 50); // ���΂̍��W

// Vector2 �^���E���΁E���W���i�[
Vector2 vectorPendulumPos    = Vector2(0, 0); // �U��q�i�}�O�l�b�g�j�̍��W
Vector2 vectorPendulumMotion = Vector2(0, 0); // �U��q�i�}�O�l�b�g�j�̉^���x�N�g��
Vector2 vectorWireRelative   = Vector2(0, 0); // ���C���[�̑��΃x�N�g��

// �v���O�����̍ŏ���WinMain�Ŏn�߂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //##### �����ݒ� #####//
    ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ݒ�
    SetGraphMode(WIDTH, HEIGHT, 32); // ��ʃT�C�Y�ݒ�
    SetWaitVSyncFlag(TRUE); // ����������L���ɂ���

    // �A�C�e���̍��W�������_���ɐݒ肷��
    vectorItemPos.x = GetRand(WIDTH - intItemSize);  // 0 ? (WIDTH - intItemSize) �͈̗̔͂������擾
    vectorItemPos.y = GetRand(HEIGHT - intItemSize); // 0 ? (HEIGHT - intItemSize) �͈̗̔͂������擾

    // DX���C�u��������������
    if (DxLib_Init() == -1) {
        return -1; // �G���[���N�����璼���ɏI��
    }

    //##### ���C�����[�v�i�`�揈���j #####//
    while (ProcessMessage() == 0) {
        ClearDrawScreen(); // ��ʂ̍X�V

        // �S�[���񐔂�5�񖢖��Ȃ�Continue
        if (intGoalCount < 5) {
            // �����ꂩ�̃L�[���͂��������ꍇ
            keyInput();

            // �^���x�N�^�����֐�
            calc ();

            // �A�[��
            DrawBox(
                    vectorArmPos.x, vectorArmPos.y,
                    WIDTH, vectorArmPos.y + intArmHeight,
                    colorYellow,
                    TRUE
            );
            // ���C���[
            DrawLine(
                    vectorArmPos.x, vectorArmPos.y + 30,
                    vectorMagnetPos.x, vectorMagnetPos.y,
                    colorWhite
            );
            // ����
            DrawCircle(
                    vectorMagnetPos.x, vectorMagnetPos.y,
                    10,
                    colorRed,
                    TRUE
            );
            // �A�C�e��
            DrawBox(
                    vectorItemPos.x, vectorItemPos.y,
                    vectorItemPos.x + intItemSize, vectorItemPos.y + intItemSize,
                    colorWhite,
                    TRUE
            );

            // �S�[���J�E���g��`��
            printfDx ("�S�[���J�E���g = %d\n", intGoalCount);
        }
        // �S�[���񐔂�5��ȏ�Ȃ�Q�[���N���A��`�悵��Break
        else {
            DrawString(WIDTH / 2, HEIGHT / 2, "�Q�[���N���A", colorWhite);
            DrawString(WIDTH / 2, HEIGHT / 2 + 20, "Enter�L�[�ŏI��", colorWhite);

            // �G���^�[�L�[�������ꂽ��Break
            if (CheckHitKey(KEY_INPUT_RETURN)) {
                return 0;
            }
        }

        // ��ʂ̍X�V�i�K�{�j
        ScreenFlip(); // ��ʂ𔽓]�����鏈��
        clsDx();      // �f�o�b�O���O��ʂ��N���A���鏈��
    }

    DxLib_End(); // DX Library�̎g�p���I�����鏈��
    return 0;    // �\�t�g�𐳏�I��
}

void calc() {
    // �^���x�N�g���ɏd�͂����Z
    vectorPendulumMotion = {
        vectorPendulumMotion.x + vectorGravity.x, // �^���x�N�g��X�ɏd�̓x�N�g��X�����Z
        vectorPendulumMotion.y + vectorGravity.y  // �^���x�N�g��Y�ɏd�̓x�N�g��Y�����Z
    };

    // �U��q�̍��W�v�Z
    vectorPendulumPos = {
        vectorMagnetPos.x + vectorPendulumMotion.x, // ���΂̍��WX�ɉ^���x�N�g��X�����Z
        vectorMagnetPos.y + vectorPendulumMotion.y  // ���΂̍��WY�ɉ^���x�N�g��Y�����Z
    };

    // ���C���[�̑��΃x�N�g�����v�Z
    vectorWireRelative = {
        vectorPendulumPos.x - vectorArmPos.x, // �U��q�̍��WX����A�[���{�̂̍��WX������
        vectorPendulumPos.y - vectorArmPos.y  // �U��q�̍��WY����A�[���{�̂̍��WY������
    };

    // ���C���[�̎n�_����ړ��n�_�܂ł̋����𑊑΃x�N�g��(sqrt)�ɂ���
    floatWireLength = sqrt((vectorWireRelative.x * vectorWireRelative.x) + (vectorWireRelative.y * vectorWireRelative.y));

    // ���C���[�̒������v�Z���čő咷�𒴂����ꍇ
    if (floatStaticWireLength < floatWireLength) {
        // ���C���[�̒����iRelative�j�����C���[�̒����Ŋ����Đ��K��
        Vector2 normTmp = {
            vectorWireRelative.x / floatWireLength, // ���C���[�̒���X�����C���[�̒����Ŋ���
            vectorWireRelative.y / floatWireLength  // ���C���[�̒���Y�����C���[�̒����Ŋ���
        };
        // ���K�������x�N�g���Ƀ��C���[�̒�����������
        Vector2 scaleTmp = {
            normTmp.x * floatStaticWireLength, // ���K�������x�N�g��X�Ƀ��C���[�̒�����������
            normTmp.y * floatStaticWireLength  // ���K�������x�N�g��Y�Ƀ��C���[�̒�����������
        };
        // ���C���[���͂��ʒu�ɖ߂�
        vectorPendulumPos = {
            vectorArmPos.x + scaleTmp.x, // �A�[���{�̂̍��WX�Ƀ��C���[�̒���X�����Z
            vectorArmPos.y + scaleTmp.y  // �A�[���{�̂̍��WY�Ƀ��C���[�̒���Y�����Z
        };

        // �ʒu��߂�����^���x�N�^�ɕ␳��������
        vectorPendulumMotion = {
            vectorPendulumPos.x - vectorMagnetPos.x, // �U��q�̍��WX���玥�΂̍��WX������
            vectorPendulumPos.y - vectorMagnetPos.y  // �U��q�̍��WY���玥�΂̍��WY������
        };

        // �^���x�N�g��������������
        vectorPendulumMotion = {
            vectorPendulumMotion.x *= floatGravityAttenuation, // �^���x�N�g��X�ɏd�̓x�N�g��X�����Z
            vectorPendulumMotion.y *= floatGravityAttenuation  // �^���x�N�g��Y�ɏd�̓x�N�g��Y�����Z
        };
    }

    // �}�O�l�b�g�̍��W���X�V
    vectorMagnetPos = vectorPendulumPos;

    // �A�C�e���̕`����W���Ƀ}�O�l�b�g������ꍇ
    if (vectorMagnetPos.x >= vectorItemPos.x && vectorMagnetPos.x <= vectorItemPos.x + intItemSize &&
        vectorMagnetPos.y >= vectorItemPos.y && vectorMagnetPos.y <= vectorItemPos.y + intItemSize) {
        // �A�C�e���̍��W�������_���ɐݒ肷��
        vectorItemPos.x = GetRand(WIDTH - intItemSize);  // 0 ? (WIDTH - intItemSize) �͈̗̔͂������擾
        vectorItemPos.y = GetRand(HEIGHT - intItemSize); // 0 ? (HEIGHT - intItemSize) �͈̗̔͂������擾

        // �S�[���J�E���g�𑝂₷
        intGoalCount++;
    }
}

// �L�[���͊Ǘ�
void keyInput() {
    // �L�[���͂̎擾
    char key[256];                         // �L�[���͂��i�[����z��
    GetHitKeyStateAll(key); // �S�ẴL�[�̓��͏�Ԃ𓾂�

    // �L�[���͂��������ꍇ
    // �\���L�[��WASD�ŋ@�\�𕪗�
    // �L�["��"�������ꂽ�ꍇ && �A�[�����WY��0���傫���ꍇ
    if (key[KEY_INPUT_UP] == 1 && vectorArmPos.y > 0) {
        vectorArmPos.y -= SPEED_DEFAULT; // �A�[���{�̂���Ɉړ�������
    }
    // �L�["��"�������ꂽ�ꍇ && �A�[�����WY����ʂ̍��� + ���C���[����菬�����ꍇ
    if (key[KEY_INPUT_DOWN] == 1 && vectorArmPos.y < (HEIGHT - floatStaticWireLength)) {
        vectorArmPos.y += SPEED_DEFAULT; // �A�[���{�̂����Ɉړ�������
    }
    // �L�["��"�������ꂽ�ꍇ && �A�[�����WX��0���傫���ꍇ
    if (key[KEY_INPUT_LEFT] == 1 && vectorArmPos.x > 0) {
        vectorArmPos.x -= SPEED_DEFAULT; // �A�[���{�̂����Ɉړ�������
    }
    // �L�["��"�������ꂽ�ꍇ && �A�[�����WX����ʂ̕���菬�����ꍇ
    if (key[KEY_INPUT_RIGHT] == 1 && vectorArmPos.x < WIDTH) {
        vectorArmPos.x += SPEED_DEFAULT; // �A�[���{�̂��E�Ɉړ�������
    }
    // �L�["W"�������ꂽ�ꍇ && ���C���[����0���傫���ꍇ
    if (key[KEY_INPUT_W] == 1 && floatStaticWireLength > 0) {
        floatStaticWireLength -= SPEED_WIRE; // �A�[���{�̂���Ɉړ�������
    }
    // �L�["S"�������ꂽ�ꍇ && ���C���[�������C���[�ő咷 - ���C���[���x��菬�����ꍇ
    if (key[KEY_INPUT_S] == 1 && floatStaticWireLength < floatWireMaxLength - SPEED_DEFAULT) {
        floatStaticWireLength += SPEED_WIRE; // �A�[���{�̂����Ɉړ�������
    }
}