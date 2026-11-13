// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerActor.h"
#include "../Event/EventScriptEngine.h"
#include "../Character/Ally/MainPlayer.h"

// Sets default values
AEventTriggerActor::AEventTriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_EventScriptEngine = CreateDefaultSubobject<UEventScriptEngine>(TEXT("EventScriptEngine"));
}

// Called when the game starts or when spawned
void AEventTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventTriggerActor::BeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (!otherActor || otherActor == this || !m_EventScriptEngine) return;

	// プレイヤー以外が触れても発動しない
	if (!Cast<AMainPlayer>(otherActor)) return;

	if (!m_TriggerID.IsNone())
	{
		// ストーリーに載っているトリガー。今の行と合わなければ何も起きない
		m_EventScriptEngine->StartStoryEvent(m_TriggerID);
	}
	else
	{
		// ストーリーに載っていない従来のトリガー
		m_EventScriptEngine->StartScript(m_EventScript);
	}
}

