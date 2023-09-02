#pragma once
#include "Essentials.h"
#include "SpShader.h"
//TODO:ê›åvÇ™ÉKÉoÇ∑Ç¨ÇÈÇÃÇ≈pso,rootsignatureÇÃê›åvÇå©íºÇ∑
struct PipelineDesc
{
	struct Shader {
		SpShader* pShader;
	} Shader;

	struct Blend {
		UINT8 RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		bool BlendEnable = true;

		struct BlendDesc {
			D3D12_BLEND_OP BlendOpAlpha = D3D12_BLEND_OP_ADD;
			D3D12_BLEND SrcBlendAlpha = D3D12_BLEND_ONE;
			D3D12_BLEND DestBlendAlpha = D3D12_BLEND_ONE;

			D3D12_BLEND_OP BlendOp = D3D12_BLEND_OP_ADD;
			D3D12_BLEND SrcBlend = D3D12_BLEND_SRC_ALPHA;
			D3D12_BLEND DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		} Desc;
	} Blend[8];

	struct Render {
		D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		UINT SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		D3D12_RASTERIZER_DESC RasterizerState = D3D12_RASTERIZER_DESC{
			D3D12_FILL_MODE_SOLID,
			D3D12_CULL_MODE_BACK,
			0,
			0,
			0.f,
			0.f,
			true,
			0,
			0,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

		UINT NumRenderTargets = 1;
		DXGI_FORMAT RTVFormat[8] = { DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN };
		DXGI_SAMPLE_DESC SampleDesc = DXGI_SAMPLE_DESC{
			1,
			0
		};

		//èâä˙âªïKê{
		D3D12_INPUT_LAYOUT_DESC InputLayout;
	} Render;

	struct Depth {
		D3D12_DEPTH_STENCIL_DESC DepthStencilState = D3D12_DEPTH_STENCIL_DESC{
			true,
			D3D12_DEPTH_WRITE_MASK_ALL,
			D3D12_COMPARISON_FUNC_LESS,
			false,
			0,
			0,
			D3D12_DEPTH_STENCILOP_DESC{},
			D3D12_DEPTH_STENCILOP_DESC{}
		};
		DXGI_FORMAT DSVFormat = DXGI_FORMAT_D32_FLOAT;
	} Depth;

	struct RootSignature {
		//èâä˙âªïKê{
		ID3D12RootSignature* ptr; 
	} RootSignature;
};

namespace PipelineUtil
{
	enum class BlendMode
	{
		Alpha,
		Add,
		Sub,
		Inv
	};

	class Blend {
	public:
		static PipelineDesc::Blend::BlendDesc GetBlendMode(BlendMode blendMode);
	};
};

class GPipeline
{
public:
	static void Create(const PipelineDesc& desc, const std::string& id);
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetDesc(const std::string& id);
	static ID3D12PipelineState* GetState(const std::string& id);
	static GPipeline* GetGPipeline(const std::string& id);

	GPipeline() {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psod{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pso = nullptr;

private:
	void Create();

private:
	void InitDesc(const PipelineDesc& desc);

};



