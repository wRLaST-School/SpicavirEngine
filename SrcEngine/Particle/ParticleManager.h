#pragma once
#include <Essentials.h>
#include <SpTextureManager.h>
#include <IParticle.h>
#include <SpConstBuffer.h>

class ParticleManager
{
public:
	ParticleManager();
	void Draw();

	~ParticleManager() {
		// ƒ}ƒbƒv‚ð‰ðœ
		vertBuff->Unmap(0, nullptr);
	};

	struct Vertex {
		Float3 position;
		float scale;
		Float4 color;
	};

protected:
	const int vertexCount = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap = nullptr;

	int activeCount = 0;

	TextureKey texture = "notexture";

private:
	SpConstBuffer<Matrix> billboard;
};