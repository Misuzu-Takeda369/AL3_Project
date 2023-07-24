#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	// 解放
	delete player_;
	delete enemy_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");

	// モデルを作りだす
	model_ = Model::Create();

	// ビュープロジェクション初期化(ゲーム共通カメラ)
	viewProjection_.Initialize();
	// 自キャラを作る(ゲーム上に写るようにする)
	player_ = new Player();
	enemy_ = new Enemy();
	// 初期化
	// GameSceneの方でモデル読み込んでいるため
	player_->Initialize(model_, textureHandle_);
	enemy_->Initialize(model_);

	enemy_->SetPlayer(player_);

	// カメラ(ウィンドウの大きさにする)
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	//
	AxisIndicator::GetInstance()->SetVisible(true);
	//
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	player_->Update();
	enemy_->Update();

	//当たり判定関連
	CheckAllCollisions();

	// スペースと＿が必要
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
	}
#endif // DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


void GameScene::CheckAllCollisions() {
	

	// playerの弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	// enemyの弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullet();

	//コライダーの変数
	std::list<Collider*> colliders_;
	//コライダーをリストに入れる
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);

	// 自分の弾すべて
	for (PlayerBullet* bulletP : playerBullets) {

		colliders_.push_back(bulletP);
	}
	//敵の弾のすべて
	for (EnemyBullet* bulletE : enemyBullets) {

		colliders_.push_back(bulletE);
	}

	//リスト内のすべてを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++ itrA) {
		//イテレータ(反復子)AからコライダーAを貰う
		Collider* A_ = *itrA;
		//イテレータBもAの要素から作る
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
		
			// イテレータ(反復子)BからコライダーBを貰う
			Collider* B_ = *itrB;

			//コライダーAとコライダーBの当たり判定をする
			CheckCollisionPair(A_, B_);

			//枠組み作らなくてもいいのは楽だがすぐ消えちゃう(分別が出来てない？)
		}
	}

	
}


void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) 
{
#pragma region 属性判定

	//属性フィルター
	if ((colliderA->GetCollistionAttribute() != colliderB->GetCollistionMask()) ||
	    (colliderB->GetCollistionAttribute() != colliderA->GetCollistionMask())) {

		return;
	}
#pragma endregion

#pragma region 当たり判定
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	// 衝突判定
	float MixAB = ((posB.x - posA.x) * (posB.x - posA.x)) +
	              ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z));

	// 半径(仮)
	const float RadishMix = 20.0f;
	// 当たってるか否か
	if (MixAB <= RadishMix) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
	
	
#pragma endregion

}
