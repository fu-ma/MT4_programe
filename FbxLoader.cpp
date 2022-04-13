#include "FbxLoader.h"
#include <cassert>

using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader *FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device *device)
{
    //�ď������`�F�b�N
    assert(fbxManager == nullptr);
    //�������烁���o�ϐ��ɑ��
    this->device = device;
    //FBX�}�l�[�W���̐���
    fbxManager = FbxManager::Create();
    //FBX�}�l�[�W���̓��o�͐ݒ�
    FbxIOSettings *ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    //FBX�C���|�[�^�̐���
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::LoadModelFromFile(const string &modelName)
{
    // ���f���Ɠ������O�̃t�H���_����ǂݍ���
    const string directoryPath = baseDirectory + modelName + "/";
    // �g���q,fbx��t��
    const string fileName = modelName + ".fbx";
    //�A�����ăt���p�X�𓾂�
    const string fullpath = directoryPath + fileName;

    //�t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
    {
        assert(0);
    }

    //�V�[������
    FbxScene *fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //�t�@�C�����烍�[�h����FBX�̏����V�[���ɃC���|�[�g
    fbxImporter->Import(fbxScene);

    //���f������
    FBXModel *model = new FBXModel();
    model->name = modelName;

    //FBX�m�[�h�̐����擾
    int nodeCount = fbxScene->GetNodeCount();
    //���炩���ߕK�v�����̃��������m�ۂ��邱�ƂŁA�A�h���X�������̂�\�h
    model->nodes.reserve(nodeCount);

    //���[�g�m�[�h���珇�ɉ�͂��ă��f���ɗ�������
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    //FBX�V�[���J��
    fbxScene->Destroy();
}

void FbxLoader::ParseNodeRecursive(FBXModel *model, FbxNode *fbxNode, Node *parent)
{
    //�m�[�h�����擾
    string name = fbxNode->GetName();

    //���f���Ƀm�[�h��ǉ�
    model->nodes.emplace_back();
    Node &node = model->nodes.back();

    //�m�[�h�����擾
    node.name = fbxNode->GetName();

    //FBX�m�[�h�̃��[�J���ړ����
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    //�`���ϊ����đ��
    node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
    node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
    node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };

    //��]�p��Degree�i�x�j���烉�W�A���ɕϊ�
    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    //�X�P�[���A��]�A���s�ړ��s��̌v�Z
    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    //���[�J���ό`�s��̌v�Z
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling; //���[���h�s��ɃX�P�[�����O�𔽉f
    node.transform *= matRotation; //���[���h�s��ɉ�]�𔽉f
    node.transform *= matTranslation; //���[���h�s��ɕ��s�ړ��𔽉f

    //�O���[�o���ό`�s��̌v�Z
    node.globalTransform = node.transform;
    if (parent)
    {
        node.parent = parent;
        //�e�̕ό`����Z
        node.globalTransform *= parent->globalTransform;
    }

    //�q�m�[�h�ɑ΂��čċA�Ăяo��
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }


}

void FbxLoader::Finalize()
{
    // �e��FBX�C���X�^���X�̔j��
    fbxImporter->Destroy();
    fbxManager->Destroy();
}