#pragma once
#include <SpTextureManager.h>
#include <Anchor.h>
#include <Color.h>
class SpDS
{
public:
	static void DrawRotaGraph(int x, int y, float dx, float dy, float rot, TextureKey key, Anchor anchor = Anchor::Center);
	static void DrawBox(int x, int y, int width, int height, float rot, Color color, Anchor anchor = Anchor::Center);
	static void DrawCircleLine(int x, int y, int r, Color color, int edges = 100);
	static void DrawLine(int start, int end, Color color);

	static void CreateBuffers();
	static void Render();

private:
	const static int graphBuffNum = 2048;
	const static int lineBuffNum = 2048;

};

namespace SpDSLayouts {
	// DrawGraph用頂点レイアウト
	const static D3D12_INPUT_ELEMENT_DESC graphLayout[] = {
			{
				"FROM", 0,
				DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"TO", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
	};

	// DrawLine2D用頂点レイアウト
	const static D3D12_INPUT_ELEMENT_DESC graphLayout[] = {
			{
				"FROM", 0,
				DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"TO", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
	};
}
