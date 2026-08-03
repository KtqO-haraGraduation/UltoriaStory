// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCombatComponent.h"
#include "CombatComponent.h"
#include "Status/CombatStatusComponent.h"
#include "../../BattleGaugeManager.h"
#include "../../../Character/CharacterBase.h"
#include "../../../Character/AI/Patrol/PatrolComponent.h"
#include "../../../Character/AI/Field/Enemy/EnemyFieldAIComponent.h"

// Sets default values for this component's properties
UEnemyCombatComponent::UEnemyCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEnemyCombatComponent::BattleStart()
{
	UCombatComponent::BattleStart();

	UPatrolComponent* patrol = m_Owner->GetComponent<UPatrolComponent>();
	patrol->SetActiveComponent(false);

	UEnemyFieldAIComponent* field = m_Owner->GetComponent<UEnemyFieldAIComponent>();
	field->SetActiveComponent(false);


}

void UEnemyCombatComponent::BattleEnd()
{
	UCombatComponent::BattleEnd();

	UPatrolComponent* patrol = m_Owner->GetComponent<UPatrolComponent>();
	patrol->SetActiveComponent(true);

	UEnemyFieldAIComponent* field = m_Owner->GetComponent<UEnemyFieldAIComponent>();
	field->SetActiveComponent(true);
}

void UEnemyCombatComponent::TakeDamage(int damage)
{
	UCombatStatusComponent* statusComponent = m_Owner->GetComponent<UCombatStatusComponent>();
	bool breaking = statusComponent->IsBreaking();
	


	//FActiveStatus* Break = m_Owner->GetStatus()->FindStatus(StatusType::BREAK);
	float breakValue = 1.0f;
	/*if (breaking)
	{
		breakValue = Break->value;
	}*/

	//int newDamage = damage * breakValue;

	UCombatComponent::TakeDamage(damage);

	UBattleGaugeManager* gaugeManager = GetWorld()->GetSubsystem<UBattleGaugeManager>();
	if (gaugeManager)
	{
		gaugeManager->AddEnemyBreakGauge(20);
	}

	// ブレイクフラグを立たせる
	if (gaugeManager->GetEnemyBreakGaugePercent() >= 1.0f)
	{
		statusComponent->SetBreaking(true);
	}
}


// Called when the game starts
void UEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

