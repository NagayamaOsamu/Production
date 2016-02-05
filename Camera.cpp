/*===========================================================================================
�T�@���F�J�����̐ݒ������N���X�̊֐���`
�쐬���F8��27��
�X�V���F8��27��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Camera.h"

//�R���X�g���N�^
Camera::Camera()
{
	//�r���[�s��p�̕ϐ��ɏ����l����
	pos	   =	D3DXVECTOR3(0, 3, -10);		//�J�����̈ʒu
	target =	D3DXVECTOR3(0, 0, 0);		//�œ_�ʒu
	upVec  =	D3DXVECTOR3(0, 1, 0);		//�����

	//�ˉe�s��p�̕ϐ��ɏ����l����
	angle = 60;												//��p
	aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;			//�A�X�y�N�g��
	nearClip = 0.1f;										//�߃N���b�s���O	����͂O�ɂ��Ȃ��ق�������
	farClip = 1000.0f;										//���N���b�s���O

}

//�f�X�g���N�^
Camera::~Camera()
{

}

//�@�\�F�r���[�s��(�J�����̈ʒu�����)���쐬����
//�����F�Ȃ�
//�ߒl�F���s��������
HRESULT Camera::SetView()
{
	//�r���[�s����쐬(�����͑S�ăA�h���X�ŁA�s�������ϐ�,�J�����̈ʒu,�œ_�ʒu,�����)
	D3DXMatrixLookAtLH(&view, &pos, &target, &upVec);

	//�r���[�s����Z�b�g
	if (FAILED(g_pDevice->SetTransform(D3DTS_VIEW, &view)))					//SetTransform�ŃZ�b�g�A���̊֐��͕ʂ̍s�񂪃Z�b�g�����܂ł��̍s�񂪃Z�b�g���ꑱ����(��P�����ɍs��̎�ނ�����)
	{
		//�s��̃Z�b�g�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Ŏ��s��m�点��
		MessageBox(0, "�r���[�s��̐ݒ�Ɏ��s���܂���", "Camera", MB_OK);

		return E_FAIL;	//���s��Ԃ�
	}

	return S_OK;		//������Ԃ�
}

//�@�\�F�ˉe�s��(��p��A�X�y�N�g��Ȃǂ̐ݒ�)���쐬����
//�����F�Ȃ�
//�ߒl�F���s��������
HRESULT Camera::SetProjection()
{
	//�ˉe�s��̍쐬(�����́A�s�������ϐ�(�A�h���X),��p(���W�A���ɕϊ�),�A�X�y�N�g��,�߃N���b�s���O�ʂ܂ł̋���,���N���b�s���O�ʂ܂ł̋���)
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(angle), aspect, nearClip, farClip);

	//�ˉe�s��̃Z�b�g
	if (FAILED(g_pDevice->SetTransform(D3DTS_PROJECTION, &proj)))
	{
		//�s��̃Z�b�g�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Ŏ��s��m�点��
		MessageBox(0, "�ˉe�s��̐ݒ�Ɏ��s���܂���", "Camera", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F���s��������
HRESULT Camera::Update()
{
	//�r���[�s��쐬�֐����Ă�
	if (FAILED(SetView()))
	{
		//���s���Ă����玸�s��Ԃ�
		return E_FAIL;		
	}
	
	//�ˉe�s��쐬�֐����Ă�
	if (FAILED(SetProjection()))
	{
		//���s���Ă����玸�s��Ԃ�
		return E_FAIL;
	}

		return S_OK;		//������Ԃ�
}