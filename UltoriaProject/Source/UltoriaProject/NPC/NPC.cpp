// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "../TestPlayer/TestPlayer.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	m_InteractWidgetComponent->SetupAttachment(RootComponent);
	m_InteractWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	m_InteractWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	m_EventScriptEngine = CreateDefaultSubobject<UEventScriptEngine>(TEXT("EventScriptEngine"));
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	m_InteractWidgetComponent->SetVisibility(false);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPC::ApprochBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor && otherActor != this)
	{
		if (!m_CanSpeak)
		{
			ATestPlayer* player = Cast<ATestPlayer>(otherActor);
			if (player)
			{
				player->SetNearNPC(this);
				player->SetCurrentScriptEngine(m_EventScriptEngine);
			}

			m_CanSpeak = true;
			ShowIntaractUI();
		}
	}
}

void ANPC::ApprochEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("NPC End Overlap!!"));

	if (m_CanSpeak)
	{
		ATestPlayer* player = Cast<ATestPlayer>(otherActor);
		if (player)
		{
			player->SetNearNPC(nullptr);
			player->SetCurrentScriptEngine(nullptr);
		}
		m_CanSpeak = false;
		HideIntaractUI();
	}
}

void ANPC::Speak()
{
	UE_LOG(LogTemp, Log, TEXT("Speak!!!!!!!!!"));

	if (m_CanSpeak && !m_EventScriptEngine->GetEventPlay())
	{
		m_EventScriptEngine->StartScript(m_EventScript);
		HideIntaractUI();
	}
}

void ANPC::ShowIntaractUI()
{
	m_InteractWidgetComponent->SetVisibility(true);
}

void ANPC::HideIntaractUI()
{
	m_InteractWidgetComponent->SetVisibility(false);
}

