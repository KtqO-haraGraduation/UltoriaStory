// Fill out your copyright notice in the Description page of Project Settings.
#include "CharacterBase.h"
#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Ally/AllyDataAsset.h"
#include "Ally/AllyDataRow.h"
#include "../Battle/Combat/Components/CombatComponent.h"
#include "../Battle/Combat/Components/Status/CombatStatusComponent.h"
#include <fstream>
#include <iostream>

#define ENEMY_MAX 3

using namespace std;

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Level = 0;
	m_Exp = 0;
	m_BaseStatus.m_TotalEXP = 0;
	m_BaseStatus.m_HP = 0;
	m_BaseStatus.m_MP = 0;
	m_BaseStatus.m_ATK = 0;
	m_BaseStatus.m_MGC = 0;
	m_BaseStatus.m_DEF = 0;
	m_BaseStatus.m_SPD = 0;
	m_BaseStatus.m_Break = 0;
	m_BaseStatus.m_BreakGauge = 0;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// コンポーネントリストに全てのコンポーネントを格納
	GetComponents<UComponentBase>(m_Components);
	InitComponent();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/// <summary>
/// 味方ステータスのバイナリファイルをロード
/// </summary>
/// <param name="status">
/// 読み込んだ味方ステータスを格納する構造体
/// </param>
/// <param name="type">
/// 取得したい味方データを指定
/// </param>
void ACharacterBase::LoadAllyStatusData(FStatus status[], EPlayerType type)
{
	TArray<uint8> binaryData;	// バイト列

	// ファイルを開く
	FString path = FPaths::ProjectContentDir() + TEXT("Data/Status/AllyStatus.bin");
	if (!FFileHelper::LoadFileToArray(binaryData, *path))
	{
		// ファイルが開けなかった
		UE_LOG(LogTemp, Error, TEXT("AllyStatus/Read Failed"));
		return;
	}

	// Reader作成
	FMemoryReader ar(binaryData);	// バイト列を読み込む(ストリーム化)
	ar.Seek(0);	// 読み込り順を0番目に指定

	// 読み込み
	for (int i = 1; i <= LEVEL_MAX; i++)
	{
		ar << status[i].m_TotalEXP;
		ar << status[i].m_HP;
		ar << status[i].m_MP;
		ar << status[i].m_ATK;
		ar << status[i].m_MGC;
		ar << status[i].m_DEF;
		ar << status[i].m_SPD;
		ar << status[i].m_Break;
		ar << status[i].m_BreakGauge;
	}

	// データアセットをロード
	UAllyDataAsset* allyDA = LoadObject<UAllyDataAsset>(nullptr, TEXT("/Script/UltoriaProject.JobDataAsset'/Game/MembarFiles/Sakumoto/DataAsset/DA_AllyData.DA_AllyData'"));

	if (!allyDA)
	{
		// ロード失敗
		UE_LOG(LogTemp, Error, TEXT("AllyDataAsset/false"));
		return;
	}
	// 味方データのリスト
	TArray<FAllyDataRow*> rows;
	rows.Empty();
	allyDA->m_AllyDataMappingTable->GetAllRows<FAllyDataRow>(TEXT(""), rows);

	// 引数で指定した味方データを取得
	m_Name = rows[static_cast<int32>(type)]->Name;
}

/// <summary>
/// 敵ステータスのバイナリファイルをロード
/// </summary>
/// <param name="status">
/// 読み込んだ敵ステータスを格納する構造体
/// </param>
void ACharacterBase::LoadEnemyStatusData(FStatus status, int id)
{
	TArray<uint8> binaryData;	// バイト列

	// ファイルを開く
	FString path = FPaths::ProjectContentDir() + TEXT("Data/Status/EnemyStatus.bin");
	if (!FFileHelper::LoadFileToArray(binaryData, *path))
	{
		// ファイルが開けなかった
		UE_LOG(LogTemp, Error, TEXT("EnemyStatus/Read Failed"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("EnemyStatus/Read Success"));		// 成功
	UE_LOG(LogTemp, Warning, TEXT("Size = %d"), binaryData.Num());	// ファイルサイズ

	// Reader作成
	FMemoryReader ar(binaryData);	// バイト列を読み込む(ストリーム化)
	ar.Seek(0);						// 読み込り順を0番目に指定

	// 読み込み
	for (int i = 1; i <= ENEMY_MAX; i++)
	{
		ar << status.m_TotalEXP;
		ar << status.m_HP;
		ar << status.m_MP;
		ar << status.m_ATK;
		ar << status.m_MGC;
		ar << status.m_DEF;
		ar << status.m_SPD;
		ar << status.m_Break;
		ar << status.m_BreakGauge;

		if (i == id)
		{
			break;
		}
	}
}

/// <summary>
/// キャラクターのステータスを上げる
/// m_Expが増えたら呼び出される
/// 現在のEXPが次のレベルの累計EXPを超えたらtrueを返す
/// </summary>
bool ACharacterBase::CheckLevelUp(int nextLevelTotalExp)
{
	// 現在のEXPが次のレベルの累計EXPを超えたか
	if (m_Exp >= nextLevelTotalExp)
	{
		return true;	// レベルが上がった
	}
	return false;		// 現状維持
}

/// <summary>
/// キャラクターのステータスを上げる
/// m_Expがm_TotalEXPを超えたら呼び出される
/// </summary>
void ACharacterBase::LevelUp(FStatus baseStatus, FStatus status[])
{
	m_Level++;

	// ステータスを設定
	SetStatus(&baseStatus,	status[m_Level].m_TotalEXP,
							status[m_Level].m_HP,
							status[m_Level].m_MP,
							status[m_Level].m_ATK,
							status[m_Level].m_MGC,
							status[m_Level].m_DEF,
							status[m_Level].m_SPD,
							status[m_Level].m_Break,
							status[m_Level].m_BreakGauge);
}

/// <summary>
/// コンポーネントの初期化処理
/// </summary>
void ACharacterBase::InitComponent()
{
	UCombatComponent* combat = GetComponent<UCombatComponent>();
	UCombatStatusComponent* status = GetComponent<UCombatStatusComponent>();

	if (combat && status)
	{
		combat->SetActiveComponent(false);
		status->SetActiveComponent(false);
	}
}

float ACharacterBase::GetHPRate()
{
	return (float)m_BaseStatus.m_HP / m_MaxHP;
}

float ACharacterBase::GetMPRate()
{
	return (float)m_BaseStatus.m_MP / m_MaxMP;
}

// Set関数
// br = break
void ACharacterBase::SetStatus(FStatus* status, int exp, int hp, int mp, int atk, int mgc, int def, int spd, int br, int brGauge)
{
	status->m_TotalEXP	 = exp;
	status->m_HP		 = hp;
	status->m_MP		 = mp;
	status->m_ATK		 = atk;
	status->m_MGC		 = mgc;
	status->m_DEF		 = def;
	status->m_SPD		 = spd;
	status->m_Break		 = br;
	status->m_BreakGauge = brGauge;
}

void ACharacterBase::SetHP(int newHP)
{
	m_BaseStatus.m_HP = FMath::Clamp(newHP, 0, m_MaxHP);
	m_OnHPChanged.Broadcast();
}

void ACharacterBase::SetMP(int newMP)
{
	m_BaseStatus.m_MP = FMath::Clamp(newMP, 0, m_MaxMP);
	m_OnMPChanged.Broadcast();
}
