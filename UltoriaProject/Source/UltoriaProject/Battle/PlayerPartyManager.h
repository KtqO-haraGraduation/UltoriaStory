// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerPartyManager.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UPlayerPartyManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void AddPlayerParty(ACharacterBase* character);
	void RemovePlayerParty(ACharacterBase* character);

	UFUNCTION(BlueprintCallable)
	TArray<ACharacterBase*> GetPlayerParty() const { return m_PlayerParty; }

private:
	TArray<ACharacterBase*> m_PlayerParty;
};
