/*===========================================================================================
�T�@���F�Q�[���̉���炷����
�쐬���F7��3��
�X�V���F7��3��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "Effect2D.h"

//�R���X�g���N�^
Effect2D::Effect2D()
{
	//������
	sprite = 0;
}

//�f�X�g���N�^
Effect2D::~Effect2D()
{
	//���X�g�̉��
	dataList.clear();

	//�X�v���C�g�N���X�폜
	SAFE_DELETE(sprite);
}

//�@�\�F�ǂݍ��ݏ���
//�����FfileName	�t�@�C���̖��O
//�����Fsz			�摜�̃T�C�Y
//�����Fsp			�`�摬�x
//�ߒl�F���������s��
HRESULT Effect2D::Load(char* fileName,D3DXVECTOR2 sz,float sp)
{
	//�X�v���C�g�N���X�𓮓I�쐬
	sprite = new Sprite;

	//�T�C�Y�ƕ`��X�s�[�h������
	size = sz;
	speed = sp;

	sprite->Load(fileName);		//�G�t�F�N�g�p�̉摜�̓ǂݍ���
	
	return S_OK;				//������Ԃ�
}

//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT Effect2D::Draw()
{	
	SpriteData data;			//�X�v���C�g�N���X�̏�������\���̂̏���

	//�G�t�F�N�g���A�j���V���[�����ĕ\��������
	data.size = size;


	//dataList�̃C�e���[�^�쐬
	std::list<EffectData>::iterator dL_i;

	for (dL_i = dataList.begin(); dL_i != dataList.end();)				//�z��̐��������[�v
	{
		//�ʒu���
		data.pos.x = (*dL_i).position.x;	//�C�e���[�^���w���Ă���potision�̔Ԓn
		data.pos.y = (*dL_i).position.y;	//dL_i->position.y�ł��\

		//�؂蔲���ʒu���
		data.cut.x = (int)(*dL_i).anime * data.size.x;	//anime�ϐ���int�^�ɂ��Ĉʒu�������̂�h��
		data.cut.y = 0;

		sprite->Draw(&data);								//���ꂽ�f�[�^�����Ƃɕ`��

		(*dL_i).anime += speed;							//�J�E���g�A�b�v�i�����_�Ōv�Z�J�E���g�A�b�v���ĕ`�摬�x��x������j

		//anime���Ō�܂ŕ`�悵����i�摜�̑S�̂̕��@/�@1�}�X�̃T�C�Y �Ɣ�r����j
		if (sprite->GetSize().x / size.x < (*dL_i).anime)
		{
			//�G�t�F�N�g�̍폜
			dL_i = dataList.erase(dL_i);

			//�G�t�F�N�g�������Ď��̃C�e���[�^����ꂽ�̂ŃC���N�������g�����Ȃ��ŏI���
			continue;
		}

		//�G�t�F�N�g���폜���Ȃ������������C���N�������g
		dL_i++;
	}

	return S_OK;				//������Ԃ�


	/*	if (sprite->GetSize().x / size.x < (*dL_i).anime)
	{
		
		dL_i = dataList.erase(dL_i);

	}

	else
	{
		dL_i++;
	}
	�̏������ł��ǂ�
	*/

}

//�@�\�F�G�t�F�N�g�摜��ǉ�����
//�����Fpos�@�`��ʒu
//�ߒl�F���������s��
HRESULT Effect2D::Add(D3DXVECTOR3 pos)
{
	//�\���̍쐬
	EffectData d;

	//���������Ƀf�[�^�ύX
	d.position = pos;
	d.anime = 0;

	//�z��ɒǉ�
	dataList.push_back(d);

	//������Ԃ�
	return S_OK;
}