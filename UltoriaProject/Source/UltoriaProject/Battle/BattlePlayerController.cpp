// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BattleManager.h"

void ABattlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* enhancedInput =
		Cast<UEnhancedInputComponent>(InputComponent);

	if (!enhancedInput) return;

	enhancedInput->BindAction(m_InputMove, ETriggerEvent::Started, this, &ABattlePlayerController::InputMove);
	enhancedInput->BindAction(m_InputConfirn, ETriggerEvent::Started, this, &ABattlePlayerController::InputConfirm);
}

void ABattlePlayerController::InputMove(const FInputActionValue& Value)
{
	// アナログスティックの入力をベクトルで取得
	FVector2D move = Value.Get<FVector2D>();

	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleManager) return;

	if (move.X > 0.5f)
	{
		//battleManager->SelectMoveTarget(1);
	}
	else if (move.X < -0.5f)
	{
		//battleManager->SelectMoveTarget(-1);
	}
}

void ABattlePlayerController::InputConfirm()
{
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleManager) return;

	battleManager->SelectConfirn();
}
