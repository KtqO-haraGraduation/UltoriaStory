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
		ABattleAreaManager* area = GetBattleArea();
		// カメラを切り替える
		ChangeBattleCamera(area);
		// 座標を設定
		SetBattleCharacterPos(area);
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

	ChangeFieldCamera();
	SetFieldCharacterPos();

	if (m_BattleManager) m_BattleManager = nullptr;
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

void UBattleLifecycle::ChangeFieldCamera()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(m_BattleManager->GetWorld(), 0);

	if (playerController)
	{
		ACharacter* player = Cast<ACharacter>(playerController->GetPawn());

		if (player)
		{
			// カメラ切り替え
			playerController->SetViewTargetWithBlend(player, 0.0f);
		}
	}
}

void UBattleLifecycle::ChangeBattleCamera(ABattleAreaManager* area)
{
	if (!area) return;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(m_BattleManager->GetWorld(), 0);

	if (playerController)
	{
		// カメラ切り替え
		playerController->SetViewTargetWithBlend(area->GetBattleCamera(), 1.0f);
	}
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
	UEncounterManager* encounterMg = GetWorld()->GetSubsystem<UEncounterManager>();
	ULevel* level = encounterMg->GetBattleLevel()->GetLoadedLevel();

	if (level)
	{
		for (AActor* actor : level->Actors)
		{
			if (!actor) continue;
			ABattleAreaManager* area = Cast<ABattleAreaManager>(actor);
			
			if (!area) continue;
			return area;
		}
	}
	return nullptr;
}
