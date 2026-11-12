// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/Combat/Selection/BattleSelectioinManager.h"
#include "../../UI/Menu/MenuStateManager.h"
#include "../../Character/Ally/MainPlayer.h"
#include "../../Akiduki/Shop/ShopManager.h"

void APlayPlayerController::ChangeInputMode(PlayInputMode mode)
{
	UEnhancedInputLocalPlayerSubsystem* subsystem =
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!subsystem) return;

	subsystem->ClearAllMappings();

	switch (mode)
	{
	case PlayInputMode::FIELD:
		subsystem->AddMappingContext(m_InputField, 0);
		break;
	case PlayInputMode::BATTLE:
		subsystem->AddMappingContext(m_InputBattle, 0);
		break;
	case PlayInputMode::MENU:
		subsystem->AddMappingContext(m_InputMenu, 0);
		break;
	case PlayInputMode::SHOP:
		subsystem->AddMappingContext(m_InputShop, 0);
	}

	m_CurrentInputMode = mode;
}

void APlayPlayerController::ChangeCursorMode(bool isShow)
{
	// マウスカーソルの表示 / 非表示 切り替え
	bShowMouseCursor = isShow;

	if (isShow)
	{
		FInputModeGameAndUI mode;
		mode.SetHideCursorDuringCapture(false);
		SetInputMode(mode);
	}
	else
	{
		FInputModeGameOnly mode;
		SetInputMode(mode);
	}
}

void APlayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(PlayInputMode::FIELD);
}

void APlayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent);
	if (!input)
	{
		return;
	}

	input->BindAction(m_InputMove, ETriggerEvent::Triggered, this, &APlayPlayerController::InputMove);
	input->BindAction(m_InputAttack, ETriggerEvent::Started, this, &APlayPlayerController::InputAttack);
	input->BindAction(m_InputInteract, ETriggerEvent::Started, this, &APlayPlayerController::InputInteract);
	input->BindAction(m_DirectionalInput,ETriggerEvent::Started,   this, &APlayPlayerController::InputDirectional);
	input->BindAction(m_InputMouseMove,ETriggerEvent::Triggered, this, &APlayPlayerController::InputMouseMove);
	input->BindAction(m_InputConfirn,ETriggerEvent::Started,   this, &APlayPlayerController::InputConfirm);
	input->BindAction(m_InputCancel,ETriggerEvent::Started,   this, &APlayPlayerController::InputCancel);
	input->BindAction(m_InputMenuOpen, ETriggerEvent::Started, this, &APlayPlayerController::InputOpen_Menu);
	input->BindAction(m_InputL1, ETriggerEvent::Started, this, &APlayPlayerController::InputL1_Menu);
	input->BindAction(m_InputL2, ETriggerEvent::Started, this, &APlayPlayerController::InputL2_Menu);
	input->BindAction(m_InputR1, ETriggerEvent::Started, this, &APlayPlayerController::InputR1_Menu);
	input->BindAction(m_InputR2, ETriggerEvent::Started, this, &APlayPlayerController::InputR2_Menu);
	input->BindAction(m_InputZoomCamera,ETriggerEvent::Triggered, this,&APlayPlayerController::InputZoomCamera);
	input->BindAction(m_InputZoomReset, ETriggerEvent::Started, this, &APlayPlayerController::InputZoomReset);
}

bool APlayPlayerController::InputKey(const FInputKeyEventArgs& Params)
{
	if (Params.Event == IE_Pressed)
	{
		bool isGamepad = Params.Key.IsGamepadKey();

		UpdateCursorByInput(isGamepad);
	}

	return Super::InputKey(Params);
}

void APlayPlayerController::UpdateCursorByInput(bool bGamepad)
{
	UBattleManager* battle =
		GetWorld()->GetSubsystem<UBattleManager>();

	// バトル中のみ判定
	if (battle && !battle->IsBattle()) return;

	// 入力媒体によって、マウスカーソルを表示 / 非表示 切り替え
	bGamepad ? ChangeCursorMode(false) : ChangeCursorMode(false);
}

void APlayPlayerController::InputMove(const FInputActionValue& Value)
{
	// 入力モードがフィールドでなければ
	if (!IsCurrentInputMode(PlayInputMode::FIELD)) return;

	AMainPlayer* player =
		Cast<AMainPlayer>(GetPawn());

	if (player)
	{
		player->Move(Value);
	}
}

void APlayPlayerController::InputDirectional(const FInputActionValue& Value)
{
	// 入力量
	FVector2D move = Value.Get<FVector2D>();
	// 入力タイプ
	EUIInputType type = EUIInputType::NONE;

	// 入力量から入力方向を決定
	if (move.Y > 0.5f)
	{
		type = EUIInputType::UP;
	}
	else if (move.Y < -0.5f)
	{
		type = EUIInputType::DOWN;
	}
	else if (move.X < -0.5f)
	{
		type = EUIInputType::LEFT;
	}
	else if (move.X > 0.5f)
	{
		type = EUIInputType::RIGHT;
	}

	// 入力モードがバトルなら
	if (IsCurrentInputMode(PlayInputMode::BATTLE))
	{
		InputDirectional_Battle(type); return;
	}	
	// 入力モードがメニューなら
	else if (IsCurrentInputMode(PlayInputMode::MENU))
	{
		InputDirectional_Menu(type); return;
	}
	// 入力モードがショップなら
	else if (IsCurrentInputMode(PlayInputMode::SHOP))
	{
		InputDirectional_Shop(type); return;
	}
		
}

void APlayPlayerController::InputMouseMove(const FInputActionValue& Value)
{
	FVector2D move = Value.Get<FVector2D>();

	if (!move.IsNearlyZero())
	{
		ChangeCursorMode(true);
	}
}

void APlayPlayerController::InputAttack(const FInputActionValue& Value)
{
	AMainPlayer* player =
		Cast<AMainPlayer>(GetPawn());

	if (player)
	{
		player->Attack(Value);
	}
}

void APlayPlayerController::InputZoomCamera(const FInputActionValue& Value)
{
	AMainPlayer* player =
		Cast<AMainPlayer>(GetPawn());

	if (player)
	{
		player->ZoomCamera(Value);
	}
}

void APlayPlayerController::InputZoomReset(const FInputActionValue& Value)
{
	AMainPlayer* player =
		Cast<AMainPlayer>(GetPawn());

	if (player)
	{
		player->ZoomReset(Value);
	}
}

void APlayPlayerController::InputInteract_Field()
{
	UShopManager* shop = GetWorld()->GetSubsystem<UShopManager>();

	if (!shop) return;

	shop->InputInteract();
}

void APlayPlayerController::InputInteract()
{
	// 入力モードがショップの場合
	if (IsCurrentInputMode(PlayInputMode::FIELD))
	{
		InputInteract_Field();
	}
}

void APlayPlayerController::InputConfirm()
{
	// 入力モードがバトルの場合
	if (IsCurrentInputMode(PlayInputMode::BATTLE))
	{
		InputConfirm_Battle(); return;
	}
	// 入力モードがメニューの場合
	else if (IsCurrentInputMode(PlayInputMode::MENU))
	{
		InputConfirm_Menu(); return;
	}
	// 入力モードがショップの場合
	else if (IsCurrentInputMode(PlayInputMode::SHOP))
	{
		InputConfirm_Shop(); return;
	}
}

void APlayPlayerController::InputCancel()
{
	// 入力モードがバトルの場合
	if (IsCurrentInputMode(PlayInputMode::BATTLE))
	{
		InputCancel_Battle(); return;
	}
	// 入力モードがメニューの場合
	else if (IsCurrentInputMode(PlayInputMode::MENU))
	{
		InputCancel_Menu(); return;
	}
	// 入力モードがショップの場合
	else if (IsCurrentInputMode(PlayInputMode::SHOP))
	{
		InputCancel_Shop(); return;
	}
}

void APlayPlayerController::InputConfirm_Battle()
{
	UBattleManager* battle = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battle) return;

	battle->GetSelectManager()->SelectConfirm();
}

void APlayPlayerController::InputCancel_Battle()
{
	UBattleManager* battle = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battle) return;

	battle->GetSelectManager()->SelectCancel();
}

void APlayPlayerController::InputDirectional_Battle(EUIInputType type)
{
	UBattleManager* battle = GetWorld()->GetSubsystem<UBattleManager>();

	if (!battle) return;

	battle->GetSelectManager()->SelectMoveTarget(type);
}

void APlayPlayerController::InputOpen_Menu()
{
	UMenuStateManager* menu = GetGameInstance()->GetSubsystem<UMenuStateManager>();

	if (!menu) return;

	menu->OpenMenu(EMenuState::MENU_MAIN);
}

void APlayPlayerController::InputConfirm_Menu()
{
	UMenuStateManager* menu = GetGameInstance()->GetSubsystem<UMenuStateManager>();

	if (!menu) return;

	menu->InputConfirm_Menu();
}

void APlayPlayerController::InputCancel_Menu()
{
	UMenuStateManager* menu = GetGameInstance()->GetSubsystem<UMenuStateManager>();

	if (!menu) return;

	menu->InputCancel_Menu();
}

void APlayPlayerController::InputDirectional_Menu(EUIInputType type)
{
	UMenuStateManager* menu = GetGameInstance()->GetSubsystem<UMenuStateManager>();

	UE_LOG(LogTemp, Warning, TEXT("InputDirectional_Menu called. type=%d, menu=%s"), (int32)type, menu ? TEXT("valid") : TEXT("nullptr"));

	if (!menu) return;

	if (type == EUIInputType::UP)
	{
		menu->InputNavigateUp();
	}
	else if(type == EUIInputType::DOWN)
	{
		menu->InputNavigateDown();
	}
	else if (type == EUIInputType::LEFT)
	{
		menu->InputNavigateLeft();
	}
	else if (type == EUIInputType::RIGHT)
	{
		menu->InputNavigateRight();
	}
}

void APlayPlayerController::InputL1_Menu()
{
	UMenuStateManager* menu = GetGameInstance()->GetSubsystem<UMenuStateManager>();
	if (!menu) return;

	menu->InputLButton();
}

void APlayPlayerController::InputL2_Menu()
{
	UMenuStateManager* menu = GetGameInstance()->GetSubsystem<UMenuStateManager>();
	if (!menu) return;

	menu->InputL2Button();
}

void APlayPlayerController::InputR1_Menu()
{
	UMenuStateManager* menu = GetGameInstance()->GetSubsystem<UMenuStateManager>();
	if (!menu) return;

	menu->InputRButton();
}

void APlayPlayerController::InputR2_Menu()
{
	UMenuStateManager* menu = GetGameInstance()->GetSubsystem<UMenuStateManager>();
	if (!menu) return;

	menu->InputR2Button();
}


void APlayPlayerController::InputConfirm_Shop()
{
	UShopManager* shop = GetWorld()->GetSubsystem<UShopManager>();

	if (!shop)return;

	shop->ConfirmShopUI();
}

void APlayPlayerController::InputCancel_Shop()
{
	UShopManager* shop = GetWorld()->GetSubsystem<UShopManager>();

	if (!shop)return;

	shop->CancelShopUI();
}

void APlayPlayerController::InputInteract_Shop()
{
	UShopManager* shop = GetWorld()->GetSubsystem<UShopManager>();

	if (!shop)return;

	shop->InputInteract();
}

void APlayPlayerController::InputDirectional_Shop(EUIInputType type)
{
	UShopManager* shop = GetWorld()->GetSubsystem<UShopManager>();

	if (!shop)return;

	shop->DirectionalInput(type);
}
