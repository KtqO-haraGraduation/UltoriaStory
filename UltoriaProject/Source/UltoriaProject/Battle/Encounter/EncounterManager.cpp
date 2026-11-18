// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterManager.h"
#include "../../System/Controller/PlayPlayerController.h"
#include "../../Character/Ally/MainPlayer.h"
#include "../../Character/CharacterBase.h"
#include "../../Stage/StageManager.h"
#include "../../Camera/CameraManager.h"
#include "../BattleManager.h"
#include "../PlayerPartyManager.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"
#include "WorldPartition/WorldPartitionSubsystem.h"

void UEncounterManager::Tick(float DeltaTime)
{
	Update(DeltaTime);
}

TStatId UEncounterManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UEncounterManager, STATGROUP_Tickables);
}

bool UEncounterManager::IsTickable() const
{
	return m_IsZoom;
}

void UEncounterManager::StartEncounter(ACharacterBase* enemy, bool isPreemptive)
{
	if (m_IsZoom) return;

	// 入力切替
	APlayPlayerController* pc =
		Cast<APlayPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc) pc->ChangeInputMode(PlayInputMode::BATTLE);

	// カメラ演出
	AMainPlayer* player = Cast<AMainPlayer>(pc->GetPawn());
	if (!player) return;

	m_Boom = player->GetCameraBoom();
	m_Camera = player->GetFollowCamera();

	if (!m_Boom || !m_Camera)
	{
		return;
	}

	// 元の値を保存
	m_PrevArmLength = m_Boom->TargetArmLength;
	m_PrevFOV = m_Camera->FieldOfView;

	// ターゲットの値を設定
	m_TargetArmLength = m_PrevArmLength * 0.6f;
	m_TargetFOV = m_PrevFOV - 15.0f;

	m_IsZoom = true;

	m_Player = player;
	m_Enemy = enemy;
	m_Flag = isPreemptive;


	FPostProcessSettings& PPS = m_Camera->PostProcessSettings;

	PPS.bOverride_DepthOfFieldFstop = true;
	PPS.bOverride_DepthOfFieldFocalDistance = true;

	PPS.DepthOfFieldFstop = 1.2f;
	PPS.DepthOfFieldFocalDistance = 200.0f;

	FWeightedBlendables& blendables =
		m_Camera->PostProcessSettings.WeightedBlendables;

	if (!m_EncounterMID)
	{
		if (blendables.Array.Num() > 0)
		{
			UMaterialInterface* Material =
				Cast<UMaterialInterface>(blendables.Array[0].Object);

			if (Material)
			{
				m_EncounterMID =
					UMaterialInstanceDynamic::Create(Material, this);

				// カメラに設定されているMIをMIDに置き換える
				blendables.Array[0].Object = m_EncounterMID;
			}
		}
	}

}

void UEncounterManager::Update(float DeltaTime)
{

	if (!m_IsZoom) return;



	m_Boom->TargetArmLength =
		FMath::FInterpTo(
			m_Boom->TargetArmLength,
			m_TargetArmLength,
			DeltaTime,
			8.0f
		);

	m_Camera->SetFieldOfView(
		FMath::FInterpTo(
			m_Camera->FieldOfView,
			m_TargetFOV,
			DeltaTime,
			8.0f
		)
	);

	float fStop = FMath::FInterpTo(
		m_Camera->PostProcessSettings.DepthOfFieldFstop,
		1.2f,
		DeltaTime,
		8.0f
	);

	m_Camera->PostProcessSettings.DepthOfFieldFstop = fStop;

	if (m_EncounterMID)
	{
		m_FogRadius = FMath::FInterpTo(
			m_FogRadius,
			0.2f,
			DeltaTime,
			5.0f);

		m_EncounterMID->SetScalarParameterValue(
			TEXT("Radius"),
			m_FogRadius
		);
	}
	

	bool ArmFinished =
		FMath::Abs(m_Boom->TargetArmLength - m_TargetArmLength) < 1.0f;

	bool FOVFinished =
		FMath::Abs(m_Camera->FieldOfView - m_TargetFOV) < 0.5f;

	if (ArmFinished && FOVFinished && m_IsZoom)
	{
		m_IsZoom = false;

		
	}
}

void UEncounterManager::ResetFieldCamera()
{
	/*m_Boom->TargetArmLength = m_PrevArmLength;
	m_Camera->FieldOfView = m_PrevFOV;*/
}

void UEncounterManager::SetEncounterFogEnabled(bool bEnable)
{
	if (!m_Camera)
	{
		return;
	}
	m_Camera->PostProcessSettings.WeightedBlendables.Array[0].Weight = bEnable ? 1.0f : 0.0f;
}

void UEncounterManager::HideBattleLevels()
{
	UWorld* world = GetWorld();
	if (!world) return;

	for (ULevelStreaming* level : world->GetStreamingLevels())
	{
		if (!level) continue;

		const FString packageName = level->GetWorldAssetPackageFName().ToString();

		// BattleLevelだけを対象にする
		if (packageName.Contains(TEXT("Battle")))
		{
			level->SetShouldBeVisible(false);
		}
	}
}

void UEncounterManager::EnterBattle()
{
	/*UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	battleMg->SetBattle(true);*/

	UStageManager* stageMg = GetWorld()->GetSubsystem<UStageManager>();
	if (!stageMg) return;

	stageMg->ShowBattleLv();

	// FieldLvとBattleLvの準備が完了したらBattleへ
	GetWorld()->GetTimerManager().SetTimer(
		m_ReadyTimer,
		this,
		&UEncounterManager::CheckBattleLevelReady,
		0.01f,
		true
	);
}

void UEncounterManager::EndBattle()
{
	// BattleLvとFieldLvの準備が完了したらFieldLvへ
	GetWorld()->GetTimerManager().SetTimer(
		m_ReadyTimer,
		this,
		&UEncounterManager::CheckFieldLevelReady,
		0.01f,
		true
	);
}

void UEncounterManager::CheckFieldLevelReady()
{
	UStageManager* stageMg = GetWorld()->GetSubsystem<UStageManager>();
	if (!stageMg) return;

	ULevelStreaming* battleLv = stageMg->GetBattleLv();
	ULevelStreaming* fieldLv = stageMg->GetFieldLv();
	if (!battleLv || !fieldLv) return;

	stageMg->ShowFieldLv();

	// Battleの読み込み・表示とFieldの非表示を確認
	if (!fieldLv->IsLevelLoaded() ||
		!fieldLv->IsLevelVisible() ||
		battleLv->IsLevelVisible())
	{
		return;
	}

	// 条件を満たしたらタイマーを停止
	GetWorld()->GetTimerManager().ClearTimer(
		m_ReadyTimer
	);
	

	UPlayerPartyManager* playerPartyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (!playerPartyMg) return;

	AMainPlayer* fieldPlayer = playerPartyMg->GetFieldPlayer();
	if (fieldPlayer) fieldPlayer->SetPrevPos();

	// ここでカメラを切り替える
	UCameraManager* cameraMg = GetWorld()->GetGameInstance()->GetSubsystem<UCameraManager>();
	if (cameraMg) cameraMg->ChangeFieldCamera();
}

void UEncounterManager::CheckBattleLevelReady()
{
	UStageManager* stageMg = GetWorld()->GetSubsystem<UStageManager>();
	if (!stageMg) return;

	ULevelStreaming* battleLv = stageMg->GetBattleLv();
	if (!battleLv) return;

	// Battleの読み込み・表示とFieldの非表示を確認
	if (!battleLv->IsLevelLoaded()  ||
        !battleLv->IsLevelVisible())
	{
		return;
	}

	// 条件を満たしたらタイマーを停止
	GetWorld()->GetTimerManager().ClearTimer(
		m_ReadyTimer
	);

	// レベル切り替え完了後に戦闘開始
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;
	
	battleMg->BattleStart(m_Enemy, m_Flag);
}
