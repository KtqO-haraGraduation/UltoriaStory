// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandCursorWidget.h"

void UCommandCursorWidget::PlayIdle()
{
	if (!IdleAnim) return;
	PlayAnimation(IdleAnim, 0.0f, 0);
}

void UCommandCursorWidget::StopIdle()
{
	if (!IdleAnim) return;
	StopAnimation(IdleAnim);
}

void UCommandCursorWidget::PlayMove()
{
	if (!MoveAnim) return;
	PlayAnimation(MoveAnim);
}

void UCommandCursorWidget::ReverseMove()
{
	if (!MoveAnim) return;
	PlayAnimationReverse(MoveAnim);
}

void UCommandCursorWidget::SetCursorPos(const FVector2D& pos)
{
	SetRenderTranslation(pos);
}

void UCommandCursorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// アニメーションが終了した時のイベントを追加
	FWidgetAnimationDynamicEvent Event;
	Event.BindDynamic(this, &UCommandCursorWidget::OnMoveAnimFinished);

	BindToAnimationFinished(MoveAnim, Event);
}

void UCommandCursorWidget::OnMoveAnimFinished()
{
	switch (m_State)
	{
	case CommandCursorState::MOVE_IN:
	{
		m_State = CommandCursorState::IDLE;
		PlayIdle();
		break;
	}
	case CommandCursorState::MOVE_OUT:
	{
		SetCursorPos(m_NextPos);
		m_State = CommandCursorState::MOVE_IN;
		PlayMove();
		break;
	}
	}
}

void UCommandCursorWidget::MoveTo(const FVector2D& pos)
{
	if (m_IsFirst)
	{
		SetCursorPos(pos);
		PlayMove();

		m_IsFirst = false;
		return;
	}

	m_NextPos = pos;

	StopIdle();
	m_State = CommandCursorState::MOVE_OUT;
	ReverseMove();
}
