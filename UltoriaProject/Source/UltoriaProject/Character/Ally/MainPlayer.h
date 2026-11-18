// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterBase.h"
#include "InputActionValue.h"
#include "MainPlayer.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API AMainPlayer : public APlayerCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

public:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	void SetPrevPos();

	void Move(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);
	void AttackEnd();

	void ZoomCamera(const FInputActionValue& Value);
	void ZoomReset(const FInputActionValue& Value);

	USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return m_FollowCamera; }

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION(BlueprintCallable)
	void StartBattle(ACharacterBase* otherActor, bool isPreemptive);


private:
	// コンポーネント取得
	void FindComponents();
	// コンポーネントの設定
	void SetupComponents();

private:
	FVector m_PrevPos;  // 前回の座標（バトル開始時の座標）
	bool m_IsAttacking; // アタック中か

	UStaticMeshComponent* m_StaticMesh;
	UBoxComponent* m_AttackBox;

	USpringArmComponent* m_CameraBoom;
	UCameraComponent* m_FollowCamera;

	int m_TestCount = 60;

	float m_InitYawOffset;
};
