/*===========================================================================================
�T�@���F�Q�c�̎l�p�`�����N���X�̃w�b�_�[��`
�쐬���F9��1��
�X�V���F9��1��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Global.h"

//���d��`�h�~����
#pragma once

class Quad
{
	//�e�N�X�`���I�u�W�F�N�g
	LPDIRECT3DTEXTURE9 pTexture;

public:
	 Quad();			//�R���X�g���N�^
	~Quad();			//�f�X�g���N�^

	HRESULT Load(char* fileName);										//�ǂݍ��ݏ���
	HRESULT Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex = NULL);		//�l�p�`�̃|���S���`��
};

