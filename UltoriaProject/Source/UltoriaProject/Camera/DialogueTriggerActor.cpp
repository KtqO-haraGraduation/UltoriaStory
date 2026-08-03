// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueTriggerActor.h"
#include "../TestPlayer/TestPlayer.h"

ADialogueTriggerActor::ADialogueTriggerActor()
{
    m_TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = m_TriggerBox;
    m_TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    m_TriggerBox->SetGenerateOverlapEvents(true);

    m_EventScriptEngine = CreateDefaultSubobject<UEventScriptEngine>(TEXT("EventScriptEngine"));
}

void ADialogueTriggerActor::BeginPlay()
{
    Super::BeginPlay();
    m_TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADialogueTriggerActor::TriggerBeginOverlap);
}

void ADialogueTriggerActor::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
    {
        ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
        if (player)
        {
            player->SetCurrentScriptEngine(m_EventScriptEngine);
        }
        m_EventScriptEngine->StartScript(m_EventScript);
    }
}
