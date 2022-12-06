#pragma once
#include <Essentials.h>

class Line 
{
public:
	Line(Float3 start, Float3 end, Float4 color) : start(start), end(end), color(color) {}
	Float3 start;
	Float3 end;

	Float4 color;
};

class LineDrawer
{
public:
	static void Init();

	static void DrawLine(Float3 start, Float3 end, Float4 color);
	static void DrawLines(vector<Float3> positions, Float4 color);
	static void DrawCube(Float3 center, Float3 scale, Float4 color);
	static void DrawRotaCube(Float3 center, Float3 scale, Float3 rot, Float4 color);

	static void DrawAllLines();

	static void ClearLineData();
private:
	static vector<Line> lines;

	static const int maxLines = 1024;

	static D3D12_VERTEX_BUFFER_VIEW vbView;
	static ComPtr<ID3D12Resource> vertBuff;
	static Line* vertMap;
};

namespace LineDrawerCommon {
	// 頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
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

