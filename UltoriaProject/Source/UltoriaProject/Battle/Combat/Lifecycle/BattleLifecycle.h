// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../../../System/Controller/PlayPlayerController.h"
#include "BattleLifecycle.generated.h"

class UBattleManager;
class ABattleAreaManager;
class AEnemyBase;
class UCameraManager;

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

	ABattleAreaManager* GetAreaManager() const { return m_AreaManager; }

private:
	void ChangeInputMode(PlayInputMode mode);
	void CreateParty(AEnemyBase* enemyBase);
	void SetFieldCharacterPos();
	void SetBattleCharacterPos(ABattleAreaManager* area);
	void CreateBattleUI();
	void RemoveBattleUI();

	ABattleAreaManager* GetBattleArea();

private:
	UCameraManager* GetCameraManager();

private:
	UPROPERTY()
	UBattleManager* m_BattleManager;
	ABattleAreaManager* m_AreaManager;
};
