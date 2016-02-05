/*===========================================================================================
�T�@���F�v���C���[�̏������܂Ƃ߂��N���X�w�b�_�[
�쐬���F10��15��
�X�V���F10��15��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "UnitBase.h"	//���j�b�g�x�[�X�N���X
#include "Fbx.h"		//Fbx�N���X
#include "Sprite.h"		//�X�v���C�g�N���X

//�萔�錾
#define QUOT 90;		//90�x

//���d��`�h�~����
#pragma once

//���j�b�g�x�[�X�N���X���p������
class Player : public UnitBase
{
	//-----------private�����o--------------

	//�N���X�ϐ�
	Fbx*	player;		//�v���C���[�̃��f���p��Fbx�N���X�̃C���X�^���X�쐬
	Sprite* lifeMark[5];	//���C�t�̉摜�p��Sprite�N���X�̃C���X�^���X�쐬

	//�����o�ϐ�
	D3DXVECTOR3 move;			//�ړ���
	BOOL		isStage;		//�v���C���[���X�e�[�W�ɓ������Ă��邩����
	float		dirX;			//x���̉�]��
	float		dirZ;			//z���̉�]��
	DWORD		blockId;		//�X�e�[�W�̎��
	D3DXMATRIX  matRX;			//x���ɉ�]������s��
	D3DXMATRIX	matRZ;			//z���ɉ�]������s��
	D3DXVECTOR3 normal;			//�e�X�e�[�W�u���b�N�̖̂@��
	D3DXVECTOR3 normalVec;		//���ׂ�x�N�g���Ŏg���x�N�g��(��ɐ^���ɔ�΂�)
	int			life;			//�̗�
	int			timeCount;		//���Ԃ��J�E���g
	BOOL		isSprikHit;		//��Q���ƏՓ˂������ǂ���
	BOOL		isDamege;		//�_���[�W���󂯂����ǂ���
	BOOL		isDown;			//�X�e�[�W���炨���Ă��邩�ǂ���
	float		downTime;		//�X�e�[�W���痎���Ă��鎞��

	//�����o�֐�
	HRESULT Move();				//�ړ�����

	HRESULT frontMove();		//�O�ړ�����
	HRESULT backMove();			//��ړ�����
	HRESULT leftMove();			//���ړ�����
	HRESULT rightMove();		//�E�ړ�����

	HRESULT frontFriction();	//�O���C����
	HRESULT backFriction();		//�㖀�C����
	HRESULT leftFriction();		//�����C����
	HRESULT rightFriction();	//�E���C����

	HRESULT Rotation();			//��]����
	HRESULT MoveCamera();		//�J�����̈ړ�����


	//------------public�����o--------------
public:

	//�����o�֐�
	 Player();			//�R���X�g���N�^
	~Player();			//�f�X�g���N�^

	HRESULT Load();						//�ǂݍ��ݏ���
	HRESULT Update();					//�X�V����
	HRESULT Hit(UnitBase* pTarget);		//�Փ˔��菈��
	HRESULT Render();					//�`�揈��

};

