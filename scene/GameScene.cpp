#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	// 解放
	delete player_;

	for (Enemy* enemy : enemies_) {
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

	//レティクルの画像読み込む
	TextureManager::Load("target.png");

	// モデルを作りだす
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ビュープロジェクション初期化(ゲーム共通カメラ)
	// 穴あきを防ぐ為の奴
	viewProjection_.farZ = 200;
	viewProjection_.Initialize();
	// 自キャラを作る(ゲーム上に写るようにする)
	player_ = new Player();

	skydome_ = new Skydome();

	// レールカメラのインクリース的な名前の奴
	railCamera_ = new RailCamera();

	// 敵の初期化関連
	LoadEnemyPopDate();

	// ここにロード関連をぶつける
	// AddEnemy({10.0f,0.0f,50.0f});

	// 初期化
	// GameSceneの方でモデル読み込んでいるため
	// 自キャラとレールカメラの親子関係を結ぶ
	Vector3 playerPosition(0, 0, 5);
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetParent(&railCamera_->GetWorldProjection());

	skydome_->Initialize(modelSkydome_);
	// プレイヤーの位置？を代入する？
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});

	for (Enemy* enemy : enemies_) {
		enemy->SetPlayer(player_);
		// 現在のゲームシーン
		enemy->SetGameScene(this);
	}

	// enemy->SetPlayer(player_);
	// enemy->SetGameScene(this);

	// カメラ(ウィンドウの大きさにする)
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	//
	AxisIndicator::GetInstance()->SetVisible(true);
	//
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	player_->Update();
	// enemy_->Update();
	// 敵の描写
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	UpdateEnemyPopCommands();
#pragma region ですフラグでの敵の消滅

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}

		return false;
	});

#pragma endregion
	skydome_->Update();

	railCamera_->Update();

	// 敵の弾の描写
	for (EnemyBullet* bullet : enemybullets_) {
		bullet->Update();
	}

	// 当たり判定
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

	// enemy_->Draw(viewProjection_);

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
	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 2つのぶつの座標
	Vector3 posA, posB;

	// playerの弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();
	// enemyの弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = GetBullet();

#pragma region プレイヤーと敵の弾当たり判定
	// 自キャラ座標
	posA = player_->GetWorldPosition();

	// プレイヤーと弾すべての当たり判定を判断する
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
			player_->OnCollision();

			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region プレイヤー弾と敵当たり判定
	// 自キャラ座標

	for (Enemy* enemy : enemies_) {

		posA = enemy->GetWorldPosition();

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
				enemy->OnCollision();

				bullet->OnCollision();
			}
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リスト
	enemybullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopDate() {
	// fileを開く
	std::ifstream enemyMovefile;
	enemyMovefile.open("Resources/EnemyPop.csv");
	// エラーの確認
	assert(enemyMovefile.is_open());

	// fileの生身を敵発生コマンドにコピー
	enemyPopCommands << enemyMovefile.rdbuf();

	// fileを閉じる
	enemyMovefile.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (waitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機が終わったよ
			waitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行(1セットの最後までループ)
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列を解析しやすくする
		std::istringstream line_stream(line);

		// カンマ区切りで先頭の文字列をとってくる(カンマとカンマの間の文字をとってくるイメージ)
		std::string word;
		getline(line_stream, word, ',');

		// 　//から始まる行はコメントなのでとばすよーっていう処理を挟み込む
		if (word.find("//") == 0) {
			continue;
		}

		// POPという文字列が最初にいた場合下記のif文を処理する
		if (word.find("POP") == 0) {
			// x座標 (カンマ区切りで拾ってくる)
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標 (カンマ区切りで拾ってくる)
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標 (カンマ区切りで拾ってくる)
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 上で拾ってきた値を敵に入れる処理
			AddEnemy({x, y, z});

		}

		// WAITから始まる文字列が最初にいた場合下記のif文を処理する
		else if (word.find("WAIT") == 0) {

			// 文字列拾ってくる
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始

			waitFlag = true;
			waitTimer = waitTime;

			// ループを抜ける(POP、WAITで1セットでwaitの最後まで行ったら抜ける)
			break;
		}
	}
}

void GameScene::AddEnemy(Vector3 pos) {
	// 初期化処理関連
	Enemy* enemy = new Enemy();
	// 多分敵の初期化処理書き直し
	enemy->Initialize(model_, pos);
	enemy->SetPlayer(player_);
	// 現在のゲームシーン
	enemy->SetGameScene(this);
	// enemy.push_buck(enemy);

	// プッシュ
	enemies_.push_back(enemy);
}
