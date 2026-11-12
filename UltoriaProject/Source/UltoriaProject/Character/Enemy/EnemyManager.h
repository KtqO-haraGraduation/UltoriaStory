// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManager.generated.h"

class AEnemyBase;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UEnemyManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void AddEnemy(AEnemyBase* enemy);
	void RemoveEnemy(AEnemyBase* enemy);

	const TArray<AEnemyBase*>& GetEnemies() const { return m_Enemies; }

private:
	UPROPERTY()
	TArray<AEnemyBase*> m_Enemies;
};
