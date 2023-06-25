#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>
#include "calc.h"

Player::~Player() {
	// 弾のデストラクタ
	for (PlayerBullet* bullet : bullets_) {

		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle,  Vector3 pos) {
	// ヌルじゃないか確認
	assert(model);
	// 外から貰ってきたデータの受け渡し
	model_ = model;
	textureHandle_ = textureHandle;

	//  ワールドトランスフォーム初期化(プレイヤーに移動するカメラ個体ごと)
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;  

	//3Dレティクルのワールドトランスフォームの初期化
	worldTransform3DReticle_.Initialize();
	// 外から受け取ったのdelete不可能

	// シングルトンインスタンス取得
	input_ = Input::GetInstance();
};

void Player::Update() {

#pragma region ですフラグでの弾の消滅

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}

		return false;
	});

#pragma endregion

	    Vector3 move = {};

	const float kCharactorspeed = 0.2f;

	// 入力処理
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharactorspeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharactorspeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharactorspeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharactorspeed;
	}

	// 回転
	const float kRotSpeed = 0.02f;

	// 押した方向回る
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	

#pragma region 移動の計算
	// 制限
	// 制限座標
	const float kMoveLimiteX = 20.0f;
	const float kMoveLimiteY = 20.0f;
	// 範囲処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimiteX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimiteX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimiteY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimiteY);

	// 移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	// 行列更新
	worldTransform_.UpdateMatrix();
#pragma endregion

	// 攻撃処理呼び出し
	Attack();

	// 自機のワールド座標から3Dレティクルのワールド座標を計算関数
	PtoReticleCalc();
	/* if (bullet_) {
	    // 弾が呼び出されている時に
	    bullet_->Update();
	}*/

	for (PlayerBullet* bullet : bullets_) {

		bullet->Update();
	}

#pragma region ImGui
	// 窓
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({300, 100});
	// プレイヤー座標表示
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
#pragma endregion

};

void Player::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textureHandle_);

	model_->Draw(worldTransform3DReticle_, viewprojection, textureHandle_);

	/* if (bullet_) {

	    bullet_->Draw(viewprojection);
	}*/

	for (PlayerBullet* bullet : bullets_) {

		bullet->Draw(viewprojection);
	}
};

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

		Vector3 ReticleVelocity;

		ReticleVelocity = Subtract(worldTransform3DReticle_.translation_,worldTransform_.translation_);
		ReticleVelocity = dir(ReticleVelocity.x, ReticleVelocity.y, ReticleVelocity.z);
		ReticleVelocity = Multiply(kBulletSpeed, ReticleVelocity);

		//worldTransform_.translation_ = GetWorldPosition();
		// 速度のベクトルを自機の向きに合わせて回転する
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		// bullet_ = newBullet;
		bullets_.push_back(newBullet);
	}
};

Vector3 Player::GetWorldPosition() {
	//ワールドを入れる奴
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

// 当たったことが伝わったらこっちで処理する関数
void Player::OnCollision() {
	//反応しない
}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ(プレイヤーの位置とカメラ)
	worldTransform_.parent_ = parent;
}

void Player::PtoReticleCalc()
{
	//自機から3Dレティクルの距離の変数
	const float kDistanceplayerTo3DReticle = 50.0f;

	//自機から3dレティクルへの補完(z向き)
	Vector3 offset = {0.0f,0.0f,1.0f};
	//自機の回転を反映(自機のワールド行列)
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = dir(offset.x, offset.y, offset.z);
	offset = Multiply(kDistanceplayerTo3DReticle, offset);
	
	//３Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = worldTransform_.translation_;
	worldTransform3DReticle_.translation_.z += offset.z;
	// 転送
	worldTransform3DReticle_.TransferMatrix();
	// 行列更新
	worldTransform3DReticle_.matWorld_ = MakeAffineMatrix(
	    worldTransform3DReticle_.scale_, worldTransform3DReticle_.rotation_,
	    worldTransform3DReticle_.translation_);

}
