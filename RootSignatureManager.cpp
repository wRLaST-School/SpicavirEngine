#include "stdafx.h"
#include "RootSignatureManager.h"
#include <SpRootSignature.h>

void RootSignatureManager::RegisterAllRS()
{
#pragma region 3D Default RS
	{
		SpRootSignature* rs3d = SpRootSignature::Register("3D");

		rs3d->UseDefaultSettings();

		D3D12_DESCRIPTOR_RANGE descRange{};
		descRange.NumDescriptors = 1;
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.BaseShaderRegister = 0;
		descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//定数バッファ0番マテリアル
		rs3d->params.emplace_back();
		rs3d->params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rs3d->params[0].Descriptor.ShaderRegister = 0;
		rs3d->params[0].Descriptor.RegisterSpace = 0;
		rs3d->params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rs3d->params.emplace_back();
		rs3d->params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rs3d->params[1].DescriptorTable.pDescriptorRanges = &descRange;
		rs3d->params[1].DescriptorTable.NumDescriptorRanges = 1;
		rs3d->params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//定数バッファ1番ワールド行列
		rs3d->params.emplace_back();
		rs3d->params[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rs3d->params[2].Descriptor.ShaderRegister = 1;
		rs3d->params[2].Descriptor.RegisterSpace = 0;
		rs3d->params[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//定数バッファ2番ビュー・射影変換行列
		rs3d->params.emplace_back();
		rs3d->params[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rs3d->params[3].Descriptor.ShaderRegister = 2;
		rs3d->params[3].Descriptor.RegisterSpace = 0;
		rs3d->params[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//定数バッファ3番輝度
		rs3d->params.emplace_back();
		rs3d->params[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rs3d->params[4].Descriptor.ShaderRegister = 3;
		rs3d->params[4].Descriptor.RegisterSpace = 0;
		rs3d->params[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//定数バッファ4番光源
		rs3d->params.emplace_back();
		rs3d->params[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rs3d->params[5].Descriptor.ShaderRegister = 4;
		rs3d->params[5].Descriptor.RegisterSpace = 0;
		rs3d->params[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		rs3d->Create();
	}
#pragma endregion

#pragma region 3D Particle
	{
		SpRootSignature* rsparticle = SpRootSignature::Register("Particle");

		rsparticle->UseDefaultSettings();

		D3D12_DESCRIPTOR_RANGE descRange{};
		descRange.NumDescriptors = 1;
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.BaseShaderRegister = 0;
		descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//定数バッファ0番マテリアル
		rsparticle->params.emplace_back();
		rsparticle->params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rsparticle->params[0].Descriptor.ShaderRegister = 0;
		rsparticle->params[0].Descriptor.RegisterSpace = 0;
		rsparticle->params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rsparticle->params.emplace_back();
		rsparticle->params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rsparticle->params[1].DescriptorTable.pDescriptorRanges = &descRange;
		rsparticle->params[1].DescriptorTable.NumDescriptorRanges = 1;
		rsparticle->params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


		//定数バッファ1番輝度
		rsparticle->params.emplace_back();
		rsparticle->params[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rsparticle->params[2].Descriptor.ShaderRegister = 1;
		rsparticle->params[2].Descriptor.RegisterSpace = 0;
		rsparticle->params[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//定数バッファ2番ビュー・射影変換行列
		rsparticle->params.emplace_back();
		rsparticle->params[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rsparticle->params[3].Descriptor.ShaderRegister = 2;
		rsparticle->params[3].Descriptor.RegisterSpace = 0;
		rsparticle->params[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		rsparticle->Create();
	}
#pragma endregion

#pragma region 2D Default RS
	{
		SpRootSignature* rs2d = SpRootSignature::Register("2D");

		rs2d->UseDefaultSettings();

		D3D12_DESCRIPTOR_RANGE descRange{};
		descRange.NumDescriptors = 1;
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.BaseShaderRegister = 0;
		descRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//定数バッファ0番スプライト用
		rs2d->params.emplace_back();
		rs2d->params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rs2d->params[0].Descriptor.ShaderRegister = 0;
		rs2d->params[0].Descriptor.RegisterSpace = 0;
		rs2d->params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//テクスチャレジスタ0番
		rs2d->params.emplace_back();
		rs2d->params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rs2d->params[1].DescriptorTable.pDescriptorRanges = &descRange;
		rs2d->params[1].DescriptorTable.NumDescriptorRanges = 1;
		rs2d->params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		rs2d->Create();
	}
#pragma endregion

}
