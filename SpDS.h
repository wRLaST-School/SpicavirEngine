#pragma once
#include <SpTextureManager.h>
#include <Anchor.h>
#include <Color.h>
#include <SpConstBuffer.h>
class SpDS
{
public:
	//��`����Ă��ꂼ��ŕ`��R�}���h���L���[����
	static void DrawRotaGraph(int x, int y, float dx, float dy, float rot, TextureKey key, Anchor anchor = Anchor::Center);
	static void DrawBox(int x, int y, int width, int height, float rot, Color color, Anchor anchor = Anchor::Center);
	static void DrawBoxLine(int x, int y, int width, int height, float rot, Color color, Anchor anchor = Anchor::Center);
	static void DrawCircleLine(int x, int y, int r, Color color, int edges = 100);
	static void DrawLine(int start, int end, Color color);

	//�e�o�b�t�@�Ƒ�ʂ̃_�~�[�X�v���C�g������
	static void CreateBuffers();
	static void Render();

private:
	const static int graphBuffNum = 2048;
	const static int lineBuffNum = 2048;

	static void RenderGraph();

	struct Line {
		Float3 start;
		Float3 end;
		Color color;
	};

	struct Graph {
		Matrix wMat;
		TextureKey key;
	};

	struct GraphGPUData {
		SpConstBuffer<Matrix> matcb;
		bool used = false;
		TextureKey key;
	};

	static vector<Line> lines;

	static D3D12_VERTEX_BUFFER_VIEW vbView;
	static ComPtr<ID3D12Resource> vertBuff;
	static Line* vertMap;

	static vector<Graph> graphs;
	static GraphGPUData ggpu;
	static D3D12_VERTEX_BUFFER_VIEW gvbView;
	static ComPtr<ID3D12Resource> gvertBuff;
	static float* gvertMap;
};

namespace SpDSLayouts {
	const static D3D12_INPUT_ELEMENT_DESC graphLayout[] = {
		{
				"DUMMY", 0,
				DXGI_FORMAT_R32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// DrawLine2D�p���_���C�A�E�g
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
