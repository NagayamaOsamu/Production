/*===========================================================================================
�T�@���F�Q�[���I�[�o�[��ʂł̏������܂Ƃ߂��N���X�̃w�b�_�[�쐬
�쐬���F12��21��
�X�V���F12��21��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "UnitBase.h"
#include "Sprite.h"

//���d��`�h�~����
#pragma once

//���j�b�g�x�[�X�N���X���p��
class GameOver	:	public UnitBase
{
	//�N���X�̃C���X�^���X�쐬
	Sprite* gameOver;			//�Q�[���I�[�o�[�̉摜

public:
	GameOver();			//�R���X�g���N�^
	~GameOver();		//�f�X�g���N�^

	HRESULT Load();		//�ǂݍ��ݏ���
	HRESULT	Update();	//�X�V����
	HRESULT	Render();	//�`�揈��
};

