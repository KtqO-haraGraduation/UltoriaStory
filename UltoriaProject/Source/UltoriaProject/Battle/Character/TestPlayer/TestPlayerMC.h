// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "../../../Character/CharacterBase.h"
#include "TestPlayerMC.generated.h"

class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Blueprintable)
class ULTORIAPROJECT_API ATestPlayerMC : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestPlayerMC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPrevPos();

	void Move(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	void AttackEnd();

	USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return m_FollowCamera; }

protected:
	UFUNCTION(BlueprintCallable)
	void StartBattle(ACharacterBase* otherActor, bool isPreemptive);

private:
	FVector m_PrevPos;  // 前回の座標（バトル開始時の座標）
	bool m_IsAttacking; // アタック中か

	UBoxComponent* m_AttackBox;
	USpringArmComponent* m_CameraBoom;
	UCameraComponent* m_FollowCamera;

	int m_TestCount = 60;
};
