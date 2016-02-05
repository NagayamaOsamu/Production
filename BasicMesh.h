/*===========================================================================================
�T�@���F��{�I�ȃ��b�V��(�R�c�|���S���̕���)�̏���������N���X�̃w�b�_�[��`
�쐬���F9��3��
�X�V���F9��3��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Global.h"


//�e�`��(���b�V��)�ɖ��O��t����
enum
{
	BMESH_BOX,			//������
	BMESH_SPHERE,		//����
	BMESH_CYLINDER,		//�~��
	BMESH_TORUS,		//�g�[���X
	BMESH_TEAPOT		//�e�B�[�|�b�g
};

//���d��`�h�~����
#pragma once

class BasicMesh
{
	LPD3DXMESH   pMesh;		//�����̂̃��b�V��������ϐ�
	D3DMATERIAL9 material;	//�}�e���A���̐ݒ荀�ڂ��܂Ƃ߂��ϐ��i�\���́j

public:
	 BasicMesh();		//�R���X�g���N�^
	~BasicMesh();		//�f�X�g���N�^


	HRESULT Create(DWORD type);					//�����̂̃��b�V�������
	HRESULT InitMaterial();						//�}�e���A���̐ݒ������
	HRESULT Draw(D3DXMATRIX* matWorld);			//�`�揈��

};

