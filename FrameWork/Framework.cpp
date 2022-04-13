#include "Framework.h"

void Framework::GameRun()
{
	staticInit();
	while (true)//�Q�[�����[�v
	{
		if (SceneTime == 0)
		{
			Init();
		}
		if (Update() == false)
		{
			break;
		}
		if (SceneTime)
		{
			Draw();
		}
	}
	SceneDelete();
}

void Framework::staticInit()
{
	winApp = std::make_unique<WinApp>();
	common = std::make_unique<DirectXCommon>(winApp.get());
	winApp->Innitialize();
	common->Initialize();
	input = Input::GetInstance();
	input->Initialize(winApp->GetHInstance(), winApp->GetHwnd());
	Audio::Init();
	//���C�g�ÓI������
	LightGroup::StaticInitialize(common->GetDev().Get());
	/*�X�v���C�g������*/
	Sprite::StaticInitialize(common->GetDev().Get(), WinApp::window_width, WinApp::window_height);
	Sprite::LoadTexture(0, L"Resources/debugfont.png");
	debugText = DebugText::GetInstance();
	debugText->Initialize(0);

	/*FBX������*/
	FbxLoader::GetInstance()->Initialize(common->GetDev().Get());

	/*���f��������*/
	ModelObj::StaticInitialize(common->GetDev().Get());
	// �J��������
	camera = std::make_unique<DebugCamera>(WinApp::window_width, WinApp::window_height, input);
	// �p�[�e�B�N���}�l�[�W��������
	ParticleManager::GetInstance()->Initialize(common->GetDev().Get());
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera.get());
}

void Framework::Init()
{
}

bool Framework::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	input->Update();
	if (winApp->ProcessMessage() || input->isKeyTrigger(DIK_ESCAPE))
	{
		return false;
	}

	return true;
}

void Framework::Draw()
{
}
