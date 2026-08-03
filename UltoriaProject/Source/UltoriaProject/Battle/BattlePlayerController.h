// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "BattlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;

private:
	void InputMove(const FInputActionValue& Value);
	void InputConfirm();


private:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* m_InputBattle;

	UPROPERTY(EditAnywhere)
	UInputAction* m_InputMove;

	UPROPERTY(EditAnywhere)
	UInputAction* m_InputConfirn;
};
