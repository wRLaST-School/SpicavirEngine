#include "GameScene.h"

void GameScene::Init()
{
	wMat = Matrix::Identity();

	for (int i = 0; i < 1; i++)
	{
		objects.push_back(Object3D());
	}

	for (auto itr = objects.begin(); itr != objects.end(); itr++)
	{
		itr->model = &model;
	}
}

void GameScene::Update()
{
	Matrix pMat = Matrix::Projection(
		PI / 4,
		(float)GetwWindow()->width / GetwWindow()->height,
		0.1f, 1000.0f
	);

	Matrix vMat;
	Float3 eye{ 0, 0, -300 };
	Float3 target{ 0, 0, 0 };
	Float3 up{ 0, 1.0f, 0 };

	vMat = Matrix::ViewLookAt(eye, target, up);

	objects.begin()->rotation.x += 0.01;
	objects.begin()->UpdateMatrix();

	vproj = vMat * pMat;
}

void GameScene::Draw()
{
	for (auto itr = objects.begin(); itr != objects.end(); itr++)
	{
		itr->Draw(vproj);
	}
}
