// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "PlayerCharacterBase.generated.h"

class UJobDataComponent;

UCLASS()
class ULTORIAPROJECT_API APlayerCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UJobDataComponent* GetJob() { return m_Job; }
	
	// 最終ステータス
	FStatus GetTotalStatus() const override;

	virtual void SetJob();					// セーブデータのロード時に
	virtual void ChangeJob(FName jobName);	// ジョブの切り替え


protected:
	UPROPERTY(EditAnywhere)
	EPlayerType m_PlayerType;

	UPROPERTY(EditAnywhere)
	int m_JobId;

	UJobDataComponent* m_Job;
};
