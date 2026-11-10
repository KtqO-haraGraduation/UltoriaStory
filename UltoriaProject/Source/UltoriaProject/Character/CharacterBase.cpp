// Fill out your copyright notice in the Description page of Project Settings.
#include "CharacterBase.h"
#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Ally/AllyDataAsset.h"
#include "Ally/AllyDataRow.h"
#include "../Battle/Combat/Components/CombatComponent.h"
#include "../Battle/Combat/Components/Status/CombatStatusComponent.h"
#include "Data/CharacterDataManager.h"
#include "Components/WidgetComponent.h"

using namespace std;

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Name = "";
	m_Level = 0;
	m_Exp = 0;
	m_MaxHP = 0;
	m_MaxMP = 0;
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
/// キャラクターのステータスを上げる
/// m_Expが増えたら呼び出される
/// 現在のEXPが次のレベルの累計EXPを超えたらtrueを返す
/// </summary>
bool ACharacterBase::CheckLevelUp(int nextLevelTotalExp)
{
	UCharacterDataManager* dataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!dataMg) return false;

	int nextLv = m_Level + 1;
	FAllyLevelData lvData = dataMg->GetAllyLevelData(m_ID, nextLv);

	// 現在のEXPが次のレベルの累計EXPを超えたか
	if (m_Exp >= lvData.Exp)
	{
		return true;	// レベルが上がった
	}
	return false;		// 現状維持
}

/// <summary>
/// キャラクターのステータスを上げる
/// m_Expがm_TotalEXPを超えたら呼び出される
/// </summary>
void ACharacterBase::LevelUp()
{
	m_Level++;

	// ステータスを設定
	SetBaseStatus();
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
	return (float)m_CurrentHP / m_MaxHP;
}

float ACharacterBase::GetMPRate()
{
	return (float)m_CurrentMP / m_MaxMP;
}

void ACharacterBase::SetBaseStatus()
{
	// Lvが0以下だったら1にする
	if (m_Level <= 0)
	{
		m_Level = 1;
	}

	UCharacterDataManager* dataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!dataMg) return;

	FAllyLevelData levelData = dataMg->GetAllyLevelData(m_ID, m_Level);
	FAllyMasterData masterData = dataMg->GetAllyMasterData(m_ID);

	m_Name = masterData.Name.ToString();

	m_BaseStatus.m_TotalEXP = levelData.Exp;
	m_BaseStatus.m_HP = levelData.HP;
	m_BaseStatus.m_MP = levelData.MP;
	m_BaseStatus.m_ATK = levelData.ATK;
	m_BaseStatus.m_MGC = levelData.MGC;
	m_BaseStatus.m_DEF = levelData.DEF;
	m_BaseStatus.m_SPD = levelData.SPD;
	m_BaseStatus.m_Break = levelData.Break;
	m_BaseStatus.m_BreakGauge = levelData.BreakGauge;

	FStatus totalStatus = GetTotalStatus();

	m_MaxHP = totalStatus.m_HP;
	m_MaxMP = totalStatus.m_MP;

	m_CurrentHP = totalStatus.m_HP;
	m_CurrentMP = totalStatus.m_MP;
}

void ACharacterBase::SetEquipmentBonus(FStatus bonus)
{
	m_EquipmentBonus = bonus;
}

void ACharacterBase::SetHP(int newHP)
{
	m_CurrentHP = FMath::Clamp(newHP, 0, m_MaxHP);
	m_OnHPChanged.Broadcast();
}

void ACharacterBase::SetMP(int newMP)
{
	m_CurrentMP = FMath::Clamp(newMP, 0, m_MaxMP);
	m_OnMPChanged.Broadcast();
}

void ACharacterBase::SetSelectionUIVisibility(bool flag)
{
	if (!m_SelectionUIWidget) return;
	m_SelectionUIWidget->SetVisibility(flag);
}

void ACharacterBase::AddEquipItem(EEquipSlot Slot, FName id)
{
	m_EquippedIds.Add(Slot, id);
}

FName ACharacterBase::GetEquippedId(EEquipSlot Slot)
{
	FName* id = m_EquippedIds.Find(Slot);
	if (id) return *id;
	return NAME_None;
}
