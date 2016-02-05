/*===========================================================================================
�T�@���F�X�e�[�W�̑I�������鏈�����܂Ƃ߂��N���X�̃w�b�_�[
�쐬���F1��28��
�X�V���F1��28��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "UnitBase.h"		//���j�b�g�x�[�X�N���X
#include "Sprite.h"			//�X�v���C�g�N���X
#include <vector>			//�x�N�^�[

//���d��`�h�~����
#pragma once

//���j�b�g�x�[�X�N���X���p��
class StageSelect : public UnitBase
{
	//�N���X�̃C���X�^���X�쐬
	Sprite* backGrround;		//�w�i�摜
	Sprite* arrow;				//���摜
	Sprite* text;				//�e�L�X�g�摜
	Sprite* num;				//�X�e�[�W�ԍ�

	//�����o�ϐ�
	int selectStage;						//���I��ł���X�e�[�W
	std::vector<DWORD> stagePartsArray;		//�X�e�[�W�̃u���b�N�̎�ނ��i�[����

	//�v���C�x�[�g�֐�
	int GetComma(char data[], int* index);		//�ǂݍ��񂾃X�e�[�W�t�@�C�����J���}��؂�Ńf�[�^���擾

public:
	StageSelect();			//�R���X�g���N�^
	~StageSelect();			//�f�X�g���N�^

	HRESULT Load();			//�ǂݍ��ݏ���
	HRESULT Update();		//�X�V����
	HRESULT Render();		//�`�揈��
};

