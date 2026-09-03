// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "InputActionValue.h"
#include "MainPlayer.generated.h"

struct FJobData;
struct FJobBase;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API AMainPlayer : public ACharacterBase
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

	USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return m_FollowCamera; }

	// 最終ステータス
	FStatus GetTotalStatus() const override;

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION(BlueprintCallable)
	void StartBattle(ACharacterBase* otherActor, bool isPreemptive);

private:
	void JobChange();	// ジョブの切り替え
	FStatus TotalStatus();	// 総合ステータス

private:
	UPROPERTY(EditAnywhere)
	EPlayerType m_PlayerType;
	
	UPROPERTY(EditAnywhere)
	int m_JobId;

	// ジョブの情報
	FJobBase* m_JobBase;	// ベース
	FJobData* m_JobData;	// データ

	FVector m_PrevPos;  // 前回の座標（バトル開始時の座標）
	bool m_IsAttacking; // アタック中か

	UBoxComponent* m_AttackBox;
	USpringArmComponent* m_CameraBoom;
	UCameraComponent* m_FollowCamera;

	int m_TestCount = 60;
};
