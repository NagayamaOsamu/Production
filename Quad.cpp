/*===========================================================================================
�T�@���F�Q�c�̎l�p�`�����N���X�̊֐���`
�쐬���F9��1��
�X�V���F9��1��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Quad.h"

//�R���X�g���N�^
Quad::Quad()
{
	//������
	pTexture = 0;
}

//�f�X�g���N�^
Quad::~Quad()
{
	//�e�N�X�`���̉��
	SAFE_RELEASE(pTexture);
}

//�@�\�F�ǂݍ��ݏ���
//�����F�t�@�C����
//�ߒl�F���������s��
HRESULT Quad::Load(char* fileName)
{
	//�u�e�N�X�`���I�u�W�F�N�g�v�̍쐬
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture)))				//��1������Direct�RD�f�o�C�X�A��2�����Ƀt�@�C������
	{																								//�w�肷��ƃe�N�X�`��������A
		//���s������ǂ̉摜���ǂݍ��߂Ȃ��������\��												//�Ō�̈����Ɏw�肵���ϐ��ɁA���������e�N�X�`�����͂���
		MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", fileName, MB_OK);

		return E_FAIL;	//���s�������Ƃ�Ԃ�(�V�[���̃��j�b�g�ȂǂŎ��s���Ă���Ƃ�)
	}

	return S_OK;	//�����������Ƃ�Ԃ�
}

//�@�\�F�l�p�`�|���S���̕`��
//�����F���[���h�s��
//�ߒl�F���s��������
HRESULT Quad::Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex)
{
	//���_�X�g���[��������ϐ����\���̂ō쐬
	struct Vertex
	{
		D3DXVECTOR3   pos;		//�ʒu���
		DWORD		color;		//�F
		D3DXVECTOR2    uv;		//�e�N�X�`����UV���W(���_���ƂɃe�N�X�`���̂ǂ̕�����\��t���邩)
	};

	//�l�p�`���ɓ�������DirectX���ƃ|���S���͎O�p�`�������݂��Ȃ�
	//�Ȃ̂ŎO�p�`��2��肻������̂����Ďl�p�`�ɂ���
	//����ȊO�ɂ�D3DPT_TRIANGLESTRIP�Ɏw�肷��
	//LIST�͗��ꂽ�����̐}�`��`�����̂ɑ΂�
	//STRIP�͌q�������}�`��`�����̂Ȃ̂Œ��_���S�w�肷�邾���Ŏl�p�`��������

	//�z�������ď����l����
	Vertex point[4] =
	{
		D3DXVECTOR3(-0.5, 0.5, 0)   , D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 0),       //����
		D3DXVECTOR3( 0.5 , 0.5 , 0) , D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 0),       //�E��
		D3DXVECTOR3(-0.5 , -0.5, 0) , D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 1),       //����
		D3DXVECTOR3( 0.5 , -0.5 ,0) , D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 1),       //�E��
	};

	//�l�p�`�̏���`����
	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)))		//������ �b ���g�����Ƃŕ����̏���ݒ�ł���
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Ŏ��s��m�点��
		MessageBox(0, "���_�X�g���[���̐ݒ�Ɏ��s���܂���", "Line", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�����̍s������[���h�s��ɕϊ�����
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		//�ϊ��Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���[���h�s��̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�e�N�X�`���̃Z�b�g
	if (FAILED(g_pDevice->SetTexture(0, pTexture)))
	{
		//�Z�b�g�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�e�N�X�`���̃Z�b�g�Ɏ��s���܂���", "Quad", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�e�N�X�`���s�񂪋�łȂ��Ƃ������s��̐ݒ���s��
	if (matTex != 0)
	{

		//�e�N�X�`���s��̃Z�b�g
		if (FAILED(g_pDevice->SetTransform(D3DTS_TEXTURE0, matTex)))
		{
			//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
			MessageBox(0, "�e�N�X�`���s��̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);

			return E_FAIL;		//���s��Ԃ�
		}
	}

	//���C�g��Off�ɂ���
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�l�p�`�̕`��
	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, point, sizeof(Vertex))))		//�����́A�`�悷����,�\�����鐔,���_���,�P���_�̃f�[�^�T�C�Y
	{
		//�`��Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�|���S���̕`��Ɏ��s���܂���", "Quad", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//���C�g��ON�ɂ���
	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���C�g�̐ݒ�Ɏ��s���܂���", "Quad", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	return S_OK;			//������Ԃ�
}