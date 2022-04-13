#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include "FBXModel.h"

class FbxLoader
{
private: //�G�C���A�X
	//std::���ȗ�
	using string = std::string;

public: //�萔
	// ���f���i�[���[�g�p�X
	static const string baseDirectory;
	//�ÓI�����o�ϐ��̎���
	static const std::string baseDirectory;
public:
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FbxLoader *GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">D3D12�f�o�C�X</param>
	void Initialize(ID3D12Device *device);

	/// <summary>
	/// �t�@�C������FBX���f���ǂݍ���
	/// <summary>
	/// <param name="modelName">���f����</param>
	void LoadModelFromFile(const string &modelName);

	/// <summary>
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	/// <param name="model">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	/// <param name="parent">�e�m�[�h</param>
	void ParseNodeRecursive(FBXModel *model, FbxNode *fbxNode, Node *parent = nullptr);

	/// ��n��
	/// </summary>
	void Finalize();

private:
	// D3D12�f�o�C�X
	ID3D12Device *device = nullptr;
	//FBX�}�l�[�W��
	FbxManager *fbxManager = nullptr;
	//FBX�C���|�[�^
	FbxImporter *fbxImporter = nullptr;

	// private�ȃR���X�g���N�^�i�V���O���g���p�^�[���j
	FbxLoader() = default;
	// private�ȃf�X�g���N�^�i�V���O���g���p�^�[���j
	~FbxLoader() = default;
	// �R�s�[�R���X�g���N�^���֎~�i�V���O���g���p�^�[���j
	FbxLoader(const FbxLoader &obj) = delete;
	// �R�s�[������Z�q���֎~�i�V���O���g���p�^�[���j
	void operator=(const FbxLoader &obj) = delete;
};