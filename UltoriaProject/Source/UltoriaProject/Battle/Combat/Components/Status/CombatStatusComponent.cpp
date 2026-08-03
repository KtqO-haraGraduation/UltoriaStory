// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatStatusComponent.h"
#include "../../Data/BattleDataManager.h"

// Sets default values for this component's properties
UCombatStatusComponent::UCombatStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombatStatusComponent::UpdateTurn()
{
	for (int i = m_StatusList.Num() - 1; i >= 0; i--)
	{
		FBattleStatusData& status = m_StatusList[i];

		switch (status.type)
		{
		case StatusType::HEAL:
		{
			UpdateHeal(status);
			break;
		}
		case StatusType::POISON:
		{
			UpdatePoison(status);
			break;
		}
		}

		status.turn--;

		if (status.turn <= 0)
		{
			m_StatusList.RemoveAt(i);

			if (status.type == StatusType::GUARD)
			{
				m_IsGuarding = false;
			}
		}
	}
}

void UCombatStatusComponent::AddStatus(StatusType type, FName statusID)
{
	FBattleStatusData* status = FindStatus(type);

	UBattleDataManager* dataMg = GetWorld()->GetSubsystem<UBattleDataManager>();

	// 同じタイプがすでにリストの中に入っていたら
	if (status)
	{
		// そのステータスのターンを初期化
		FBattleStatusData data = dataMg->FindStatusData(statusID);
		status->turn = data.turn;
	}
	else
	{
		FBattleStatusData data = dataMg->FindStatusData(statusID);
		m_StatusList.Add(data);
	}
}

bool UCombatStatusComponent::HasStatus(StatusType type) const
{
	for (const auto& status : m_StatusList)
	{
		if (status.type == type)
		{
			return true;
		}
	}
	return false;
}

FBattleStatusData* UCombatStatusComponent::FindStatus(StatusType type)
{
	for (auto& status : m_StatusList)
	{
		if (status.type == type)
		{
			return &status;
		}
	}
	return nullptr;
}

void UCombatStatusComponent::UpdateHeal(const FBattleStatusData& status)
{
}

void UCombatStatusComponent::UpdatePoison(const FBattleStatusData& status)
{
}

// Called when the game starts
void UCombatStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

