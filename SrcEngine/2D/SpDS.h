#pragma once
#include <SpTextureManager.h>
#include <Anchor.h>
#include <Color.h>
#include <SpConstBuffer.h>
class SpDS
{
public:
	enum class Blend {
		Alpha,
		Add,
		Sub
	};

	static DLLExport void DrawRotaGraph(int32_t x, int32_t y, float dx, float dy, float rot,
		TextureKey key, Anchor anchor,
		Color brightness = Color(0xffffff));

	static DLLExport void DrawRotaGraph(int32_t x, int32_t y, float dx, float dy, float rot,
		TextureKey key);

	static DLLExport void DrawBox(int32_t x, int32_t y, int32_t width, int32_t height, float rot,
		const Color& color, const Anchor& anchor = Anchor::Center);

	static DLLExport void DrawBox(int32_t x0, int32_t y0, int32_t x1, int32_t y1, const Color& color);

	static DLLExport void SetBlendMode(const Blend& blendMode);
	static DLLExport void SetRenderTarget(const TextureKey& key);
	static DLLExport void SetPreDrawFunc(std::function<void(void)> prop);

	static DLLExport void DrawBoxLine(int32_t x, int32_t y, int32_t width, int32_t height,
		const Color& color, float thickness, const  Anchor& anchor = Anchor::Center);

	static DLLExport void DrawLine(int32_t startX, int32_t startY, int32_t endX, int32_t endY,
		const Color& color, int32_t thickness = 1);

	static DLLExport void DrawCircleLine(int32_t x, int32_t y, int32_t r, Color color, int32_t edges = 100);
	//各バッファと大量のダミースプライト初期化
	static void CreateBuffers();
	static void Render();

private:
	const static int32_t GRAPH_BUFF_NUM = 4096;
	const static int32_t LINE_BUFF_NUM = 2048;

	static void RenderGraph();

	struct Line {
		Float3 start;
		Float3 end;
		Color color;
	};

	struct Graph {
		Matrix wMat;
		TextureKey key;
		Color brightness;
		bool isRenderTarget = false;
	};

	struct GraphCBData {
		Float4 brightness;
		Matrix mat;
	};

	struct GraphGPUData {
		SpConstBuffer<GraphCBData> matcb;
		bool used = false;
		TextureKey key;
	};

	struct GraphVertData {
		Float3 pos;
		Float2 uv;
	};

	static eastl::vector<Line> sLines;

	/*static D3D12_VERTEX_BUFFER_VIEW vbView;
	static ComPtr<ID3D12Resource> vertBuff;*/
	//static Line* vertMap;

	static eastl::vector<Graph> sGraphs;
	static eastl::list<GraphGPUData> sGgpu;
	static D3D12_VERTEX_BUFFER_VIEW sGvbView;
	static Microsoft::WRL::ComPtr<ID3D12Resource> sGvertBuff;
	static int32_t sGraphCount;
	static eastl::multimap<int32_t, std::function<void(void)>> sCommands;
};

namespace SpDSLayouts {
	const static D3D12_INPUT_ELEMENT_DESC graphLayout[] = {
		{
				"POSITION", 0,
				DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
				"TEXCOORD", 0,
				DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// DrawLine2D用頂点レイアウト
	const static D3D12_INPUT_ELEMENT_DESC lineLayout[] = {
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
