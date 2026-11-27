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

void UPlayerCombatComponent::TakeDamage(int damage)
{
	UCombatComponent::TakeDamage(damage);

	// ダメージを受けたらウルトリアゲージを増加
	UBattleGaugeManager* gaugeMg = GetWorld()->GetSubsystem<UBattleGaugeManager>();
	if (gaugeMg)
	{
		gaugeMg->AddUltoriaGauge(20);
	}
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
	if (m_Targets.IsEmpty()) return;
	int damage = m_Owner->GetBaseStatus().m_ATK;

	for (ACharacterBase* target : m_Targets)
	{
		if (!target) continue;
		UCombatComponent* combat = target->GetComponent<UCombatComponent>();

		// ウルトリアゲージが満タンならダメージを倍にする
		UCombatStatusComponent* status = m_Owner->GetComponent<UCombatStatusComponent>();
		if (status)
		{
			if (status->IsUltoria())
			{
				UBattleDataManager* dataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
				FBattleStatusData data;

				// ガード値を取得
				data = dataMg->FindStatusData(StatusType::ULTORIA);
				damage *= data.value;
			}
		}

		combat->TakeDamage(damage);
	}
}

