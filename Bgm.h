#include "UnitBase.h"
#include "Audio.h"

#pragma once

class Bgm : public UnitBase
{
	//�I�[�f�B�I�N���X�̃I�u�W�F�N�g�쐬
	Audio* audio;

public:
	Bgm();
	~Bgm();

	HRESULT Load();
	void	Sound();
};

