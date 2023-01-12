#include "stdafx.h"
#include "Player.h"
#include <Input.h>
#ifdef _DEBUG
#include <SpImGui.h>
#endif // DEBUG
#include <Util.h>
#include <SceneManager.h>
#include <ResultScene.h>
#include <BossHP.h>

void Player::Update()
{
	rotation.x += (Input::Key::Down(DIK_DOWN) - Input::Key::Down(DIK_UP) - (Input::Pad::GetRStick().y)) * PIf / 120;
	rotation.y += (Input::Key::Down(DIK_RIGHT) - Input::Key::Down(DIK_LEFT) + (Input::Pad::GetRStick().x)) * PIf / 120;
	Matrix cm = Matrix::RotRollPitchYaw(rotation);
	Vec3 front = cm.ExtractAxisZ();
	Vec3 right = cm.ExtractAxisX();
	front.y = 0;
	right.y = 0;

	Vec3 up = cm.ExtractAxisY();

	Vec3 move =
		front.SetLength((Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S) + (Input::Pad::GetLStick()).y)) +
		right.SetLength((Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A) + (Input::Pad::GetLStick()).x));
	move.y = 0;
	if (move.x || move.z)
	{
		move.SetLength(speed);
	}
	position = move + position;

	position.x = Util::Clamp(position.x, -16.0f + scale.x, 16.0f - scale.x);
	position.z = Util::Clamp(position.z, -16.0f + scale.z, 16.0f - scale.z);
	position.y = scale.y;

	if (Input::Key::Triggered(DIK_LSHIFT) || Input::Pad::Triggered(Button::R) || Input::Pad::Triggered(Button::L))
	{
		Dodge();
	}
	if (usingFlash)
	{
		dodgeTimer++;

		immune = dodgeTimer < dodgeTime;

		usingFlash = dodgeTimer < dodgeTime + flashCD;
	}

	if (immune)
	{
		position = dodgeVel + position;
	}

	UpdateMatrix();
	
	successTimer++;
	if (successTime - successTimer > 0)
	{
		*brightnessCB.contents = { 10.f, 10.0f, 10.f, 1.0f };
	}
	else
	{
		*brightnessCB.contents = { 1.f, 1.f, 1.f, 1.0f };
	}

	em.position = this->position;
	em.active = immune;
	em.Update();
}

void Player::Draw()
{
	Object3D::Draw();

#ifdef _DEBUG
	SpImGui::Command([&]() {
		if (ImGui::Begin("Player"))
		{
			ImGui::SliderFloat3("Position", &position.x, -30.f, 30.f);
			ImGui::SliderFloat3("Scale", &scale.x, 0.f, 1.f);
			ImGui::SliderFloat3("Rotation", &rotation.x, 0.f, 1.f);
			ImGui::InputInt("Health", &health);

			ImGui::End();
		}
	});
#endif // DEBUG

	em.Draw();
}

void Player::Dodge()
{
	if (!usingFlash)
	{
		SoundManager::Play("Dodge");
		usingFlash = true; 
		Matrix cm = Matrix::RotRollPitchYaw(rotation);
		Vec3 front = cm.ExtractAxisZ();
		Vec3 right = cm.ExtractAxisX();
		front.y = 0;
		right.y = 0;

		Vec3 up = cm.ExtractAxisY();

		Vec3 move =
			front.SetLength((Input::Key::Down(DIK_W) - Input::Key::Down(DIK_S) + (Input::Pad::GetLStick()).y)) +
			right.SetLength((Input::Key::Down(DIK_D) - Input::Key::Down(DIK_A) + (Input::Pad::GetLStick()).x));
		move.y = 0;
		if (move.x || move.z)
		{
			move.SetLength(dodgespd);
		}

		dodgeVel = move;
		dodgeTimer = 0;
	}
}

Player* Player::Get()
{
	return current;
}

void Player::Set(Player* p)
{
	current = p;
}

void Player::Damage()
{
	if (immune)
	{
		successTimer = 0;
		BossHP::hp -= (1 * 60);
		SoundManager::Play("counterSuccess");
		return;
	}

	SoundManager::Play("PlayerDamaged");
	health--;

	if (health <= 0)
	{
		SceneManager::LoadScene<ResultScene>();
	}
}

Player* Player::current = nullptr;