// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"
#include "../../BattleGaugeManager.h"
#include "Status/CombatStatusComponent.h"
#include "../../../Character/CharacterBase.h"
#include "../../../UI/Battle/DamageWidget.h"
#include "../Data/BattleDataManager.h"

// Sets default values for this component's properties
UPlayerCombatComponent::UPlayerCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerCombatComponent::OnAttackHit()
{
	if (!m_Target) return;
	int damage = m_Owner->GetBaseStatus().m_ATK;
	UCombatComponent* combat = m_Target->GetComponent<UCombatComponent>();

	// ウルトリアゲージが満タンならダメージを倍にする
	UCombatStatusComponent* status = m_Owner->GetComponent<UCombatStatusComponent>();
	if (status)
	{
		UBattleDataManager* dataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
		FBattleStatusData data;

		// ガード値を取得
		data = dataMg->FindStatusData("Ultoria");
		damage *= data.value;
	}

	combat->TakeDamage(damage);
}

