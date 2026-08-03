// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "EnemyCombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API UEnemyCombatComponent : public UCombatComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyCombatComponent();

	// バトル開始処理
	void BattleStart() override;
	// バトル終了処理
	void BattleEnd()  override;

	void TakeDamage(int damage) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
