/*===========================================================================================
�T�@���FMaya�ō쐬�������f��(Fbx�`��)��`�悷��N���X�w�b�_�[
�쐬���F9��10��
�X�V���F10��5��
����ҁF�i�R����
=============================================================================================*/

//--------------------�C���N���[�h------------------
#include "Global.h"
#include <fbxsdk.h>	
#include <vector>

//--------------------���C�u����-----------------
#pragma comment(lib,"libfbxsdk-mt.lib")

//���d��`�h�~����
#pragma once

//���C�L���X�g�p�\����(D3DXIntersetTri�֐��ŕK�v�ɂȂ�f�[�^���܂Ƃ߂�)
struct RayCastData
{
	D3DXVECTOR3 start;	//���C���ˈʒu
	D3DXVECTOR3 dir;	//���C�̌����x�N�g��
	float		dist;	//�Փ˓_�܂ł̋���
	D3DXVECTOR3 normal;	//�Փ˓_�̖@��
	D3DXVECTOR3 pos;	//�Փˈʒu
	BOOL		hit;	//���C������������
	D3DXMATRIX	matrix;	//���f���̍s��
};

class Fbx
{
	//FBX�t�@�C�����J���̂ɕK�v�ȕϐ�
	FbxManager*					manager;					//�}�l�[�W��(FBX�@�\���Ǘ�����)
	FbxImporter*				importer;					//�C���|�[�^(�t�@�C�����J���̂ɕK�v)
	FbxScene*					scene;						//�V�[��(�J�����t�@�C�����Ǘ�����)

	//���b�V���̒��_�o�b�t�@�����̂ɕK�v�ȕϐ�
	int							vertexCount;				//���_�̐�
	IDirect3DVertexBuffer9      *pVertexBuffer;				//��������_�o�b�t�@������ϐ�

	//�C���f�b�N�X�o�b�t�@�����̂ɕK�v�ȕϐ�
	int polygonCount;										//�|���S����
	int indexCount;											//�C���f�b�N�X��
	IDirect3DIndexBuffer9    **pIndexBuffer;				//������C���f�b�N�X�o�b�t�@������ϐ��̃|�C���^�̃|�C���^(�|�C���^�ϐ��̃A�h���X���i�[����ϐ�)

	//�}�e���A���쐬�ɕK�v�ȕϐ�
	int					  materialCount;					//�}�e���A���̌�
	D3DMATERIAL9          *pMaterial;						//�}�e���A���̔z������̂ɕK�v�ȃ|�C���^
	int*				  polygonCountOfMaterial;			//�}�e���A�����̃|���S����������|�C���^

	//�e�N�X�`����\��̂ɕK�v�ȕϐ�
	LPDIRECT3DTEXTURE9       *pTexture;						//�e�N�X�`��������|�C���^

	//���C�L���X�g�����Ŏg������
	D3DXMATRIX lastWorldMatrix;								//�Ō�Ɏg�����s����L��������ϐ�
	std::vector<D3DXMATRIX> lastMatrix;
	
	
	//���b�V���ɕK�v�Ȓ��_�����\���̂ɓ����
	struct Vertex
	{
		D3DXVECTOR3 pos;	//�ʒu���
		D3DXVECTOR3 normal;	//�@�����(�@���͒��_��񂾂��|���S�����Ɍ��܂��Ă���)
		D3DXVECTOR2						uv;						//UV���W
	};

	//�v���C�x�[�g�֐�
	HRESULT      CheckNode(FbxNode* pNode);		//���b�V���m�[�h��T���֐�
	HRESULT      CheckMesh(FbxMesh* pMesh);		//���b�V���̒��_�������o���֐�

	//�o�b�t�@�쐬
	HRESULT		CreateVertexBuf(FbxMesh* pMesh);				//���_�o�b�t�@�쐬
	HRESULT		CreateindexBuf(FbxMesh* pMesh);					//�C���f�b�N�X�o�b�t�@�쐬

public:
	 Fbx();		//�R���X�g���N�^
	~Fbx();		//�f�X�g���N�^

	HRESULT Load(char* fileName);			//�ǂݍ��ݏ���
	HRESULT Draw(D3DXMATRIX* matWorld);		//�`�揈��
	BOOL	RayCast(RayCastData *data);		//���C�L���X�g����

	BOOL		HitRay(D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist,D3DXMATRIX matW);
	BOOL		HitRay(D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXVECTOR3* normal, D3DXMATRIX matW);
	//BOOL		HitRay(MeshFBX * pMesh D3DXVECTOR3* pRayPos, D3DXVECTOR3 *pRayDir, float* pDist, D3DXVECTOR3* normal);

	BOOL HitMouseRay(D3DXMATRIX matW);		//�}�E�X���W��Fbx�̏Փ˔���
};

