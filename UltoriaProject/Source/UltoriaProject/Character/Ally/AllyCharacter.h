// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "PlayerCharacterBase.h"
#include "AllyCharacter.generated.h"

struct FJobData;
struct FJobBase;
class UJobDataComponent;

UCLASS()
class ULTORIAPROJECT_API AAllyCharacter : public APlayerCharacterBase
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

private:

	//UPROPERTY(EditAnywhere)
	//int m_JobId;

	// ジョブの情報
	//FJobBase* m_JobBase;	// ベース
	//FJobData* m_JobData;	// データ
};
