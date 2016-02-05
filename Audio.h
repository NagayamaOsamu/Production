/*===========================================================================================
�T�@���F�Q�[���̉���炷����
�쐬���F6��26��
�X�V���F6��26��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Global.h"
#include <xact3.h>

//���d��`�h�~����
#pragma once

class Audio
{
	//�����o�ϐ�
	IXACT3Engine*		xactEngine;		//XACT�G���W���̏�������ϐ�
	IXACT3WaveBank*		waveBank;		//�g�`�̏�������
	IXACT3SoundBank*   soundBank;		//����炷�炳�Ȃ���������
	void*			   mapWaveBank;		//wavbank�̏�������ivoid*�͉��ł����������̂ƍl���Ă悢�j
	void*			   soundBankData;	//soundbank�̏�������

	//�����o�֐��iprivate�j
	HRESULT InitEngine();	//XACT�G���W���̏���
	HRESULT LoadWaveBank(char* fileName);		//xwb�t�@�C���̓ǂݍ���
	HRESULT LoadSoundBank(char* name);			//xsb�t�@�C���̓ǂݍ���

public:
	Audio();	//�R���X�g���N�^
	~Audio();	//�f�X�g���N�^

	HRESULT Load(char* waveBankFileName, char* soundBankFileName);	//����炷�̂ɕK�v�Ȋ֐��̓ǂݍ���

	HRESULT Audio::Play(char* cueName);								//�����Đ�������
	HRESULT Audio::Stop(char* cueName);								//�����~������
};

