/*===========================================================================================
�T�@���F�X�e�[�W�̏������܂Ƃ߂��N���X�w�b�_�[
�쐬���F10��15��
�X�V���F10��15��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "UnitBase.h"	//���j�b�g�x�[�X�N���X
#include "Fbx.h"		//Fbx�N���X
#include <vector>		//���I�z��

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
class Stage : public UnitBase
{
	//------------�\����-----------------

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


	//�e�m�[�h�����܂Ƃ߂��\����
	struct Node
	{
		D3DXMATRIX matrix;							//�e�u���b�N���Ƃ̍s��
		DWORD type;									//�u���b�N�̎��(�����AT���H�A���E�����)
		std::vector<Node*> childNode;				//�u���b�N������������������̂œ��I�z��ɂ���
		D3DXVECTOR3 pos;							//�ʒu
		std::vector<D3DXVECTOR3> sprikPos;			//��Q���̈ʒu
	};

	//-------------�����o�ϐ�----------------

	char flg;

	//��Q���̃��f���ϐ�
	Fbx* sprik;

	//�m�[�h�ϐ�
	Node startNode;									//�J�n�m�[�h	
	DWORD nodeID;									//�m�[�h�̔ԍ��i�J�n�_���牽�ڂ̃m�[�h���j

	//�v���C���[�̈ʒu���
	D3DXVECTOR3 playerPos;							//�v���C���[�̈ʒu

	//�X�e�[�W�u���b�N�ϐ�
	DWORD stageBlock;								//�X�e�[�W�u���b�N�̎��

	//�Փˏ����Ŏg���ϐ�
	D3DXVECTOR3 blocksNormal;						//�X�e�[�W�u���b�N�̖@��
	BOOL		isSprikHit;							//��Q���ƏՓ˂�����true�����Ă��Ȃ��Ƃ���false�ɂ���
	BOOL		isPlayerDown;						//�v���C���[���X�e�[�W���痎���Ă��邩�ǂ���

	//-----------private�֐�--------------------
	void Create(D3DXVECTOR3 pos, float angle, Node* pNode);				//�X�e�[�W�쐬
	HRESULT BlocksRender(D3DXVECTOR3 pos, float angle, Node* pNode);	//�p�[�c�`��
	void Clear(Node* pNode);											//�m�[�h�̍폜
	HRESULT Hit(Node* pNode);											//�X�e�[�W�u���b�N�ƃv���C���[�̏Փ˔���


public:
	Stage();		//�R���X�g���N�^
	~Stage();		//�f�X�g���N�^

	HRESULT Load();						//�ǂݍ��ݏ���
	HRESULT Update();					//�X�V����
	HRESULT Hit(UnitBase* pTarget);		//�Փ˔��菈��
	HRESULT Render();					//�`�揈��

	//-----------�A�N�Z�X�֐�------------------
	float		GetPlayerPos()	 { return playerPos.y; }		//�v���C���[�ƃX�e�[�W�u���b�N�̏Փˈʒu��n��
	D3DXVECTOR3 GetBlocksNormal(){ return blocksNormal; }		//�X�e�[�W�u���b�N�̖@����n��
	BOOL		GetIsSprikHit()	 { return isSprikHit; }			//��Q���ƃv���C���[���Փ˂������ǂ�����Ԃ�
	BOOL		GetPlayerDown(){ return isPlayerDown; }			//�v���C���[���X�e�[�W���痎���Ă��邩�ǂ�����Ԃ�
};

