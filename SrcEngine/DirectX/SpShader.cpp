#include "SpShader.h"
#include "GPipeline.h"

map<string, SpShader> shaderMap;

void SpShader::InitVS(const string& vsPath)
{
	string shaderFolder = "Resources/Shaders/";
	string vsPath_ = shaderFolder + vsPath;
	//�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	HRESULT result;
	result = D3DCompileFromFile(
		wstring(vsPath_.begin(), vsPath_.end()).c_str(),  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void SpShader::InitPS(const string& psPath)
{
	string shaderFolder = "Resources/Shaders/";
	string psPath_ = shaderFolder + psPath;
	//�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	HRESULT result;
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		wstring(psPath_.begin(), psPath_.end()).c_str(),   // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void SpShader::InitGS(const string& gsPath)
{
	string shaderFolder = "Resources/Shaders/";
	string gsPath_ = shaderFolder + gsPath;
	//�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	HRESULT result;
	result = D3DCompileFromFile(
		wstring(gsPath_.begin(), gsPath_.end()).c_str(),  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void RegisterShader(const string& id)
{
	shaderMap[id] = SpShader();
}

void InitGS(const string& id, const string& gsPath)
{
	shaderMap[id].InitGS(gsPath);
}

void InitVS(const string& id, const string& vsPath)
{
	shaderMap[id].InitVS(vsPath);
}

void InitPS(const string& id, const string& psPath)
{
	shaderMap[id].InitPS(psPath);
}

SpShader* GetShader(const string& id)
{
	return &shaderMap[id];
}
