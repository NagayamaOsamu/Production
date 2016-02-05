/*===========================================================================================
�T�@���F�X�e�[�W�̑I�������鏈�����܂Ƃ߂��N���X�̊֐���`
�쐬���F1��28��
�X�V���F1��28��
����ҁF�i�R����
=============================================================================================*/

//�C���N���[�h
#include "StageSelect.h"
#include "Stage.h"		//�X�e�[�W�N���X
#include "StageData.h"	//�X�e�[�W�f�[�^�N���X
#include <string.h>

//�R���X�g���N�^
StageSelect::StageSelect()
{
	//�N���X�̏�����
	backGrround = NULL;		//�w�i�摜
	arrow		= NULL;		//���摜
	text		= NULL;		//�e�L�X�g�摜
	num		    = NULL;		//�X�e�[�W�ԍ��摜

	//�����o�ϐ��̏�����
	selectStage = 0;		//���I��ł���X�e�[�W
}

//�f�X�g���N�^
StageSelect::~StageSelect()
{
	//�N���X�̉��
	SAFE_DELETE(backGrround);		//�w�i�摜
	SAFE_DELETE(arrow);				//���摜
	SAFE_DELETE(text);				//�e�L�X�g�摜
	SAFE_DELETE(num);				//�X�e�[�W�ԍ��摜
}


//�@�\�F�ǂݍ��ݏ���
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT StageSelect::Load()
{
	//�N���X�̓��I�쐬
	backGrround = new Sprite;		//�w�i�摜
	arrow		= new Sprite;		//���摜
	text		= new Sprite;		//�e�L�X�g�摜
	num			= new Sprite;		//�X�e�[�W�ԍ��摜

	//�w�i�摜�̓ǂݍ���
	if (FAILED(backGrround->Load("Asset\\Sprite\\ST_SelectBack.bmp")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�I����ʂ̔w�i�摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//���摜�̓ǂݍ���
	if (FAILED(arrow->Load("Asset\\Sprite\\Arrow.png")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "���摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�e�L�X�g�摜�̓ǂݍ���
	if (FAILED(text->Load("Asset\\Sprite\\StageText.png")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�e�L�X�g�摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	//�X�e�[�W�ԍ��摜�̓ǂݍ���
	if (FAILED(num->Load("Asset\\Sprite\\StageTextNum.png")))
	{
		//���s�����烁�b�Z�[�W�{�b�N�X�Œm�点��
		MessageBox(0, "�X�e�[�W�ԍ��摜�̓ǂݍ��݂Ɏ��s���܂���", "�ǂݍ��݃G���[", MB_OK);

		return E_FAIL;		//���s��Ԃ�
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�X�V����
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT StageSelect::Update()
{
	//�\����L�[�������ꂽ��
	if (g_pInput->IsKeyTap(DIK_UP))
	{
		selectStage--;		//�X�e�[�W�I��ϐ����J�E���g�_�E��
	}

	//�\�����L�[�������ꂽ��
	if (g_pInput->IsKeyTap(DIK_DOWN))
	{
		selectStage++;		//�X�e�[�W�I��ϐ����J�E���g�A�b�v
	}

	//���݂̃J�����g�f�B���N�g���̃t�H���_�����o���Ă�������
	char  defaultCurrentDir[MAX_PATH];							//�f�t�H���g�̃t�H���_��������ϐ�
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);			//���݂̃J�����g�f�B���N�g����GetCurrentDirectory�֐��Œ��וϐ��ɑ��
	//���̏����ȍ~�́��Ŏ擾�����t�H���_���̃t�H���_�������g������悤�ɂ���
	SetCurrentDirectory(defaultCurrentDir);

	

	//�ϐ�selectStage��0�ł��G���^�[�L�[�������ꂽ�Ƃ�
	if (selectStage == 0 && g_pInput->IsKeyTap(DIK_RETURN))
	{

		//�t�@�C�����J��
		HANDLE hFile;
		hFile = CreateFile(
			"sample1.stage",		//�t�@�C����
			GENERIC_READ,			//�A�N�Z�X���[�h�i�ǂݍ��ݗp�j
			0,						//���L�i�Ȃ��j
			NULL,					//�Z�L�����e�B����
			OPEN_EXISTING,			//�J�����@�i�t�@�C�����Ȃ���Ύ��s�j
			FILE_ATTRIBUTE_NORMAL,	//�����ƃt���O�i�ݒ�Ȃ��j
			NULL
			);

		//�J���Ȃ������璆�f
		if (hFile == 0) return S_OK;

		//�t�@�C���T�C�Y�̎擾
		DWORD fileSize = GetFileSize(hFile, &fileSize);

		//�t�@�C���T�C�Y�����������m��
		char* data;
		data = new char[fileSize];

		//�S���ǂݍ���
		DWORD dwBytes = 0;	//�ǂݍ��݈ʒu
		ReadFile(
			hFile,					//�t�@�C���n���h��
			data,					//�f�[�^������ϐ�
			fileSize,				//�ǂݍ��ރT�C�Y
			&dwBytes,				//�ǂݍ��񂾃T�C�Y�i�ǂݍ��݈ʒu�j
			NULL					//�I�[�o�[���b�v�x�\���́i����͎g��Ȃ��j
			);

		//�t�@�C���n���h���̉��
		CloseHandle(hFile);

		
		int index = 0;									//�t�@�C���̏ꏊ�������ϐ�
		int arraySize = GetComma(data, &index);			//�t�@�C���ɂ���f�[�^�̐����擾

		int i = 0;

		//�f�[�^�̐��������[�v
		while (i < arraySize)
		{
			//�t�@�C���f�[�^�́u�u���b�N�̎��(����)�@,(�J���})�v�ŋ�؂��Ă�̂ŃJ���}���Ƃɋ�؂��Ĕz��Ɋi�[����
			stagePartsArray.push_back(GetComma(data, &index));
			StageData::GetInst()->data[i] = stagePartsArray[i];
			i++;

		}

		//�Ō�����m�点���邽�߂�'\0'�������
		StageData::GetInst()->data[i] = 99;
		//�f�t�H���g�̃f�B���N�g���ɖ߂�
		SetCurrentDirectory(defaultCurrentDir);

		
		SAFE_DELETE(data);			//�ϐ�data�����


		g_gameScene = SC_PLAY;					//�v���C�V�[���Ɉڍs
	}

	//�ϐ�selectStage���ł��G���^�[�L�[�������ꂽ�Ƃ�
	if (selectStage == 1 && g_pInput->IsKeyTap(DIK_RETURN))
	{

		//�t�@�C�����J��
		HANDLE hFile;
		hFile = CreateFile(
			"sample2.stage",		//�t�@�C����
			GENERIC_READ,			//�A�N�Z�X���[�h�i�ǂݍ��ݗp�j
			0,						//���L�i�Ȃ��j
			NULL,					//�Z�L�����e�B����
			OPEN_EXISTING,			//�J�����@�i�t�@�C�����Ȃ���Ύ��s�j
			FILE_ATTRIBUTE_NORMAL,	//�����ƃt���O�i�ݒ�Ȃ��j
			NULL
			);

		//�J���Ȃ������璆�f
		if (hFile == 0) return S_OK;

		//�t�@�C���T�C�Y�̎擾
		DWORD fileSize = GetFileSize(hFile, &fileSize);

		//�t�@�C���T�C�Y�����������m��
		char* data;
		data = new char[fileSize];

		//�S���ǂݍ���
		DWORD dwBytes = 0;	//�ǂݍ��݈ʒu
		ReadFile(
			hFile,					//�t�@�C���n���h��
			data,					//�f�[�^������ϐ�
			fileSize,				//�ǂݍ��ރT�C�Y
			&dwBytes,				//�ǂݍ��񂾃T�C�Y�i�ǂݍ��݈ʒu�j
			NULL					//�I�[�o�[���b�v�x�\���́i����͎g��Ȃ��j
			);

		//�t�@�C���n���h���̉��
		CloseHandle(hFile);


		int index = 0;									//�t�@�C���̏ꏊ�������ϐ�
		int arraySize = GetComma(data, &index);			//�t�@�C���ɂ���f�[�^�̐����擾

		//�f�[�^�̐��������[�v
		for (int i = 0; i < arraySize; i++)
		{
			//�t�@�C���f�[�^�́u�u���b�N�̎��(����)�@,(�J���})�v�ŋ�؂��Ă�̂ŃJ���}���Ƃɋ�؂��Ĕz��Ɋi�[����
			stagePartsArray.push_back(GetComma(data, &index));
		}

		//�f�t�H���g�̃f�B���N�g���ɖ߂�
		SetCurrentDirectory(defaultCurrentDir);

		//�ϐ�data�����
		SAFE_DELETE(data);

		g_gameScene = SC_PLAY;		//�v���C�V�[���Ɉڍs
	}

	return S_OK;			//������Ԃ�
}

//�@�\�F�X�e�[�W�t�@�C���̃f�[�^���J���}��؂�Ŏ擾����
//�����Fdata[]�@�t�@�C���̃f�[�^
//�����Findex�@ �ǂݍ��ފJ�n�ʒu
//�ߒl�F�J���}�O�̐��l
int StageSelect::GetComma(char data[], int* index)
{
	//�ϐ�����
	char chr[256];	//�R���}��؂�̃f�[�^������ϐ�
	int i = 0;		//�ϐ�chr�̗v�f		

	//�u,�v�܂ňꕶ�����ϐ��ɓ����
	while (data[*index] != ',')
	{
		chr[i] = data[*index];		//���������
		i++;						//�J�E���g�A�b�v
		(*index)++;					//���̈ʒu��
	}

	//�Ō�Ɂu\0�v��t����
	chr[i] = '\0';		//�z��̍Ō�����m�点
	i++;				//���̕�����
	(*index)++;			//���̈ʒu��

	//�����ɕϊ�
	int value;			//�ϊ���̃f�[�^������ϐ�
	value = atoi(chr);	//atoi�֐����g���ĕ�����𐔒l�ɕϊ�

	//���l�ɕϊ������l��Ԃ�
	return value;
}

//�@�\�F�`�揈��
//�����F�Ȃ�
//�ߒl�F���������s��
HRESULT StageSelect::Render()
{
	//-----------------�X�v���C�g�̕`��--------------------
	SpriteData data;		////�X�v���C�g�̏����܂Ƃ߂��\���̂̏���

	//�w�i�摜�̕`��
	backGrround->Draw(&data);

	//���̈ʒu���w��
	data.pos.x = 200;
	data.pos.y = (float)(150 + (selectStage * 100));

	//���̉摜��`��
	arrow->Draw(&data);

	//����Ă���X�e�[�W�̐��������[�v
	for (int i = 0; i < 2; i++)
	{

		//�e�L�X�g�̈ʒu���w��
		data.pos.x = 300;
		data.pos.y = (float)(130 + (i * 100));

		//�e�L�X�g�`��
		text->Draw(&data);

		////�X�e�[�W�ԍ��̈ʒu���w��
		//data.pos.x = 1000;
		//data.pos.y = 250 + i * 200;
		//data.size.x = 140;
		//data.cut.x = 0 + i* 140;

		////�X�e�[�W�ԍ���`��
		//num->Draw(&data);
	}

	//�X�e�[�W�ԍ��̈ʒu���w��
	data.pos.x = 900;
	data.pos.y = 130;
	data.size.x = 140;
	data.cut.x = 140;

	//�X�e�[�W�ԍ���`��
	num->Draw(&data);

	////�X�e�[�W�ԍ��̈ʒu���w��
	data.pos.x = 900;
	data.pos.y = 230;
	data.size.x = 140;
	data.cut.x = 280;

	//�X�e�[�W�ԍ���`��
	num->Draw(&data);

	return S_OK;			//������Ԃ�
}
