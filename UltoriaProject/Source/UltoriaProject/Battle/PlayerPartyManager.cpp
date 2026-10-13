// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPartyManager.h"
#include "../Character/CharacterBase.h"
#include "../Character/Data/CharacterDataManager.h"

constexpr int PARTY_MEMBER_MIN = 1;
constexpr int PARTY_MEMBER_MAX = 4;

void UPlayerPartyManager::AddPlayerParty(const FPartyData& character)
{
	if (m_PartyData.Num() >= PARTY_MEMBER_MAX) return;

	m_PartyData.AddUnique(character);
}

void UPlayerPartyManager::RemovePlayerParty(const FPartyData& character)
{
	if (m_PartyData.Num() <= PARTY_MEMBER_MIN) return;

	m_PartyData.Remove(character);
}

void UPlayerPartyManager::CreatePlayerParty()
{
	// プレイヤーパーティーを取得（ID → CharacterBase 変換）
	for (const FPartyData& player : m_PartyData)
	{
		UCharacterDataManager* characterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
		if (!characterDataMg) return;
		FAllyMasterData data = characterDataMg->GetAllyMasterData(player.characterID);

		// 生成時のスポーン座標
		FVector spawnLocation(
			m_PlayerParty.Num() * 200.0f,
			0.0f,
			0.0f
		);

		// 生成時の当たり判定緩和（ある程度キャラクター同士が重なってても生成してくれる）
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		UWorld* world = GetWorld();

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("World: %s"),
			world ? *world->GetName() : TEXT("nullptr")
		);

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Class: %s"),
			data.Class ? *data.Class->GetPathName() : TEXT("nullptr")
		);

		// プレイヤーと仲間を生成
		ACharacterBase* character = world
			? world->SpawnActor<ACharacterBase>(
				data.Class,
				spawnLocation,
				FRotator::ZeroRotator,
				spawnParams)
			: nullptr;

		UE_LOG(LogTemp, Warning, TEXT("Spawn Result : %s"),
			character ? *character->GetName() : TEXT("nullptr"));

		// 成功したら追加
		if (character)
		{
			// AIControllerを生成してキャラクターに設定
			character->SpawnDefaultController();
			m_PlayerParty.Add(character);
		}
	}
}

void UPlayerPartyManager::DeletePlayerParty()
{
	for (ACharacterBase* character : m_PlayerParty)
	{
		if (character)
		{
			character->Destroy();
		}
	}

	m_PlayerParty.Empty();
}
