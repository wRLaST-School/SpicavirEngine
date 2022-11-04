#include "SpShader.h"
#include "GPipeline.h"

map<string, SpShader> shaderMap;

void SpShader::InitVS(string vsPath)
{
	string shaderFolder = "Resources/Shaders/";
	vsPath = shaderFolder + vsPath;
	//シェーダーの読み込みとコンパイル
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	HRESULT result;
	result = D3DCompileFromFile(
		wstring(vsPath.begin(), vsPath.end()).c_str(),  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void SpShader::InitPS(string psPath)
{
	string shaderFolder = "Resources/Shaders/";
	psPath = shaderFolder + psPath;
	//シェーダーの読み込みとコンパイル
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	HRESULT result;
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		wstring(psPath.begin(), psPath.end()).c_str(),   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void SpShader::InitGS(string gsPath)
{
	string shaderFolder = "Resources/Shaders/";
	gsPath = shaderFolder + gsPath;
	//シェーダーの読み込みとコンパイル
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	HRESULT result;
	result = D3DCompileFromFile(
		wstring(gsPath.begin(), gsPath.end()).c_str(),  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void RegisterShader(string id)
{
	shaderMap[id] = SpShader();
}

void InitGS(string id, string gsPath)
{
	shaderMap[id].InitGS(gsPath);
}

void InitVS(string id, string vsPath)
{
	shaderMap[id].InitVS(vsPath);
}

void InitPS(string id, string psPath)
{
	shaderMap[id].InitPS(psPath);
}

SpShader* GetShader(string id)
{
	return &shaderMap[id];
}
