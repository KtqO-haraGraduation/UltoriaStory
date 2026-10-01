// Fill out your copyright notice in the Description page of Project Settings.


#include "AllyCharacter.h"
#include "../../Job/JobDataSubsystem.h"
#include "../../Battle/Combat/Components/PlayerCombatComponent.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Job/JobDataComponent.h"
#include "../Data/AllyMasterData.h"
#include "../Data/CharacterDataManager.h"

// Sets default values
AAllyCharacter::AAllyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//m_JobBase = nullptr;
	//m_JobData = nullptr;
	//m_JobId = 0;
}

// Called when the game starts or when spawned
void AAllyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//m_TeamType = ETeamType::PLAYER;
	//
	////JobChange();
	//

	//SetBaseStatus();
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


//// ƒWƒ‡ƒu‚ÌØ‚è‘Ö‚¦
//void AAllyCharacter::JobChange()
//{
//	UJobDataSubsystem* jobDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UJobDataSubsystem>();
//	if (!jobDataSubsystem)
//	{
//		return;	// UJobDataSubsystem‚ÌŽæ“¾Ž¸”s
//	}
//	m_JobData = jobDataSubsystem->GetJobData(m_JobId);
//	m_JobBase = jobDataSubsystem->GetJobBase(m_JobId);
//}

