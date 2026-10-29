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

	void Start();
	void End();

	// キャラクターを追加
	void AddCharacter(ACharacterBase* character);			  // 単体で追加
	void AddParty(const TArray<ACharacterBase*>& characters); // パーティー単位で追加
	
	void SortBySpeedDescending();	// スピード順に並び変える

	const TArray<ACharacterBase*>& GetCharacters() const { return m_Characters; }

	ACharacterBase* GetCurrentCharacter() { return m_Characters[m_CurrentIndex]; }
	ACharacterBase* GetPrevCharacter() { return m_PrevCharacter; }

	int GetTurnNum() const { return m_TurnNum; }

private:
	void ResetCharacters();

	void AddTurn();   // ターン数を加算
	void ResetTurn(); // ターン数をリセット

	void CreateSpeedUI(); // スピードUIを作り直す
	void RemoveSpeedUI(); // 行動が終わったキャラクターのスピードUIを非表示

private:
	int m_CurrentIndex;
	int m_TurnNum;	// ターン数

	ACharacterBase* m_PrevCharacter; // 前回行動したキャラクター
	TArray<ACharacterBase*> m_Characters;
};
