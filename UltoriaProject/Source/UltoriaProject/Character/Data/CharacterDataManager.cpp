// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataManager.h"
#include "CharacterDataAsset.h"

void UCharacterDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// キャラクターのデータアセットをロード
	LoadCharacterDataAsset();

	// 必要なキャラデータをロード
	LoadAllyMasterData();
	LoadAllyLevelData();

	LoadEnemyData();

	LoadSkillData();

	LoadJobData();
	LoadJobLvData();
}

/// <summary>
/// キャラクターのデータアセットをロード
/// </summary>
void UCharacterDataManager::LoadCharacterDataAsset()
{
	m_CharacterDataAsset = LoadObject<UCharacterDataAsset>(
		nullptr,
		TEXT("/Game/Data/Character/DA_Character.DA_Character")
	);
}

/// <summary>
/// プレイヤーと仲間のマスターデータをロード
/// </summary>
void UCharacterDataManager::LoadAllyMasterData()
{
	UDataTable* table = m_CharacterDataAsset->AllyTable;
	if (!table) return;

	m_AllyMasterDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FAllyMasterData* data = reinterpret_cast<FAllyMasterData*>(row.Value);

		if (data)
		{
			m_AllyMasterDatas.Add(rowName, *data);
		}
	}
}

/// <summary>
/// 仲間のLVごとのデータをロード
/// </summary>
void UCharacterDataManager::LoadAllyLevelData()
{
	UDataTable* table = m_CharacterDataAsset->AllyLevelTable;
	if (!table) return;

	m_AllyLevelDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FAllyLevelData* data = reinterpret_cast<FAllyLevelData*>(row.Value);

		if (data)
		{
			m_AllyLevelDatas.Add(rowName, *data);
		}
	}
}

/// <summary>
/// 敵のデータをロード
/// </summary>
void UCharacterDataManager::LoadEnemyData()
{
	UDataTable* table = m_CharacterDataAsset->EnemyTable;
	if (!table) return;

	m_EnemyDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FEnemyMasterData* data = reinterpret_cast<FEnemyMasterData*>(row.Value);

		if (data)
		{
			m_EnemyDatas.Add(rowName, *data);
		}
	}
}

/// <summary>
/// 敵のパーティーデータをロード
/// </summary>
void UCharacterDataManager::LoadEnemyPartyData()
{
	UDataTable* table = GetEnemyPartyTable();
	if (!table) return;

	m_EnemyPartyDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FName rowName = row.Key;
		FEnemyPartyData* data = reinterpret_cast<FEnemyPartyData*>(row.Value);

		if (data)
		{
			FEnemyPartyData copy = *data;

			copy.Name = rowName;

			m_EnemyPartyDatas.Add(copy);
		}
	}
}

/// <summary>
/// スキルデータをロード
/// </summary>
void UCharacterDataManager::LoadSkillData()
{
	UDataTable* table = m_CharacterDataAsset->SkillTable;
	if (!table) return;

	m_SkillDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FSkillDataRow* data = reinterpret_cast<FSkillDataRow*>(row.Value);

		if (data)
		{
			m_SkillDatas.Add(data->Name, *data);
		}
	}
}

/// <summary>
/// ジョブデータをロード
/// </summary>
void UCharacterDataManager::LoadJobData()
{
	UDataTable* table = m_CharacterDataAsset->JobTable;
	if (!table) return;

	m_JobDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FJobDataRow* data = reinterpret_cast<FJobDataRow*>(row.Value);

		if (data)
		{
			m_JobDatas.Add(data->Name, *data);
		}
	}
}

/// <summary>
/// ジョブのEXPのデータをロード
/// </summary>
void UCharacterDataManager::LoadJobLvData()
{
	UDataTable* table = m_CharacterDataAsset->JobLvTable;
	if (!table) return;

	m_JobLvDatas.Empty();

	for (const auto& row : table->GetRowMap())
	{
		FString rowName = row.Key.ToString();

		FString lvString;
		if (!rowName.Split(TEXT("_"), nullptr, &lvString)) continue;

		int jobLv = FCString::Atoi(*lvString);

		FJobLvRow* data = reinterpret_cast<FJobLvRow*>(row.Value);

		if (data)
		{
			m_JobLvDatas.Add(jobLv, *data);
		}
	}
}

/// <summary>
/// 指定した仲間のLVデータを取得
/// </summary>
/// <param name="allyId">キャラクターID</param>
/// <param name="Lv">ほしいデータのLV</param>
/// <returns>指定したLVでデータ</returns>
FAllyLevelData UCharacterDataManager::GetAllyLevelData(FName allyId, int Lv) const
{
	FName rowName(
		*FString::Printf(TEXT("%s_%02d"), *allyId.ToString(), Lv)
	);

	const FAllyLevelData* data = m_AllyLevelDatas.Find(rowName);

	if (data)
	{
		return *data;
	}
	return FAllyLevelData();
}

/// <summary>
/// 指定した仲間のマスターデータを取得
/// </summary>
/// <param name="allyId">敵のID</param>
/// <returns>指定した仲間のマスターデータ</returns>
FAllyMasterData UCharacterDataManager::GetAllyMasterData(FName allyId) const
{
	const FAllyMasterData* data = m_AllyMasterDatas.Find(allyId);

	if (data)
	{
		return *data;
	}
	return FAllyMasterData();
}

/// <summary>
/// 指定した敵のデータを取得
/// </summary>
/// <param name="enemyId">敵のID</param>
/// <returns>指定した敵のデータ</returns>
FEnemyMasterData UCharacterDataManager::GetEnemyData(FName enemyId) const
{
	const FEnemyMasterData* data = m_EnemyDatas.Find(enemyId);

	if (data)
	{
		return *data;
	}
	return FEnemyMasterData();
}

/// <summary>
/// 指定した敵クラスを取得
/// </summary>
/// <param name="enemyId">敵のID</param>
/// <returns>指定した敵のクラス</returns>
TSubclassOf<AEnemyBase> UCharacterDataManager::GetEnemyClass(FName enemyId)
{
	const FEnemyMasterData* data = m_EnemyDatas.Find(enemyId);

	if (data)
	{
		return data->EnemyClass;
	}
	return nullptr;
}

/// <summary>
/// IDを元にランダムで敵のパーティーを取得
/// </summary>
/// <param name="enemyId">敵のID</param>
/// <returns>敵のパーティー</returns>
FEnemyPartyData UCharacterDataManager::GetRandomEnemyParty(FName enemyId)
{
	TArray<FEnemyPartyData> candidates =
		FindEnemyParty(enemyId);

	if (candidates.Num() == 0)
	{
		return FEnemyPartyData();
	}

	int index = FMath::RandRange(0, candidates.Num() - 1);

	return candidates[index];
}

/// <summary>
/// IDが含まれたデータリスト
/// </summary>
/// <param name="enemyId">敵のID</param>
/// <returns>IDが含まれるデータリスト</returns>
TArray<FEnemyPartyData> UCharacterDataManager::FindEnemyParty(FName enemyId)
{
	TArray<FEnemyPartyData> result;

	for (const FEnemyPartyData& data : m_EnemyPartyDatas)
	{
		if (data.Enemies.Contains(enemyId))
		{
			result.Add(data);
		}
	}
	return result;
}


/// <summary>
/// 指定したスキルデータを取得
/// </summary>
/// <param name="name">スキルの名前</param>
/// <returns></returns>
const FSkillDataRow* UCharacterDataManager::GetSkillData(FName name)
{
	return m_SkillDatas.Find(name);
}

/// <summary>
/// 指定したジョブのデータを取得
/// </summary>
/// <param name="jobName">ジョブの名前</param>
/// <returns></returns>
FJobDataRow UCharacterDataManager::GetJobData(FName jobName)
{
	const FJobDataRow* data = m_JobDatas.Find(jobName);

	if (data)
	{
		return *data;
	}
	return FJobDataRow();
}

/// <summary>
/// 指定したジョブの解放に必要な経験値の値のデータを返す
/// </summary>
/// <param name="nowJobLv">次のLv</param>
/// <returns></returns>
FJobLvRow UCharacterDataManager::GetJobLvData(int nexJobLv)
{
	const FJobLvRow* data = m_JobLvDatas.Find(nexJobLv);

	if (data)
	{
		return *data;
	}
	return FJobLvRow();
}

/// <summary>
/// 現在のステージを元に、敵のパーティーデータテーブルを取得
/// </summary>
/// <returns>敵のデータテーブル</returns>
UDataTable* UCharacterDataManager::GetEnemyPartyTable()
{
	UStageManager* stageMg = GetWorld()->GetSubsystem<UStageManager>();
	if (!stageMg) return nullptr;

	TObjectPtr<UDataTable>* table = 
		m_CharacterDataAsset->EnemyPartyTable.Find(stageMg->GetStageType());
	if (!table) return nullptr;

	return *table;
}
