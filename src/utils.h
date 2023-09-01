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

// �֐��̃v���g�^�C�v�錾
int random(int min, int max);
Vector2 addVector(Vector2 V1, Vector2 V2);
Vector2 subVector(Vector2 V1, Vector2 V2);
float crossProduct(Vector2 V1, Vector2 V2);
float dotProduct(Vector2 V1, Vector2 V2);
Vector2 relative(Vector2 V1, Vector2 V2);
Vector2 normalized(Vector2 V1);
float getDistance(Vector2 V1, Vector2 V2);
bool pointBetween(Vector2 V1, Vector2 V2, Vector2 V3);
Vector2 findDistance(Vector2 V1, Vector2 V2);
Vector2 dotNearestNeighborSearch(Vector2 V1, Vector2 V2);
void moveCross(Vector2 *pVec2, Vector2 vec2, int p);

// ##### �����_���Ȓl��ԋp����֐� #####
// �Q�l���� : http://vivi.dyndns.org/tech/cpp/random.html
// ����     : min, max
// �߂�l   : min�ȏ�max�ȉ��̃����_���Ȓl
// ����     : random�֐����g�p���邽�߁Arandom���C���N���[�h����K�v������
int random(int min, int max) {
    random_device rnd;     // �񌈒�I�ȗ���������𐶐�
    mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
    uniform_int_distribution<> rand100(min, max); // �͈͎w��̗���
    // �����_���Ȓl��ԋp����
    return rand100(mt);
}

// ##### �x�N�g���̉��Z #####
// �Q�l���� : �Ȃ�
// ����     : V1, V2
// �߂�l   : ���Z�����x�N�g��
// ����     : �Ȃ�
Vector2 addVector(Vector2 V1, Vector2 V2) {
    return Vector2(V1.x + V2.x, V1.y + V2.y);
};

// ##### �x�N�g���̌��Z #####
// �Q�l���� : �Ȃ�
// ����     : V1, V2
// �߂�l   : ���Z�����x�N�g��
// ����     : �Ȃ�
Vector2 subVector(Vector2 V1, Vector2 V2) {
    return Vector2(V1.x - V2.x, V1.y - V2.y);
};

// ##### �O�όv�Z�֐� #####
// �Q�l���� : �Q�[���A���S���Y�� Z07 7�y�[�W�ڎ���
// ����     : x1, y1, x2, y2
// �߂�l   : �O��
// ����     : �Ȃ�
float crossProduct(Vector2 V1, Vector2 V2) {
    return (float)V1.x * V2.y - V1.y * V2.x;
}

// ##### ���όv�Z�֐� #####
// �Q�l���� : �Q�[���A���S���Y�� Z07 12�y�[�W�ڎ���
// ����     : Vector2 V1, Vector2 V2
// �߂�l   : ����
// ����     : �Ȃ�
float dotProduct(Vector2 V1, Vector2 V2) {
    return (float)V1.x * V2.x + V1.y * V2.y;
}

// ##### ���΃x�N�^�[�v�Z�֐� #####
// �Q�l���� : �Q�[���A���S���Y�� Z07 7�y�[�W�ڎ���
// ����     : x1, y1, x2, y2
// �߂�l   : ���΃x�N�^�[
// ����     : �Ȃ�
Vector2 relative(Vector2 V1, Vector2 V2) {
    // 2�̉~�̒��S���W�̍������v�Z�i���΃x�N�g���j
    // �ړI�̒n�_ - ���݂̒n�_
    return Vector2(V1.x - V2.x, V1.y - V2.y);
};

// ##### ���΃x�N�^�[���K�� #####
// �Q�l���� : �Q�[���A���S���Y�� Z07 7�y�[�W�ڎ���
// ����     : V1
// �߂�l   : ���΃x�N�^�[
// ����     : �Ȃ�
Vector2 normalized(Vector2 V1) {
    // ���΃x�N�g���𐳋K������
    float length = getDistance(V1, V1);
    // ���K���������΃x�N�g����ԋp����
    return Vector2(V1.x / length, V1.y / length);
};

// ##### 2�̉~�̒��S���W�̋������v�Z����֐� #####
// �Q�l���� : https://www.geisya.or.jp/~mwm48961/math/distance1j.htm
// ����     : Vector2 V1, Vector2 V2
// �߂�l   : 2�̉~�̒��S���W�̋���
// ����     : sqrt�֐����g�p���邽�߁Amath.h or cmath���C���N���[�h����K�v������
float getDistance(Vector2 V1, Vector2 V2) {
    // 2�̉~�̒��S���W�̍������v�Z�i���΃x�N�g���j
    // �ړI�̒n�_ - ���݂̒n�_
    Vector2 diff = subVector(V1, V2);
    // 2�̉~�̒��S���W�̋������v�Z
    return sqrt(pow(diff.x, 2) + pow(diff.y, 2));
}

// ##### ���ςƊO�ς��v�Z���ē_�Ɠ_�̊Ԃ�������true��Ԃ� #####
// �Q�l���� : �Ȃ�
// ����     : Vector2 V1, Vector2 V2, Vector2 V3
// �߂�l   : �_�Ɠ_�̊Ԃ�������true
// ����     : �Ȃ�
bool pointBetween(Vector2 V1, Vector2 V2, Vector2 V3) {
    // �_�Ɠ_�̊Ԃ��ǂ����𔻒肷��
    // 2�̉~�̒��S���W�̍������v�Z�i���΃x�N�g���j
    // �ړI�̒n�_ - ���݂̒n�_
    Vector2 V4 = subVector(V2, V1);
    Vector2 V5 = subVector(V3, V1);
    // �O�ς��v�Z
    float crossProduct = crossProduct (V4, V5);
    // ���ς��v�Z
    float dotProduct = dotProduct (V4, V5);
    // �_�Ɠ_�̊Ԃ̕��O��100�ɐN�����Ă�����true��Ԃ�
    if (crossProduct >= -10 || crossProduct <= 10 && dotProduct >= 0) {
        return true;
    }
    // ���ςƊO�ς��v�Z���ē_�Ɠ_�̊Ԃ�������true��Ԃ�
    else return false;
}

// ���菈�������ʉ�
Vector2 findDistance(Vector2 V1, Vector2 V2) {
    // ���΃x�N�g���𐳋K��
    Vector2 relativeVector = {
        (V1.x - V2.x) / getDistance(V1, V2),
        (V1.y - V2.y) / getDistance(V1, V2)
    };

    // return ���ɓ���ւ�������@���킩��Ȃ��̂�POWER IS POWER
    Vector2 tmp = { relativeVector.y, -relativeVector.x };

    // �������́i���́j�̐i�s�����x�N�g����ԋp
    return tmp;
};

// ##### �ŋߖT�_��T�� #####
// �Q�l���� : �Q�[���A���S���Y�� Z10 7�y�[�W�ڎ���
// ����     : V1 �ړI�̒n�_, V2 ���݂̒n�_
// �߂�l   : �ŋߖT�_(Vector2)
// ����     : �Ȃ�
Vector2 dotNearestNeighborSearch(Vector2 V1, Vector2 V2) {
    // ���ς��v�Z����
    float dot = dotProduct (V1, V2);
    Vector2 V3 = normalized (V1);
    // �ŋߖT�_���v�Z����
    Vector2 nearestNeighbor = {V1.x + (V3.x * dot), V1.y + (V3.y * dot)};
    // ���ς�ԋp����
    return nearestNeighbor;
};

// ##### ����I�u�W�F�N�g�ړ����� #####
// �Q�l���� : �Ȃ�
// ����     : pPos, vec2, p
// �߂�l   : �Ȃ�
// ����     : p�ɂ�1��2������K�v������B1��pPos1�A2��pPos2�𑀍삷��
void moveCross(Vector2 *pVec2, Vector2 vec2, int p) {
    if (p == 1) {
        // �\���L�[��pPos2���ړ�������
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
        // WASD�L�[��pPos1���ړ�������
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
