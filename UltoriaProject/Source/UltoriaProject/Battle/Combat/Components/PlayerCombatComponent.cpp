// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"
#include "../../BattleGaugeManager.h"

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

void UPlayerCombatComponent::TakeDamage(int damage)
{
	UCombatComponent::TakeDamage(damage);

	// お試し（ウルトリアゲージ）
	UBattleGaugeManager* gaugeManager = GetWorld()->GetSubsystem<UBattleGaugeManager>();
	if (gaugeManager)
	{
		gaugeManager->AddUltoriaGauge(5);
	}
}

