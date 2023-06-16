#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	// 解放
	delete player_;
	for (Enemy* enemy: enemies_) {
		delete enemy;
	}
	delete skydome_;
	delete model_;
	delete debugCamera_;
	delete modelSkydome_;
	delete railCamera_;

	for (EnemyBullet* bullet : enemybullets_) {

		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");

	// モデルを作りだす
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ビュープロジェクション初期化(ゲーム共通カメラ)
	//穴あきを防ぐ為の奴
	viewProjection_.farZ = 200;
	viewProjection_.Initialize();
	// 自キャラを作る(ゲーム上に写るようにする)
	player_ = new Player();
	for (Enemy* enemy : enemies_) {
		enemy = new Enemy();
	}
	skydome_ = new Skydome();

	//レールカメラのインクリース的な名前の奴
	railCamera_ = new RailCamera();


	// 初期化
	// GameSceneの方でモデル読み込んでいるため
	// 自キャラとレールカメラの親子関係を結ぶ
	Vector3 playerPosition(0,0,5);
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetParent(&railCamera_->GetWorldProjection());

	//enemy_->Initialize(model_);
	for (Enemy* enemy : enemies_) {
		enemy->Initialize(model_);
	}

	skydome_->Initialize(modelSkydome_);
	//プレイヤーの位置？を代入する？
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f,0.0f,0.0f});

	for (Enemy* enemy : enemies_) {
		enemy->SetPlayer(player_);
		enemy->SetGameScene(this);
	}

	//enemy->SetPlayer(player_);
	//enemy->SetGameScene(this);


	// カメラ(ウィンドウの大きさにする)
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	//
	AxisIndicator::GetInstance()->SetVisible(true);
	//
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	player_->Update();
	//enemy_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	skydome_->Update();

	railCamera_->Update();

	for (EnemyBullet* bullet : enemybullets_) {

		bullet->Update();
	}

	//当たり判定
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
	} 
	else {
		// 描写に反映
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
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

	//enemy_->Draw(viewProjection_);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	skydome_->Draw(viewProjection_);

	railCamera_->Draw();

	for (EnemyBullet* bullet : enemybullets_) {

		bullet->Draw(viewProjection_);
	}
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
	//2つのぶつの座標
	Vector3 posA, posB;

	//playerの弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	//enemyの弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = GetBullet();

#pragma region プレイヤーと敵の弾当たり判定
	// 自キャラ座標
	posA = player_->GetWorldPosition();

	// プレイヤーと弾すべての当たり判定を判断する
	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();


		// 衝突判定
		float MixAB = ((posB.x - posA.x) * (posB.x - posA.x)) 
			+((posB.y - posA.y) * (posB.y - posA.y))
			+((posB.z - posA.z) * (posB.z - posA.z));
		
		// 半径(仮)
		float RadishMix = 20.0f;
		// 当たってるか否か
		if (MixAB <= RadishMix) {
			player_->OnCollision();

			bullet->OnCollision();
		}

	}
#pragma endregion

#pragma region プレイヤー弾と敵当たり判定
	// 自キャラ座標
	posA = enemy_->GetWorldPosition();

	// プレイヤーと弾すべての当たり判定を判断する
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();

		// 衝突判定
		float MixAB = ((posB.x - posA.x) * (posB.x - posA.x)) +
		              ((posB.y - posA.y) * (posB.y - posA.y)) +
		              ((posB.z - posA.z) * (posB.z - posA.z));

		// 半径(仮)
		float RadishMix = 20.0f;
		// 当たってるか否か
		if (MixAB <= RadishMix) {
			enemy_->OnCollision();

			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region プレイヤー弾と敵の弾当たり判定
	
	for (PlayerBullet* bulletP : playerBullets) {
	
		posA = bulletP->GetWorldPosition();
		for (EnemyBullet* bullet : enemyBullets) {
			posB = bullet->GetWorldPosition();

			// 衝突判定
			float MixAB = ((posB.x - posA.x) * (posB.x - posA.x)) +
			              ((posB.y - posA.y) * (posB.y - posA.y)) +
			              ((posB.z - posA.z) * (posB.z - posA.z));

			// 半径(仮)
			float RadishMix = 20.0f;
			// 当たってるか否か
			if (MixAB <= RadishMix) {
				bulletP->OnCollision();

				bullet->OnCollision();
			}

		}
	}
#pragma endregion

}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) 
{
	//リスト
	enemybullets_.push_back(enemyBullet);
}
