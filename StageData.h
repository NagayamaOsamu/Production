/*===========================================================================================
�T�@���F�X�e�[�W�̃f�[�^���i�[���邽�߂̃N���X�w�b�_�[(.cpp�͂Ȃ�)
�쐬���F2��2��
�X�V���F2��2��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include <Windows.h>

//���d��`�h�~����
#pragma once

//�V���O���g��(�C���X�^���X���������������Ȃ��悤�ɂ���)���g��
class StageData
{
	static StageData* inst;			//���̃N���X�̃C���X�^���X

	//�R���X�g���N�^
	StageData();	//null������

public:

	 DWORD data[100];					//�X�e�[�W�̃f�[�^������

	//���̃N���X�̃C���X�^���X��n���֐�
	static StageData* GetInst()
	{
		//�C���X�^���X�̒��g��null��������
		if (inst == nullptr)
		{
			inst = new StageData;		//�X�e�[�W�̃C���X�^���X������
		}
		return inst;					//�C���X�^���X��Ԃ�
	}

	//�C���X�^���X�̉���֐�
	static void Free()
	{
		//�폜
		delete inst;
	}
};

