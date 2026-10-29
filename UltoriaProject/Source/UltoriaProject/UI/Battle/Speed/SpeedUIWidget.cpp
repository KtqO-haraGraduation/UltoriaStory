// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedUIWidget.h"
#include "../../../Battle/BattleManager.h"
#include "../../../Battle/Combat/Turn/BattleTurnManager.h"
#include "../../../Battle/Combat/Data/BattleDataManager.h"
#include "../Data/BattleUIDataAsset.h"
#include "../../../Character/CharacterBase.h"
#include "SpeedWidget.h"
#include "Components/CanvasPanelSlot.h"

const float StartX = 500.0f;
const float StartY = 20.0f;
const float Width = 100.0f;
const float Spacing = 20.0f;


void USpeedUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// スピード順のUIを生成
	CreateSpeedWidgets();
}

void USpeedUIWidget::CreateSpeedWidget(ACharacterBase* character)
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (!battleMg || !battleDataMg) return;

	const TArray<ACharacterBase*>& characters = battleMg->GetTurnManager()->GetCharacters();
	TSubclassOf<USpeedWidget> widgetClass = battleDataMg->GetBattleUIDataAsset()->SpeedWidgetClass;

	USpeedWidget* widget = CreateWidget<USpeedWidget>(GetWorld(), widgetClass);
	if (!widget) return;

	widget->SetCharacter(character);

	m_SpeedWidgets.Add(widget);
}

void USpeedUIWidget::CreateSpeedWidgets()
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (!battleMg || !battleDataMg) return;

	const TArray<ACharacterBase*>& characters = battleMg->GetTurnManager()->GetCharacters();
	TSubclassOf<USpeedWidget> widgetClass = battleDataMg->GetBattleUIDataAsset()->SpeedWidgetClass;

	int i = 0;
	for (auto character : characters)
	{
		if (!character) continue;

		USpeedWidget* widget = CreateWidget<USpeedWidget>(GetWorld(), widgetClass);
		if (!widget) continue;

		// ここらへんで自分の画像を入れ込む
		widget->SetCharacter(character);

		UCanvasPanelSlot* slot = SpeedPanel->AddChildToCanvas(widget);
		if (!slot) continue;

		const float X = StartX + i * (Width + Spacing);

		slot->SetPosition(FVector2D(X, StartY));
		//slot->SetSize(FVector2D(Width, 100.0f));

		m_SpeedWidgets.Add(widget);

		i++;
	}
}

void USpeedUIWidget::SetSpeedWidgetData()
{
	if (m_SpeedWidgets.IsEmpty()) return;

	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	const TArray<ACharacterBase*>& characters = battleMg->GetTurnManager()->GetCharacters();

	for (int i = 0; i < characters.Num(); i++)
	{
		ACharacterBase* character = characters[i];

		// ウィジェットが足りない場合だけ生成
		if (m_SpeedWidgets.Num() <= i)
		{
			CreateSpeedWidget(character);
		}

		USpeedWidget* widget = m_SpeedWidgets[i];
		
		if (!widget || !character) continue;

		widget->SetCharacter(character);
	}
}

void USpeedUIWidget::RemoveSpeedWidget(USpeedWidget* widget)
{
	m_SpeedWidgets.Remove(widget);
}

void USpeedUIWidget::ResetSpeedWidget()
{
	m_SpeedWidgets.Empty();
}
