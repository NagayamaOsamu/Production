/*===========================================================================================
�T�@���F��{�I�ȃ��b�V���̏���������N���X�̊֐���`
�쐬���F9��3��
�X�V���F9��3��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "BasicMesh.h"


//�R���X�g���N�^
BasicMesh::BasicMesh()
{
	//������
	pMesh = 0;
}

//�f�X�g���N�^
BasicMesh::~BasicMesh()
{
	//LP�^�̉��
	SAFE_RELEASE(pMesh);
}

//�@�\�F�����̂̃��b�V�������
//�����F�쐬���������b�V���̎��
//�ߒl�F���������s��
HRESULT BasicMesh::Create(DWORD type)
{
	HRESULT hr;		//���b�V�����쐬�����Ƃ��̐��������s������ϐ�

	switch (type)
	{
		//�����̂��I�΂ꂽ��
	case BMESH_BOX:

		//�����̂̃��b�V�����쐬(�����E���s��ϐ��ɓ����)
		hr = D3DXCreateBox(g_pDevice, 1, 1, 1, &pMesh, NULL);					//������(�f�o�C�X�I�u�W�F�N�g,��,����,���s��,���b�V���̃A�h���X,NULL)

		break;

		//���̂��I�΂ꂽ��
	case BMESH_SPHERE:

		//���̂̃��b�V�����쐬
		hr = D3DXCreateSphere(g_pDevice, 1, 24, 24, &pMesh, NULL);				//������(�f�o�C�X�I�u�W�F�N�g,���a,�p�̐�,������,,���b�V���̃A�h���X,NULL)

		break;

		//�~�����I�΂ꂽ��
	case BMESH_CYLINDER:

		//�~���̃��b�V�����쐬
		hr = D3DXCreateCylinder(g_pDevice, 1, 1, 3, 24, 1, &pMesh, NULL);		//������(�f�o�C�X�I�u�W�F�N�g,��O�̔��a,���̔��a,����,������,�X�^�b�N��,���b�V���̃A�h���X,NULL)

		break;

		//�g�[���X(�h�[�i�b�c��)���I�΂ꂽ��
	case BMESH_TORUS:

		//�g�[���X�̃��b�V���쐬
		hr = D3DXCreateTorus(g_pDevice, 0.5, 1, 12, 24, &pMesh, NULL);			//������(�f�o�C�X�I�u�W�F�N�g,���̔��a,�O���̔��a,�ӂ̐�,�p�̐�,���b�V���̃A�h���X,NULL)

		break;

		//�e�B�[�|�b�g���I�΂ꂽ��
	case BMESH_TEAPOT:

		//�e�B�[�|�b�g�̃��b�V���쐬
		hr = D3DXCreateTeapot(g_pDevice, &pMesh, NULL);							//������(�f�o�C�X�I�u�W�F�N�g,���b�V���̃A�h���X,NULL)

		break;
	}
	
	//���b�V���̍쐬�Ɏ��s������
	if (FAILED(hr))
	{
		//���b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���b�V���̍쐬�Ɏ��s���܂���", "BasicMesh", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}


	InitMaterial();			//�}�e���A���̐ݒ������֐��̌Ăяo��

	return S_OK;			//������Ԃ�
}

//�@�\�F�}�e���A���̐ݒ�
//�����F�Ȃ�
//�ߒl�F���s��������
HRESULT BasicMesh::InitMaterial()
{
	//�\���̂�S�ď�����
	ZeroMemory(&material, sizeof(D3DMATERIAL9));

	//�f�B�t���[�Y(�g�U����)��RGB�l�ƃA���t�@�l�̐ݒ�
	//�f�B�t���[�Y�Ƃ͉��F�̌����ǂ̂��炢���˂��Ă��邩������킷
	material.Diffuse.r = 1.0f;		//�ԐF�̌��̋���
	material.Diffuse.g = 1.0f;		//�ΐF�̌��̋���
	material.Diffuse.b = 1.0f;		//�F�̌��̋���
	material.Diffuse.a = 1.0f;		//�����x

	//�A���r�G���g(����)�𔽎˂�����ݒ�
	material.Ambient.r = 1.0f;		//�ԐF�̌�
	material.Ambient.g = 1.0f;		//�ΐF�̌�
	material.Ambient.b = 1.0f;		//�F�̌�
	material.Ambient.a = 1.0f;		//�����x

	return S_OK;			//������Ԃ�
}

//�@�\�F�`�揈��
//�����F���[���h�s��
//�ߒl�F���s��������
HRESULT BasicMesh::Draw(D3DXMATRIX* matWorld)
{
	//�e�N�X�`����\��Ȃ�����
	if (FAILED(g_pDevice->SetTexture(0, NULL)))
	{
		//�����Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�e�N�X�`���̃��Z�b�g�Ɏ��s���܂���", "BasicMesh", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//���[���h�s��ϊ�
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		//�ݒ�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���[���h�s��̐ݒ�Ɏ��s���܂���", "BasicMesh", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	
	//�}�e���A���̐ݒ�̃Z�b�g
	if (FAILED(g_pDevice->SetMaterial(&material)))
	{
		//�ݒ�Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�}�e���A���̐ݒ�Ɏ��s���܂���", "BasicMesh", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�����̂̕`��(�O���[�v���������Ȃ�)
	if (FAILED(pMesh->DrawSubset(0)))				//�����̓O���[�v(�T�u�Z�b�g)��ݒ肷��
	{
		//�`��Ɏ��s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���b�V���̕`��Ɏ��s���܂���", "BasicMesh", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}
	return S_OK;			//������Ԃ�
}


