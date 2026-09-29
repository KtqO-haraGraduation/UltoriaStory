// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterBase.h"
#include "../../Job/JobDataComponent.h"
#include "../Data/AllyMasterData.h"
#include "../Data/CharacterDataManager.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_TeamType = ETeamType::PLAYER;
	m_Job = FindComponentByClass<UJobDataComponent>();

	SetJob();
	SetBaseStatus();
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FStatus APlayerCharacterBase::GetTotalStatus() const
{
	// 基本値 + 装備補正値 * ジョブ補正値(倍率)
	FStatus result = {};

	if (!m_Job) return result;

	result.m_HP = (int)((m_BaseStatus.m_HP + m_EquipmentBonus.m_HP) * m_Job->GetHP());
	result.m_MP = (int)((m_BaseStatus.m_MP + m_EquipmentBonus.m_MP) * m_Job->GetMP());
	result.m_ATK = (int)((m_BaseStatus.m_ATK + m_EquipmentBonus.m_ATK) * m_Job->GetATK());
	result.m_DEF = (int)((m_BaseStatus.m_DEF + m_EquipmentBonus.m_DEF) * m_Job->GetDEF());
	result.m_MGC = (int)((m_BaseStatus.m_MGC + m_EquipmentBonus.m_MGC) * m_Job->GetMGC());
	result.m_SPD = (int)((m_BaseStatus.m_SPD + m_EquipmentBonus.m_SPD) * m_Job->GetSPD());

	return result;
}

void APlayerCharacterBase::SetJob()
{
	UCharacterDataManager* characterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!characterDataMg || !m_Job) return;

	FAllyMasterData masterData = characterDataMg->GetAllyMasterData(m_ID);
	m_Job->SetJobData(masterData.JobType);
}

void APlayerCharacterBase::ChangeJob(FName jobName)
{
	m_Job->SetJobData(jobName);
}
