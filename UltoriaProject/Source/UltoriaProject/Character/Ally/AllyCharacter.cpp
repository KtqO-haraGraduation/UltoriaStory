// Fill out your copyright notice in the Description page of Project Settings.


#include "AllyCharacter.h"
#include "../../Job/JobDataSubsystem.h"
#include "../../Battle/Combat/Components/PlayerCombatComponent.h"
#include "../../Battle/PlayerPartyManager.h"

// Sets default values
AAllyCharacter::AAllyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_JobBase = nullptr;
	m_JobData = nullptr;
	m_JobId = 0;
}

// Called when the game starts or when spawned
void AAllyCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_TeamType = ETeamType::PLAYER;
	JobChange();
	SetBaseStatus();
}

// Called every frame
void AAllyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAllyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 総合のステータス
FStatus AAllyCharacter::GetTotalStatus() const
{
	// 基本値 + 装備補正値 + ジョブ補正値
	FStatus result = {};

	if (!m_JobData) return result;

	result.m_HP  = (int)((m_BaseStatus.m_HP  + m_EquipmentBonus.m_HP)  * m_JobData->m_HP);
	result.m_MP  = (int)((m_BaseStatus.m_MP  + m_EquipmentBonus.m_MP)  * m_JobData->m_MP);
	result.m_ATK = (int)((m_BaseStatus.m_ATK + m_EquipmentBonus.m_ATK) * m_JobData->m_ATK);
	result.m_DEF = (int)((m_BaseStatus.m_DEF + m_EquipmentBonus.m_DEF) * m_JobData->m_DEF);
	result.m_MGC = (int)((m_BaseStatus.m_MGC + m_EquipmentBonus.m_MGC) * m_JobData->m_MGC);
	result.m_SPD = (int)((m_BaseStatus.m_SPD + m_EquipmentBonus.m_SPD) * m_JobData->m_SPD);

	return result;
}

// ジョブの切り替え
void AAllyCharacter::JobChange()
{
	UJobDataSubsystem* jobDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UJobDataSubsystem>();
	if (!jobDataSubsystem)
	{
		return;	// UJobDataSubsystemの取得失敗
	}
	m_JobData = jobDataSubsystem->GetJobData(m_JobId);
	m_JobBase = jobDataSubsystem->GetJobBase(m_JobId);
}