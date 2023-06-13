#pragma once
#include <Essentials.h>

class Line 
{
public:
	Line(const Float3& start, const Float3& end, const Float4& color) : start(start), end(end), color(color) {}
	Float3 start;
	Float3 end;

	Float4 color;
};

class LineDrawer
{
public:
	static void Init();

	static void DrawLine(const Float3& start, const Float3& end, const Float4& color);
	static void DrawLines(const vector<Float3>& positions, const Float4& color);
	static void DrawCube(const Float3& center, const Float3& scale, const Float4& color);
	static void DrawRotaCube(const Float3& center, const Float3& scale, const Float3& rot, const Float4& color);

	static void DrawAllLines();

	static void ClearLineData();
private:
	static vector<Line> lines;

	static const int32_t maxLines = 1024;

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

