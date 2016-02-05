/*===========================================================================================
�T�@���F�J�����̐ݒ������w�b�_�[
�쐬���F8��27��
�X�V���F8��27��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Global.h"

//���d��`�h�~����
#pragma once

class Camera
{
	//-------------�r���[�s��Ŏg���ϐ�-------------
	D3DXVECTOR3     pos;     //�J�����̈ʒu
	D3DXVECTOR3		target;  //�œ_�ʒu
	D3DXVECTOR3     upVec;   //�����

	//-------------�ˉe�s��Ŏg���ϐ�-----------------
	float angle;        //��p
	float aspect;      //�A�X�y�N�g��
	float nearClip;    //�߃N���b�s���O
	float farClip;     //���N���b�s���O

	//------------�s�������ϐ�-----------------
	D3DXMATRIX		view;    //�r���[�s��
	D3DXMATRIX		proj;	 //�ˉe�s��
	
	//-----------�s��쐬����֐�-------------------
	HRESULT SetView();			//�r���[�s����쐬����
	HRESULT SetProjection();	//�ˉe�s����쐬����

public:

	 Camera();				//�R���X�g���N�^
	~Camera();				//�f�X�g���N�^

	HRESULT Update();		//�X�V����

	//----------------�r���[�s��Ŏg���ϐ��ɒl��n���A�N�Z�X�֐�(�S�ăC�����C���֐�)-------------
	void SetPos	  (D3DXVECTOR3 v){ pos	  = v; }		 //�J�����̈ʒu
	void SetTarget(D3DXVECTOR3 v){ target = v; }		 //�œ_�ʒu
	void SetUp	  (D3DXVECTOR3 v){ upVec  = v; }		 //�����

	//----------------�ˉe�s��Ŏg���ϐ��ɒl��n���A�N�Z�X�֐�(�S�ăC�����C���֐�)--------------
	void SetAngle	(float f){ angle	= f; }			//��p
	void SetAspect	(float f){ aspect	= f; }			//�A�X�y�N�g��
	void SetNearClip(float f){ nearClip = f; }			//�߃N���b�s���O
	void SetFarClip	(float f){ farClip	= f; }			//���N���b�s���O

	//---------------�e�s���n���A�N�Z�X�֐�(�S�ăC�����C���֐�)--------------------------------
	D3DXMATRIX* GetView(){ return &view; }				//�r���[�s��
	D3DXMATRIX* GetProj(){ return &proj; }				//�ˉe�s��


};

