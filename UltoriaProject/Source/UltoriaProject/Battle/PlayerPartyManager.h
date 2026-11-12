// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerPartyManager.generated.h"

class ACharacterBase;
class AMainPlayer;

USTRUCT(BlueprintType)
struct FPartyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName characterID;

	bool operator==(const FPartyData& other) const
	{
		return characterID == other.characterID;
	}
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UPlayerPartyManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void SetFieldPlayer(AMainPlayer* player);
	AMainPlayer* GetFieldPlayer() const { return m_FieldPlayer; }

	void AddPlayerParty(const FPartyData& character);
	void RemovePlayerParty(const FPartyData& character);

	void CreatePlayerParty();
	void DeletePlayerParty();

	UFUNCTION(BlueprintCallable)
	TArray<FPartyData> GetPartyData() const { return m_PartyData; }
	TArray<ACharacterBase*> GetPlayerParty() const { return m_PlayerParty; }

private:
	AMainPlayer* m_FieldPlayer;

	TArray<FPartyData> m_PartyData;
	TArray<ACharacterBase*> m_PlayerParty;
};
