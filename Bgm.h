#include "UnitBase.h"
#include "Audio.h"

#pragma once

class Bgm : public UnitBase
{
	//オーディオクラスのオブジェクト作成
	Audio* audio;

public:
	Bgm();
	~Bgm();

	HRESULT Load();
	void	Sound();
};

