// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "../UI/Menu/MenuStateManager.h"
#include "EnhancedInputComponent.h"


// Sets default values
ATestPlayer::ATestPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController)
	{
		ULocalPlayer* localPlayer = playerController->GetLocalPlayer();
		if (localPlayer)
		{
			localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(m_InputMap, 0);
			if (localPlayer)
			{
				UEnhancedInputLocalPlayerSubsystem* inputSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
				if (inputSystem)
				{
					inputSystem->AddMappingContext(m_InputMap, 0);
				}
			}
		}
	}
}

// Called every frame
void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInput)
	{
		enhancedInput->BindAction(m_InputMove, ETriggerEvent::Triggered, this, &ATestPlayer::InputMove);
		enhancedInput->BindAction(m_InputIntaract, ETriggerEvent::Started, this, &ATestPlayer::TrySpeakNPC);
		enhancedInput->BindAction(m_InputTalkEnter, ETriggerEvent::Started, this, &ATestPlayer::NextSpeak);
		enhancedInput->BindAction(m_InputOpenMenu, ETriggerEvent::Started, this, &ATestPlayer::OpenMenu);
		enhancedInput->BindAction(m_InputCloseMenu, ETriggerEvent::Started, this, &ATestPlayer::CloseMenu);
		enhancedInput->BindAction(m_InputMenuNavigate, ETriggerEvent::Started, this, &ATestPlayer::MenuNavigate);
		enhancedInput->BindAction(m_InputMenuConfirm, ETriggerEvent::Started, this, &ATestPlayer::MenuConfirm);
	}
}

void ATestPlayer::InputMove(const FInputActionValue& Value)
{
	FVector2D moveVec2D = Value.Get<FVector2D>();

	FVector moveVec = FVector(moveVec2D.Y, moveVec2D.X, 0.0f);

	AddMovementInput(moveVec, m_speed);
}

void ATestPlayer::TrySpeakNPC()
{
	if (!m_NearNPC) return;
	m_NearNPC->Speak();
}

void ATestPlayer::NextSpeak()
{
	if (!m_CurrentScriptEngine) return;

	if (m_CurrentScriptEngine->IsTyping())
	{
		m_CurrentScriptEngine->SkipTyping();
		return;
	}
	m_CurrentScriptEngine->NextCommand();
}

void ATestPlayer::OpenMenu()
{
	UE_LOG(LogTemp, Log, TEXT("OpenMenu!!!!!!!!!!!"));

	UMenuStateManager* menuManager = GetGameInstance()->GetSubsystem<UMenuStateManager>();
	if (menuManager)
	{
		menuManager->OpenMenu(EMenuState::MENU_MAIN);
	}
}

void ATestPlayer::CloseMenu()
{
	UE_LOG(LogTemp, Log, TEXT("OpenMenu!!!!!!!!!!!"));
	UMenuStateManager* menuManager = GetGameInstance()->GetSubsystem<UMenuStateManager>();
	if (menuManager)
	{
		menuManager->CloseMenu();
	}
}

void ATestPlayer::MenuNavigate(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("MenuNavigate!!!!!!!!!!!"));

	float axisValue = Value.Get<float>();
	UMenuStateManager* menuManager = GetGameInstance()->GetSubsystem<UMenuStateManager>();

	if (!menuManager)return;

	if (axisValue > 0.0f)
	{
		menuManager->InputNavigateUp();
	}
	else if (axisValue < 0.0f)
	{
		menuManager->InputNavigateDown();
	}
}

void ATestPlayer::MenuConfirm()
{
	UMenuStateManager* menuManager = GetGameInstance()->GetSubsystem<UMenuStateManager>();

	if (menuManager)
	{
		menuManager->InputConfirm_Menu();
	}
}

void ATestPlayer::EquipItem(EEquipSlot Slot, FName id)
{
	m_EquippedIds.Add(Slot, id);
}

FName ATestPlayer::GetEquippedId(EEquipSlot Slot)
{
	FName* id = m_EquippedIds.Find(Slot);
	if (id) return *id;
	return NAME_None;
}
