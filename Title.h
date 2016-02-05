/*===========================================================================================
�T�@���F�^�C�g����ʂł̏������܂Ƃ߂��N���X�w�b�_�[�쐬
�쐬���F12��21��
�X�V���F12��21��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "UnitBase.h"
#include "Sprite.h"

//���d��`�h�~����
#pragma once

//��ʂ̏�Ԃ�萔�Ő錾
enum
{
	WAIT_STATE,				//�ҋ@��
	MODESELECT_STATE,		//���[�h�Z���N�g
	MAX_STATE				//�ő�l
};

//���[�h�Z���N�g��Ԃ̂Ƃ��ǂ̃��[�h��I��ł��邩��萔�Ő錾
enum
{
	SELECT_MODE,			//�X�e�[�W�I�����[�h
	CREATE_MODE,			//�X�e�[�W�쐬���[�h
	MAX_MODE				//�ő�l
};

//���j�b�g�x�[�X�N���X���p��
class Title	: public UnitBase
{
	//�N���X�̃C���X�^���X�쐬
	Sprite* title;			//�^�C�g���摜
	Sprite* wait;			//�ҋ@���̉摜
	Sprite* modeSelect;		//���[�h�Z���N�g�Ɏg���摜
	Sprite* arrow;			//���摜

	//�����o�ϐ�
	int state;				//��ʂ̏��
	int mode;				//�I��ł��郂�[�h

public:
	Title();			//�R���X�g���N�^
	~Title();			//�f�X�g���N�^

	HRESULT Load();		//�ǂݍ��ݏ���
	HRESULT	Update();	//�X�V����
	HRESULT	Render();	//�`�揈��
};

