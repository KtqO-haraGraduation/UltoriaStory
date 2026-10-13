// Fill out your copyright notice in the Description page of Project Settings.

#include "MyNetworkPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include <Net/UnrealNetwork.h>

AMyNetworkPlayerState::AMyNetworkPlayerState()
{
	m_SlotIndex = 0;

	m_IsReady = false;
}

void AMyNetworkPlayerState::SetSlotIndex(int32 NewIndex)
{
	if (!HasAuthority()) return;
	m_SlotIndex = NewIndex;
}

void AMyNetworkPlayerState::SetIsReady(bool NewValue)
{
	if (!HasAuthority()) return;
	m_IsReady = NewValue;
	AGameStateBase* GS = GetWorld()->GetGameState();

	for (APlayerState* BasePS : GS->PlayerArray)
	{
		if (AMyNetworkPlayerState* PS = Cast<AMyNetworkPlayerState>(BasePS))
		{
			PS->OnStateChanged.Broadcast();
		}
	}
}

void AMyNetworkPlayerState::OnRep_SlotIndex()
{
}

void AMyNetworkPlayerState::MyStateChanged()
{
	OnStateChanged.Broadcast();
}

void AMyNetworkPlayerState::OnRep_IsReady()
{
	AGameStateBase* GS = GetWorld()->GetGameState();

	for (APlayerState* BasePS : GS->PlayerArray)
	{
		if (AMyNetworkPlayerState* PS = Cast<AMyNetworkPlayerState>(BasePS))
		{
			PS->OnStateChanged.Broadcast();
		}
	}
}

void AMyNetworkPlayerState::Server_SetIsReady_Implementation(bool NewValue)
{
	SetIsReady(NewValue);
}

void AMyNetworkPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyNetworkPlayerState, m_SlotIndex);
	DOREPLIFETIME(AMyNetworkPlayerState, m_IsReady);
}