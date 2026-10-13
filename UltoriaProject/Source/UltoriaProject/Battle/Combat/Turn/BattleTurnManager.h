// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleTurnManager.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleTurnManager : public UObject
{
	GENERATED_BODY()

public:
	void NextTurn();
	void UpdateTurnEnd();

	// キャラクターを追加
	void AddCharacter(ACharacterBase* character);			  // 単体で追加
	void AddParty(const TArray<ACharacterBase*>& characters); // パーティー単位で追加

	void ResetCharacters();
	
	void SortBySpeedDescending();	// スピード順に並び変える

	const TArray<ACharacterBase*>& GetCharacters() const { return m_Characters; }

	ACharacterBase* GetCurrentCharacter() { return m_Characters[m_CurrentIndex]; }
	ACharacterBase* GetPrevCharacter() { return m_PrevCharacter; }

private:
	int m_CurrentIndex;

	ACharacterBase* m_PrevCharacter; // 前回行動したキャラクター
	TArray<ACharacterBase*> m_Characters;
};
