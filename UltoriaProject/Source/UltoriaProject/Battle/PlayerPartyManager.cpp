// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPartyManager.h"
#include "../Character/Ally/MainPlayer.h"
#include "../Character/CharacterBase.h"
#include "../Character/Data/CharacterDataManager.h"
#include "BattleManager.h"

constexpr int PARTY_MEMBER_MIN = 1;
constexpr int PARTY_MEMBER_MAX = 4;

void UPlayerPartyManager::SetFieldPlayer(AMainPlayer* player)
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg || !player) return;

	if (!battleMg->IsBattle())
	{
		m_FieldPlayer = player;
	}
}

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

void UPlayerPartyManager::CreatePlayerParty(/*ACharacterBase* mainPlayer*/)
{
	//if (!mainPlayer) return;

	//// 既存の仲間を削除
	//DeletePlayerParty();

	//// メインプレイヤーをパーティーの先頭に登録
	//m_PlayerParty.Add(mainPlayer);

	//UCharacterDataManager* characterDataMg =
	//	GetWorld()->GetSubsystem<UCharacterDataManager>();

	//if (!characterDataMg) return;

	//// 仲間だけを生成（メインプレイヤーは除外）
	//for (int32 i = 1; i < m_PartyData.Num(); i++)
	//{
	//	const FPartyData& player = m_PartyData[i];

	//	FAllyMasterData data = characterDataMg->GetAllyMasterData(player.characterID);

	//	if (!data.Class) continue;

	//	FVector spawnLocation(mainPlayer->GetActorLocation().X + i * 200.0f,
	//		mainPlayer->GetActorLocation().Y, mainPlayer->GetActorLocation().Z);

	//	FActorSpawnParameters spawnParams;
	//	spawnParams.SpawnCollisionHandlingOverride =
	//		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//	ACharacterBase* character =GetWorld()->SpawnActor<ACharacterBase>(
	//		data.Class, spawnLocation, mainPlayer->GetActorRotation(), spawnParams);

	//	if (character)
	//	{
	//		character->SpawnDefaultController();
	//		m_PlayerParty.Add(character);
	//	}
	//}

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

		// プレイヤーと仲間を生成
		ACharacterBase* character = world
			? world->SpawnActor<ACharacterBase>(
				data.Class,
				spawnLocation,
				FRotator::ZeroRotator,
				spawnParams)
			: nullptr;

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
