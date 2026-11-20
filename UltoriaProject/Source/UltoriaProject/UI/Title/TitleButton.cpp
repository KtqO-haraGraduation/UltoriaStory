// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTitleButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_DefaultStyle = m_Button->GetStyle();
}

void UTitleButton::NativePreConstruct()
{

	Super::NativePreConstruct();

	if (m_LabelTexture)
	{
		m_LabelImage->SetBrushFromTexture(m_LabelTexture, true);
	}
	m_LabelImage->SetColorAndOpacity(m_NormalLabelColor);
}

void UTitleButton::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);

	FButtonStyle style = m_DefaultStyle;
	style.Normal = style.Hovered;
	m_Button->SetStyle(style);

	m_LabelImage->SetColorAndOpacity(m_FocusedLabelColor);
}

void UTitleButton::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	m_Button->SetStyle(m_DefaultStyle);
	m_LabelImage->SetColorAndOpacity(m_NormalLabelColor);
}

void UTitleButton::HandleHovered()
{
	m_Button->SetKeyboardFocus();
}

void UTitleButton::HandleClicked()
{
	switch (m_Action)
	{
	case ETitleButtonAction::NewGame:
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, m_StartLevel);
		break;
	default:
		// Continue / Settings ‚ÍŒã‚ÅŽÀ‘•
		break;
	}
}
