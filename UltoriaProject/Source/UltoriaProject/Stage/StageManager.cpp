// Fill out your copyright notice in the Description page of Project Settings.

#include "StageManager.h"
#include "Data/StageDataAsset.h"
#include "Data/StageLvData.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "../Event/EventScriptEngine.h"
#include "../UI/Map/MapManager.h"
#include "../Stage/StageManager.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/LevelStreaming.h"
#include "../Story/StoryManager.h"

void UStageManager::Initialize(FSubsystemCollectionBase& Collection)
{
	// ステージのデータアセットをロード
	LoadStageDataAsset();

	// 必要なステージデータをロード
	LoadStageLvData();

	// Lv_Playを開いでいる場合のみ実行
	UWorld* world = GetWorld();
	if (world && world->GetName() == TEXT("Lv_Play"))
	{
		ChangeStage(EStageType::TUTORIAL, "Tutorial");
	}
}

void UStageManager::LoadStageDataAsset()
{
	m_StageDataAsset = LoadObject<UStageDataAsset>(
		nullptr,
		TEXT("/Game/Data/Lv/DA_Stage.DA_Stage")
	);
}

void UStageManager::LoadStageLvData()
{
	if (!IsStageDataAssetLoaded()) return;

	UDataTable* table = m_StageDataAsset->StageLvTable;
	if (!table) return;

	m_StageLvDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FStageLvData* data =
			reinterpret_cast<FStageLvData*>(row.Value);

		if (data)
		{
			m_StageLvDatas.Add(data->Name, *data);
		}
	}
}

void UStageManager::ChangeStage(EStageType type, FName id)
{
	const FStageLvData* data = m_StageLvDatas.Find(id);
	if (!data) return;

	SetScreenBlack();

	m_StageType = type;
	m_StageName = id;

	m_IsEnterEventPending = true;

	// 現在のLvをUnload
	if (m_CurrentFieldLv)
	{
		m_CurrentFieldLv->SetShouldBeVisible(false);
		m_CurrentFieldLv->SetShouldBeLoaded(false);
		m_CurrentFieldLv = nullptr;
	}

	if (m_CurrentBattleLv)
	{
		m_CurrentBattleLv->SetShouldBeVisible(false);
		m_CurrentBattleLv->SetShouldBeLoaded(false);
		m_CurrentBattleLv = nullptr;
	}

	// 新しいLvを作成
	m_CurrentFieldLv = LoadLevel(data->FieldLv, true);
	m_CurrentBattleLv = LoadLevel(data->BattleLv, false);

	// フィールドLvの表示完了後にミニマップを作る
	if (m_CurrentFieldLv)
	{
		m_CurrentFieldLv->OnLevelShown.AddDynamic(this, &UStageManager::OnFieldLevelShown);
	}
}

void UStageManager::ShowFieldLv()
{
	if (!m_CurrentFieldLv || !m_CurrentBattleLv) return;

	m_CurrentBattleLv->SetShouldBeVisible(false);
}

void UStageManager::ShowBattleLv()
{
	if (!m_CurrentFieldLv || !m_CurrentBattleLv) return;

	m_CurrentBattleLv->SetShouldBeVisible(true);
}

FStageLvData UStageManager::GetNowStageLvData()
{
	const FStageLvData* data = m_StageLvDatas.Find(m_StageName);
	if (data)
	{
		return *data;
	}
	return FStageLvData();
}

FStageLvData UStageManager::GetStageLvData(FName id)
{
	const FStageLvData* data = m_StageLvDatas.Find(id);
	if (data)
	{
		return *data;
	}
	return FStageLvData();
}

bool UStageManager::ShouldCreateSubsystem(UObject* Outer) const
{
	if(!Super::ShouldCreateSubsystem(Outer)) return false;

	UWorld* world = Cast<UWorld>(Outer);
	return world && world->IsGameWorld();
}

void UStageManager::SetScreenBlack()
{
	UWorld* world = GetWorld();
	if (!world)return;

	APlayerController* pc = world->GetFirstPlayerController();
	if (!pc || !pc->PlayerCameraManager) return;

	// 時間0で即座に黒、そのまま保持
	pc->PlayerCameraManager->StartCameraFade(1.0f, 1.0f, 0.0f, FLinearColor::Black, false, true);
}

void UStageManager::MovePlayerToStageStart()
{
	// プレイヤーコントローラーを取得
	UWorld* world = GetWorld();
	APlayerController* pc = world ? world->GetFirstPlayerController() : nullptr;

	// 現在のフィールドレベル、プレイヤー、プレイヤーコントローラのいずれかがなければ終了
	if (!pc || !pc->GetPawn() || !m_CurrentFieldLv) return;

	// フィールドレベルを取得
	ULevel* fieldLevel = m_CurrentFieldLv->GetLoadedLevel();
	// フィールドレベルを取得できなければ終了
	if (!fieldLevel) return;

	// プレイヤースタートを検索
	for (AActor* actor : fieldLevel->Actors)
	{
		if (APlayerStart* start = Cast<APlayerStart>(actor))
		{
			// プレイヤーの位置をスタート地点に移動
			pc->GetPawn()->TeleportTo(start->GetActorLocation(), start->GetActorRotation());
			pc->SetControlRotation(start->GetActorRotation());

			if (ACharacter* chara = Cast<ACharacter>(pc->GetPawn()))
			{
				chara->GetCharacterMovement()->StopMovementImmediately();
			}
			return;
		}
	}
	
}

void UStageManager::OnFieldLevelShown()
{
	UE_LOG(LogTemp, Warning, TEXT("[Stage] OnFieldLevelShown"));

	UWorld* world = GetWorld();
	if (!world)return;

	if (UGameInstance* gameInstance = world->GetGameInstance())
	{
		if (UMapManager* map = gameInstance->GetSubsystem<UMapManager>())
		{
			map->CreateMiniMap(world);
		}

		if (UStoryManager* story = gameInstance->GetSubsystem<UStoryManager>())
		{
			story->CreateObjectiveWidget(world);
		}
	}

	

	if (m_IsEnterEventPending)
	{
		m_IsEnterEventPending = false;

		// Pawnのスポーンを待つため、次のティックで開始
		world->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateUObject(this, &UStageManager::StartEnterEvent));
	}
}

bool UStageManager::IsStageDataAssetLoaded()
{
	if (!m_StageDataAsset) return false;
	return true;
}

ULevelStreamingDynamic* UStageManager::LoadLevel(TSoftObjectPtr<UWorld> level, bool isVisible)
{
	if (level.IsNull()) return nullptr;

	bool bSuccess = false;

	ULevelStreamingDynamic* streamingLevel =
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
			GetWorld(),
			level,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			bSuccess,
			TEXT(""),
			nullptr,
			false
		);

	streamingLevel->SetShouldBeLoaded(true);
	streamingLevel->SetShouldBeVisible(isVisible);

	return streamingLevel;
}

void UStageManager::StartEnterEvent()
{
	MovePlayerToStageStart();

	// ステージ開始時イベントのデータを取得
	UDataTable* table = GetNowStageLvData().EnterEventTable.LoadSynchronous();
	if (!table)return;

	// イベントを走らせるアクターを取得
	TArray<AActor*> runners;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("StageEventRunner"), runners);

	if (runners.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Stage] StageEventRunner not found"));
		return;
	}

	if (UEventScriptEngine* engine = runners[0]->FindComponentByClass<UEventScriptEngine>())
	{
		engine->StartScript(table);
	}
}

