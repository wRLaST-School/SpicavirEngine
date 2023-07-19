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
	const int32_t vertexCount = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap = nullptr;

	int32_t activeCount = 0;

	TextureKey texture = "notexture";

private:
};