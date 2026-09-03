// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "AllyCharacter.generated.h"

struct FJobData;
struct FJobBase;

UCLASS()
class ULTORIAPROJECT_API AAllyCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAllyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 最終ステータス
	FStatus GetTotalStatus() const override;

private:
	void JobChange();	// ジョブの切り替え

private:
	UPROPERTY(EditAnywhere)
	EPlayerType m_PlayerType;

	UPROPERTY(EditAnywhere)
	int m_JobId;

	// ジョブの情報
	FJobBase* m_JobBase;	// ベース
	FJobData* m_JobData;	// データ
};
