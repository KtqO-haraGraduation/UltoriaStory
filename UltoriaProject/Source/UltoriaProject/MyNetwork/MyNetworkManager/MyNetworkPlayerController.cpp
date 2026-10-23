// Fill out your copyright notice in the Description page of Project Settings.

#include "MyNetworkPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "../MyNetworkInstance/MyNetworkGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "../MyNetworkInstance/MyOnlineSubsystem.h"
#include "../MyNetworkPlayerState.h"

void AMyNetworkPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UMyOnlineSubsystem* online = GetGameInstance()->GetSubsystem<UMyOnlineSubsystem>();
	if (online)
	{
		online->Login();
	}

	CreateWidgetsOnce();
	UpdateUI();
}

void AMyNetworkPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	int32 NumPlayers = GetWorld() && GetWorld()->GetGameState()
		? GetWorld()->GetGameState()->PlayerArray.Num()
		: -1;

	FString Text = FString::Printf(
		TEXT(
			"===== NET DEBUG =====\n"
			"Name: %s\n"
			"LocalController: %d\n"
			"Authority: %d\n"
			"NetMode: %d\n"
			"Players: %d\n"
			"PlayerState: %s\n"
			"====================="
		),
		*GetName(),
		IsLocalController(),
		HasAuthority(),
		(int32)GetNetMode(),
		NumPlayers,
		PlayerState ? *PlayerState->GetName() : TEXT("None")
	);

	GEngine->AddOnScreenDebugMessage(
		1,
		0.f,
		HasAuthority() ? FColor::Red : FColor::Blue,
		Text
	);
}

void AMyNetworkPlayerController::FindSession()
{
	UMyOnlineSubsystem* online = GetGameInstance()->GetSubsystem<UMyOnlineSubsystem>();
	if (online)
	{
		if (UMyNetworkGameInstance* GI = GetGameInstance<UMyNetworkGameInstance>())
		{
			GI->IsInLobby = true;
		}
		online->FindSession();
	}
}

void AMyNetworkPlayerController::CreateRoom(const FString& roomId)
{
	UMyOnlineSubsystem* online = GetGameInstance()->GetSubsystem<UMyOnlineSubsystem>();
	if (online)
	{
		if (UMyNetworkGameInstance* GI = GetGameInstance<UMyNetworkGameInstance>())
		{
			GI->IsInLobby = true;
		}
		online->CreateRoom(roomId);
	}
}

void AMyNetworkPlayerController::JoinRoom(const FString& roomId)
{
	UMyOnlineSubsystem* online = GetGameInstance()->GetSubsystem<UMyOnlineSubsystem>();
	if (online)
	{
		if (UMyNetworkGameInstance* GI = GetGameInstance<UMyNetworkGameInstance>())
		{
			GI->IsInLobby = true;
		}
		online->JoinRoom(roomId);
	}
}


void AMyNetworkPlayerController::Input1()
{
	Server_Input1();
}

void AMyNetworkPlayerController::Server_Input1_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AMyNetworkPlayerController* PC = Cast<AMyNetworkPlayerController>(It->Get()))
		{
			PC->Client_ShowInput1();
		}
	}
}

void AMyNetworkPlayerController::Client_ShowInput1_Implementation()
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Green,
		FString::Printf(TEXT("Input 1"))
	);
}

void AMyNetworkPlayerController::SessionLeave()
{
	Server_LeaveSession();
}

void AMyNetworkPlayerController::Server_LeaveSession_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AMyNetworkPlayerController* PC = Cast<AMyNetworkPlayerController>(It->Get()))
		{
			PC->Client_LeaveSession();
		}
	}
}

void AMyNetworkPlayerController::Client_LeaveSession_Implementation()
{
	UMyOnlineSubsystem* online = GetGameInstance()->GetSubsystem<UMyOnlineSubsystem>();
	if (online)
	{
		if (UMyNetworkGameInstance* GI = GetGameInstance<UMyNetworkGameInstance>())
		{
			GI->IsInLobby = false;
		}
		online->LeaveSession();
	}
}

void AMyNetworkPlayerController::IsReady()
{
	if (AMyNetworkPlayerState* PS = GetPlayerState<AMyNetworkPlayerState>())
	{
		PS->Server_SetIsReady(!PS->GetIsReady());
	}
}

void AMyNetworkPlayerController::CreateWidgetsOnce()
{
	if (!IsLocalController())
	{
		return;
	}

	if (!m_MainWidget && m_MainWidgetClass)
	{
		m_MainWidget = CreateWidget<UUserWidget>(this, m_MainWidgetClass);

		if (m_MainWidget)
		{
			m_MainWidget->AddToViewport();
		}
	}

	if (!m_ReadyWidget && m_ReadyWidgetClass)
	{
		m_ReadyWidget = CreateWidget<UUserWidget>(this, m_ReadyWidgetClass);
		if (m_ReadyWidget)
		{
			m_ReadyWidget->AddToViewport();
		}
	}
}

void AMyNetworkPlayerController::UpdateUI()
{
	UMyNetworkGameInstance* GI = GetGameInstance<UMyNetworkGameInstance>();
	if (!GI) return;

	if (GI->IsInLobby)
	{
		ShowReadyUI();
		HideMainUI();
	}
	else
	{
		ShowMainUI();
		HideReadyUI();
	}
}

void AMyNetworkPlayerController::ShowMainUI()
{
	if (m_MainWidget)
	{
		m_MainWidget->SetVisibility(ESlateVisibility::Visible);

		bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(m_MainWidget->TakeWidget());
		SetInputMode(InputMode);
	}
}

void AMyNetworkPlayerController::HideMainUI()
{
	if (m_MainWidget)
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		bShowMouseCursor = false;

		m_MainWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyNetworkPlayerController::ShowReadyUI()
{
	if (m_ReadyWidget)
	{
		m_ReadyWidget->SetVisibility(ESlateVisibility::Visible);
	}

	CheckPlayerStateReady();
}

void AMyNetworkPlayerController::CheckPlayerStateReady()
{
	if (AMyNetworkPlayerState* PS = GetPlayerState<AMyNetworkPlayerState>())
	{
		AGameStateBase* GS = GetWorld()->GetGameState();

		int32 NumPlayers = GS ? GS->PlayerArray.Num() : -1;

		GEngine->AddOnScreenDebugMessage(
			100,
			10.0f,
			FColor::Yellow,
			FString::Printf(TEXT("PlayerArray Num = %d"), NumPlayers)
		);

		PS->OnStateChanged.Broadcast();
		return;
	}

	GetWorldTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(
			this,
			&AMyNetworkPlayerController::CheckPlayerStateReady
		)
	);
}

void AMyNetworkPlayerController::HideReadyUI()
{
	if (m_ReadyWidget)
	{
		m_ReadyWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
