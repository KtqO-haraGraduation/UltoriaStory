// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentBase.h"
#include "../../../Character/CharacterBase.h"

// Sets default values for this component's properties
UComponentBase::UComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// コンポーネントがついているキャラクター（オーナー）を取得
	m_Owner = Cast<ACharacterBase>(GetOwner());
}


// Called every frame
void UComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

