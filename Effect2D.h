/*===========================================================================================
�T�@���F�Q�[���̃G�t�F�N�g����
�쐬���F7��3��
�X�V���F7��3��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Sprite.h"
#include <list>

//���d��`�h�~����
#pragma once

class Effect2D
{
	Sprite* sprite;			//�X�v���C�g�N���X�̃|�C���^�쐬
	float		speed;		//�`�摬�x
	D3DXVECTOR2 size;		//�摜�̂P�R�}�̃T�C�Y������

	//�P�̉摜�ł������g���\��������ϐ����\���̂ɂ܂Ƃ߂�
	struct EffectData
	{
		float		anime;		//�A�j���[�V�����p�̕ϐ�
		D3DXVECTOR3 position;	//�`��ʒu
	};

	//�\���̂𓮓I�z��ɂ���
	std::list<EffectData> dataList;

public:
	Effect2D();		//�R���X�g���N�^
	~Effect2D();	//�f�X�g���N�^

	HRESULT Load(char* fileName,D3DXVECTOR2 sz, float a_sp);	//�ǂݍ��ݏ���
	HRESULT Draw();					//�`�揈��
	HRESULT Add(D3DXVECTOR3 pos);					//�G�t�F�N�g��ǉ�����֐�
};

