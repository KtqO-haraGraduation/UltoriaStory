// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuStateManager.h"
#include "MenuBase.h"
#include "Kismet/GameplayStatics.h"
#include "../../System/Controller/PlayPlayerController.h"

void UMenuStateManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	m_CurrentMenuState = EMenuState::MENU_NONE;

	// 各クラスのデータを読み込む
	UMenuClassDataAsset* menuClassData = LoadObject<UMenuClassDataAsset>(nullptr,
		TEXT("/Game/MembarFiles/Yoshihiro/Data_Yoshi/Menu/DA_MenuClass"));

	// 各クラスとステートを紐づける
	if (menuClassData)
	{
		m_MenuClassMap.Add(EMenuState::MENU_MAIN, menuClassData->MainMenuClass);
		m_MenuClassMap.Add(EMenuState::MENU_STATES, menuClassData->StatesMenuClass);
		m_MenuClassMap.Add(EMenuState::MENU_ITEMS, menuClassData->ItemsMenuClass);
		m_MenuClassMap.Add(EMenuState::MENU_QUEST, menuClassData->QuestMenuClass);
		m_MenuClassMap.Add(EMenuState::MENU_OPTIONS, menuClassData->OptionMenuClass);
		m_MenuClassMap.Add(EMenuState::MENU_SAVE, menuClassData->SaveMenuClass);
		m_MenuClassMap.Add(EMenuState::MENU_END, menuClassData->EndMenuClass);
	}
}

void UMenuStateManager::Deinitialize()
{
	Super::Deinitialize();
}

void UMenuStateManager::OpenMenu(EMenuState State)
{
	if (m_CurrentMenuState == State)return;

	// 今のメニューを閉じる
	if (m_CurrentMenuState != EMenuState::MENU_NONE)
	{
		UMenuBase** currentMenu = m_MenuMap.Find(m_CurrentMenuState);
		if (currentMenu && *currentMenu)
		{
			(*currentMenu)->Hide();
		}
	}

	// メニューがまだ生成されていなければ生成する
	if (!m_MenuMap.Contains(State))
	{
		TSubclassOf<UMenuBase> menuClass = GetMenuClass(State);
		if (menuClass)
		{
			APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
			UMenuBase* newMenu = CreateWidget<UMenuBase>(PC, menuClass);
			if (newMenu)
			{
				newMenu->AddToViewport();
				newMenu->Hide();
				m_MenuMap.Add(State, newMenu);
			}
		}
	}

	// 新しいメニューを開く
	m_CurrentMenuState = State;
	UMenuBase** newMenu = m_MenuMap.Find(State);
	if (newMenu && *newMenu)
	{
		(*newMenu)->Show();
	}

	// プレイヤーの入力を止める
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
	}

	// 入力モードを変更
	ChangeInputMode(PlayInputMode::MENU);
}

void UMenuStateManager::CloseMenu()
{
	if (m_CurrentMenuState == EMenuState::MENU_NONE)return;

	// 今のメニューを閉じる
	UMenuBase** currentMenu = m_MenuMap.Find(m_CurrentMenuState);
	if (currentMenu && *currentMenu)
	{
		(*currentMenu)->Hide();
	}

	m_CurrentMenuState = EMenuState::MENU_NONE;

	// プレイヤーの入力を戻す
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}

void UMenuStateManager::ChangeInputMode(PlayInputMode mode)
{
	APlayPlayerController* pc =
		Cast<APlayPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc)pc->ChangeInputMode(mode);
}

void UMenuStateManager::InputNavigateUp()
{
	UE_LOG(LogTemp, Log, TEXT("NavigateUp!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::NavigateUp);
}

void UMenuStateManager::InputNavigateDown()
{
	UE_LOG(LogTemp, Log, TEXT("NavigateDown!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::NavigateDown);
}

void UMenuStateManager::InputNavigateLeft()
{
	UE_LOG(LogTemp, Log, TEXT("NavigateLeft!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::NavigateLeft);
}

void UMenuStateManager::InputNavigateRight()
{
	UE_LOG(LogTemp, Log, TEXT("NavigateRight!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::NavigateRight);
}

void UMenuStateManager::InputLButton()
{
	UE_LOG(LogTemp, Log, TEXT("OnLButton!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::OnLButton);
}

void UMenuStateManager::InputRButton()
{
	UE_LOG(LogTemp, Log, TEXT("OnRButton!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::OnRButton);
}

void UMenuStateManager::InputL2Button()
{
	UE_LOG(LogTemp, Log, TEXT("OnL2Button!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::OnL2Button);
}

void UMenuStateManager::InputR2Button()
{
	UE_LOG(LogTemp, Log, TEXT("OnR2Button!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::OnR2Button);
}

void UMenuStateManager::InputConfirm_Menu()
{
	UE_LOG(LogTemp, Log, TEXT("Confirm!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::Confirm);
}

void UMenuStateManager::InputCancel_Menu()
{
	UE_LOG(LogTemp, Log, TEXT("Cancel!!!!!!!!!!!"));

	DispatchMenuInput(&UMenuBase::Cancel);
}

void UMenuStateManager::InputBuck()
{
	CloseMenu();
}

TSubclassOf<UMenuBase> UMenuStateManager::GetMenuClass(EMenuState State)
{
	TSubclassOf<UMenuBase>* menuClass = m_MenuClassMap.Find(State);
	return menuClass ? *menuClass : nullptr;
}

void UMenuStateManager::DispatchMenuInput(void(UMenuBase::*func)())
{
	if (m_CurrentMenuState == EMenuState::MENU_NONE)return;
	UMenuBase** currentMenu = m_MenuMap.Find(m_CurrentMenuState);
	if (currentMenu && *currentMenu)
	{
		((*currentMenu)->*func)();
	}
}