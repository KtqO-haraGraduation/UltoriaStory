// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "EncounterManager.generated.h"

class ACharacterBase;
class USpringArmComponent;
class UCameraComponent;
class ULevelStreaming;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UEncounterManager : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;

	void StartEncounter(ACharacterBase* player, ACharacterBase* enemy, bool isPreemptive);
	void Update(float DeltaTime);
	void ResetFieldCamera();

	void SetEncounterFogEnabled(bool bEnable);

	ULevelStreaming* GetBattleLevel() const { return m_BattleLevel; }

	// バトルLVの表示 / 非表示
	UFUNCTION(BlueprintCallable)
	void HideBattleLevels();
	ULevelStreaming* SetLevelVisible(FName levelName, bool bVisible);
	void SetBattleLevelVisible(bool bVisible);
	void SetFieldLevelVisible(bool bVisible);
	ULevelStreaming* GetLevel(FName levelName);

private:
	void EnterBattle();

private:
	float m_PrevArmLength;
	float m_PrevFOV;

	float m_TargetArmLength;
	float m_TargetFOV;

	bool m_IsZoom = false;

	USpringArmComponent* m_Boom;
	UCameraComponent* m_Camera;

	ACharacterBase* m_Player;
	ACharacterBase* m_Enemy;
	bool m_Flag;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> m_EncounterMID;

	float m_FogRadius;

	ULevelStreaming* m_BattleLevel;
	ULevelStreaming* mFieldLevel;
};
