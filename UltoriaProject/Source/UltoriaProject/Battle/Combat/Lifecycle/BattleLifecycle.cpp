// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleLifecycle.h"
#include "../Turn/BattleTurnManager.h"
#include "../../BattleManager.h"
#include "../../BattleAreaManager.h"
#include "../../BattleGaugeManager.h"
#include "../../Encounter/EncounterManager.h"
#include "../../../Character/Ally/MainPlayer.h"
#include "../../../Character/Enemy/EnemyBase.h"
#include "../../../System/Controller/PlayPlayerController.h"
#include "../../../UI/Battle/BattleUIManager.h"
#include "../../../Camera/CameraManager.h"
#include "../../../Stage/StageManager.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Engine/Level.h"

void UBattleLifecycle::Start(UBattleManager* mg, AEnemyBase* enemyBase)
{
	if (!m_BattleManager) m_BattleManager = mg;

	// 入力モード切り替え
	ChangeInputMode(PlayInputMode::BATTLE);

	if (enemyBase)
	{
		// パーティー作成
		CreateParty(enemyBase);
		
		// BattleAreaManagerを取得
		m_AreaManager = GetBattleArea();

		if (!m_AreaManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("nullpttr"));
		}

		// カメラを切り替える
		GetCameraManager()->ChangeBattleCamera(EBattleCameraType::MAIN);
		// 座標を設定
		SetBattleCharacterPos(m_AreaManager);
	}

	// UIを生成
	CreateBattleUI();

	//// 各ゲージの更新
	//UBattleGaugeManager* gaugeManager = GetWorld()->GetSubsystem<UBattleGaugeManager>();
	//if (gaugeManager)
	//{
	//	gaugeManager->UpdateGauge();
	//}
}

void UBattleLifecycle::End()
{
	// UIを削除
	RemoveBattleUI();

	// 入力モード切り替え
	ChangeInputMode(PlayInputMode::FIELD);

	GetCameraManager()->ChangeFieldCamera();
	SetFieldCharacterPos();

	if (m_BattleManager) m_BattleManager = nullptr;
	if (m_AreaManager) m_AreaManager = nullptr;
}

void UBattleLifecycle::ChangeInputMode(PlayInputMode mode)
{
	APlayPlayerController* pc =
		Cast<APlayPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc)pc->ChangeInputMode(mode);
}

void UBattleLifecycle::CreateParty(AEnemyBase* enemyBase)
{
	m_BattleManager->CreateParty(enemyBase);
	// スピード順にリストを並び変える
	m_BattleManager->GetTurnManager()->SortBySpeedDescending();
}

void UBattleLifecycle::SetFieldCharacterPos()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(m_BattleManager->GetWorld(), 0);

	if (playerController)
	{
		AMainPlayer* player = Cast<AMainPlayer>(playerController->GetPawn());

		if (player)
		{
			player->SetPrevPos();
		}
	}
}

void UBattleLifecycle::SetBattleCharacterPos(ABattleAreaManager* area)
{
	if (!area) return;
	m_BattleManager->SetCharacterPos(area);
}

void UBattleLifecycle::CreateBattleUI()
{
	UBattleUIManager* uiManager = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (uiManager)
	{
		uiManager->CreateBattleUI();
	}

	// 敵のステータスUIを生成
	for (auto enemy : m_BattleManager->GetEnemyParty())
	{
		AEnemyBase* e = Cast<AEnemyBase>(enemy);
		if (e)
		{
			e->SetStatusUIVisibility(true);
		}
	}
}

void UBattleLifecycle::RemoveBattleUI()
{
	UWorld* world = GetWorld();
	if (!world) return;

	UBattleUIManager* uiManager = world->GetSubsystem<UBattleUIManager>();
	
	if (uiManager)
	{
		uiManager->RemoveBattleUI();
	}
}

/// <summary>
///  バトルLv上に存在するAreaを探して返す
/// </summary>
/// <returns>バトルLvに存在するBattleArea</returns>
ABattleAreaManager* UBattleLifecycle::GetBattleArea()
{
	UStageManager* stageMg = GetWorld()->GetSubsystem<UStageManager>();
	if (!stageMg) return nullptr;

	ULevel* level = stageMg->GetBattleLv()->GetLoadedLevel();

	if (level)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loaded Battle Level: %s"), *level->GetName());

		for (AActor* actor : level->Actors)
		{
			if (!actor) continue;
			ABattleAreaManager* area = Cast<ABattleAreaManager>(actor);
			
			if (!area) continue;

			UE_LOG(LogType, Warning, TEXT("BattleArea Name: %s"), *area->GetName());
			UE_LOG(LogType, Warning, TEXT("BattleArea Pos: %s"), *area->GetActorLocation().ToString());

			return area;
		}
	}
	return nullptr;
}

UCameraManager* UBattleLifecycle::GetCameraManager()
{
	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (!gameInstance) return nullptr;

	UCameraManager* cameraMg = gameInstance->GetSubsystem<UCameraManager>();
	return cameraMg;
}
