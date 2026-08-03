// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../../../System/Controller/PlayPlayerController.h"
#include "BattleLifecycle.generated.h"

class UBattleManager;
class ABattleAreaManager;
class AEnemyBase;
class ABattleAreaManager;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleLifecycle : public UObject
{
	GENERATED_BODY()
	
public:
	void Start(UBattleManager* mg, AEnemyBase* enemyBase);
	void End();

private:
	void ChangeInputMode(PlayInputMode mode);
	void CreateParty(AEnemyBase* enemyBase);
	void ChangeFieldCamera();
	void ChangeBattleCamera(ABattleAreaManager* area);
	void SetFieldCharacterPos();
	void SetBattleCharacterPos(ABattleAreaManager* area);
	void CreateBattleUI();
	void RemoveBattleUI();

	ABattleAreaManager* GetBattleArea();

private:
	UPROPERTY()
	UBattleManager* m_BattleManager;
};
