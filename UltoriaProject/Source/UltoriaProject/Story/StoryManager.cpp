// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryManager.h"
#include "StoryRow.h"
#include "Engine/DataTable.h"
#include "ObjectiveWidget.h"
#include "GameFramework/PlayerController.h"


void UStoryManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// パスは、作ったDT_Storyの実際の場所に合わせる
	m_StoryTable = LoadObject<UDataTable>(
		nullptr,
		TEXT("/Game/Data/Story/StoryData")
	);

	// 最初の行を「今の行」にする
	if (m_StoryTable)
	{
		const TArray<FName> names = m_StoryTable->GetRowNames();
		m_CurrentRow = names.IsEmpty() ? NAME_None : names[0];
	}
}

UDataTable* UStoryManager::TryGetEvent(FName triggerID, FName& outRowName)
{
	if (!m_StoryTable || m_CurrentRow.IsNone()) return nullptr;

	const FStoryRow* row = m_StoryTable->FindRow<FStoryRow>(m_CurrentRow, TEXT(""));
	if (!row)return nullptr;

	// 今の行のトリガーと違えば、まだ発動しない
	if (row->TriggerID != triggerID)
	{
		return nullptr;
	}

	outRowName = m_CurrentRow;
	return row->Script.LoadSynchronous();
}

void UStoryManager::CompleteEvent(FName rowName)
{
	if (!m_StoryTable)return;

	// 今の行以外の終了通知は無視
	if (rowName != m_CurrentRow) return;

	const TArray<FName> names = m_StoryTable->GetRowNames();
	const int32 index = names.IndexOfByKey(m_CurrentRow);

	m_CurrentRow = names.IsValidIndex(index + 1) ? names[index + 1] : NAME_None;
}

void UStoryManager::CreateObjectiveWidget(UWorld* world)
{
	if (!world || !world->IsGameWorld())return;

	// 古いものは消す
	if (m_ObjectiveWidget)
	{
		m_ObjectiveWidget->RemoveFromParent();
		m_ObjectiveWidget = nullptr;
	}

	// 目的表示用テキストのWBPクラスをロード
	UClass* widgetClass = LoadClass<UObjectiveWidget>(
		nullptr, TEXT("/Game/MembarFiles/Yoshihiro/UI_Yoshi/Objective/WBP_Objective.WBP_Objective_C"));

	if (!widgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Story] WBP_Objective not found"));
		return;
	}

	// WBPクラスを元に作成
	m_ObjectiveWidget = CreateWidget<UObjectiveWidget>(world, widgetClass);
	if (!m_ObjectiveWidget) return;

	// 画面に追加
	m_ObjectiveWidget->AddToViewport();
	UpdateObjective();
}

void UStoryManager::SetObjectiveVisble(bool isVisible)
{
	m_IsObjectiveVisible = isVisible;
	UpdateObjective();
}

FName UStoryManager::GetCurrentTriggerID() const
{
	if (!m_StoryTable || m_CurrentRow.IsNone()) return NAME_None;

	// 現在のストーリ行と対応するトリガーIDを渡す
	const FStoryRow* row = m_StoryTable->FindRow<FStoryRow>(m_CurrentRow, TEXT(""));
	return row ? row->TriggerID : NAME_None;
}

void UStoryManager::UpdateObjective()
{
	if (!m_ObjectiveWidget)return;

	FText text;
	if (m_StoryTable && !m_CurrentRow.IsNone())
	{
		if (const FStoryRow* row = m_StoryTable->FindRow<FStoryRow>(m_CurrentRow, TEXT("")))
		{
			text = row->ObjectiveText;
		}
	}

	m_ObjectiveWidget->SetObjective(m_IsObjectiveVisible ? text : FText::GetEmpty());
}
