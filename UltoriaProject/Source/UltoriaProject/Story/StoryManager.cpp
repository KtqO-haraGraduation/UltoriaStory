// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryManager.h"
#include "StoryRow.h"
#include "Engine/DataTable.h"


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

	UE_LOG(LogTemp, Warning, TEXT("[Story] table=%d currentRow=%s"),
		m_StoryTable != nullptr, *m_CurrentRow.ToString());
}

UDataTable* UStoryManager::TryGetEvent(FName triggerID, FName& outRowName)
{
	if (!m_StoryTable || m_CurrentRow.IsNone()) return nullptr;

	const FStoryRow* row = m_StoryTable->FindRow<FStoryRow>(m_CurrentRow, TEXT(""));
	if (!row)return nullptr;

	// 今の行のトリガーと違えば、まだ発動しない
	if (row->TriggerID != triggerID)
	{
		UE_LOG(LogTemp, Log, TEXT("[Story] trigger '%s' ignored (current=%s needs '%s')"),
			*triggerID.ToString(), *m_CurrentRow.ToString(), *row->TriggerID.ToString());
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

	UE_LOG(LogTemp, Warning, TEXT("[Story] completed -> next row = %s"), *m_CurrentRow.ToString());
}
