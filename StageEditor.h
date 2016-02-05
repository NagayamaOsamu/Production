/*===========================================================================================
�T�@���F�X�e�[�W�G�f�B�^�[�N���X�̏������܂Ƃ߂��N���X�w�b�_�[�쐬
�쐬���F1��12��
�X�V���F1��12��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include <list>			//���X�g
#include "UnitBase.h"	//���j�b�g�x�[�X�N���X
#include "Fbx.h"		//FBX�N���X
#include "Sprite.h"		//�X�v���C�g�N���X

//���d��`�h�~����
#pragma once

//�萔�錾
enum					//�X�e�[�W�u���b�N���Ƃɒ萔�Ŗ��O��t����
{
	BLOCK_START,		//�X�^�[�g
	BLOCK_STRAIGHT,		//����
	BLOCK_BRANCH,		//T���H
	BLOCK_UP,			//����
	BLOCK_DOWN,			//�����
	BLOCK_GOAL,			//�S�[��
	BLOCK_STOP,			//�s���~�܂�
	BLOCK_MAX			//�ő�u���b�N��
};

//���j�b�g�x�[�X�N���X���p��
class StageEditor : public UnitBase
{
	//�N���X�̃C���X�^���X�쐬
	Sprite* chipWindow;			//�`�b�v�E�B���h�E(�u�������u���b�N��I�����镔��)
	Sprite* frame;				//�ǂ̃`�b�v��I���ł��邩������悤�ɂ���g

	//�e�X�e�[�W�u���b�N�̏o���̏����܂Ƃ߂��\����
	struct ExitState
	{
		D3DXVECTOR3 pos;		//�ʒu
		float		angle;		//�p�x

	};

	//�X�e�[�W�u���b�N�̐ݒ���܂Ƃ߂��\����
	struct
	{
		Fbx stage;							//�X�e�[�W�̃��f��

		//�o���̏����܂Ƃ߂��z��
		std::vector<ExitState> exitList;	//�o������������̂œ��I�z��ɂ���	

	}blocksState[BLOCK_MAX];

	//�����o�ϐ�
	std::list<DWORD> stagePartsList;			//�X�e�[�W�u���b�N���i�[���Ă���
	std::list<D3DXMATRIX> blocksMatList;		//�X�e�[�W�u���b�N���Ƃ̍s��
	D3DXVECTOR3		cameraPos;					//�J�����̈ʒu
	D3DXVECTOR3		cameraTarget;				//�J�����̊p�x
	int				selectChip;					//�I�΂�Ă���`�b�v
	POINT			mouseTapPos;				//�}�E�X�̃N���b�N�ʒu
	POINT			mouseDownPos;				//�}�E�X�̃N���b�N��������Ă�Ԃ̈ʒu
	D3DXVECTOR3		branchPos;					//T���H�u���b�N�̈ʒu
	float			branchAngle;				//T���H�u���b�N�̊p�x
	BOOL			isChipWin;					//�}�E�X�J�[�\�����`�b�v�E�B���h�E�ɂ��Ȃ��Ƃ���FALSE�ɂ���
	std::list<DWORD>::iterator itr;				//���X�gstagePartsList�̃C�e���[�^
	std::list<D3DXMATRIX>::iterator matItr;			//���X�gblocksPos�̃C�e���[�^

public:
	StageEditor();			//�R���X�g���N�^
	~StageEditor();			//�f�X�g���N�^

	HRESULT Load();			//�ǂݍ��ݏ���
	HRESULT Update();		//�X�V����
	HRESULT Render();		//�`�揈��
};

