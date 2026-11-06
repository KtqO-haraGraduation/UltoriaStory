// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EventScriptRow.h"
#include "../UI/Dialouge/DIalougeWidget.h"
#include "Components/AudioComponent.h"
#include "../UI/Subtitle/SubtitleWidget.h"
#include "EventScriptEngine.generated.h"

class ULevelSequencePlayer;
class ALevelSequenceActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API UEventScriptEngine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEventScriptEngine();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// スクリプト開始関数
	UFUNCTION(BlueprintCallable)
	void StartScript(UDataTable* ScriptTable);

	// 次コマンド実行関数
	UFUNCTION(BlueprintCallable)
	void NextCommand();

	// イベントシーン再生中フラグ取得関数
	bool GetEventPlay() { return m_IsEventPlaying; }
	// 文字送り中関数
	bool IsTyping();
	// 文字送りスキップ関数
	void SkipTyping();

	// シーケンサー再生中字幕関数
	UFUNCTION(BlueprintCallable)
	void AdvanceSubtitle();

private:
	// コマンド実行関数
	void ExcCommand(FEventScriptRow* Row);

	// 各コマンド処理
	void Command_Talk(FEventScriptRow* Row);
	void Command_AutoTalk(FEventScriptRow* Row);
	void Command_Camera(FEventScriptRow* Row);
	void Command_BGM(FEventScriptRow* Row);
	void Command_Fade(FEventScriptRow* Row);
	void Command_Wait(FEventScriptRow* Row);
	void Command_Anim(FEventScriptRow* Row);
	void Command_Sequence(FEventScriptRow* Row);
	void Command_BattlePause(FEventScriptRow* Row);
	void Command_BattleResume(FEventScriptRow* Row);
	void Command_BattleStart(FEventScriptRow* Row);

	// スクリプト終了関数
	void FinishedScript();
	UFUNCTION()
	// シーケンス終了関数
	void FinishedSequence();

	// バトル終了時関数
	void OnBattleFinished();

	// イベント時プレイヤー非表示関数
    void ShowPlayerPawn();

	// 全コマンドの呼び出し用関数ポインタ
	typedef void (UEventScriptEngine::* CommandFunc)(FEventScriptRow*);

private:
// スクリプト関連
	// スクリプトのデータテーブル
	UPROPERTY()
	UDataTable* m_ScriptTable;	
	// 現在のコマンドIndex
	UPROPERTY()
	int32 m_CurrentIndex;
	// 全コマンドのリスト
	TArray<FEventScriptRow*> m_Rows;
	// コマンド関数のマップ
	TMap<FString, CommandFunc> m_CommandMap;

// シーケンサ関連
	// シーケンサのデータテーブル
	UPROPERTY(EditAnywhere, Category = "Event")
	UDataTable* m_SequenceMappingTable;
	// シーケンサのプレイヤーとアクター
	UPROPERTY()
	ULevelSequencePlayer* m_SequencePlayer;
	UPROPERTY()
	ALevelSequenceActor* m_SequenceActor;

	// 再生中の字幕
	TArray<FEventScriptRow> m_SubtitleQueue;
	// 再生中字幕用Index
	int32 m_SubtitleQueueIndex;
	// 字幕クラス
	UPROPERTY(EditAnywhere, Category = "Event")
	TSubclassOf<USubtitleWidget> m_SubtitleWidgetClass;
	// 字幕表示用ウィジェット
	UPROPERTY()
	USubtitleWidget* m_SubtitleWidgetInstance;

// テキスト・オーディオ関連
	// メッセージウィンドウクラス
	UPROPERTY(EditAnywhere, Category = "Event")
	TSubclassOf<UDIalougeWidget> m_DialogueWidgetClass;
	// メッセージウィンドウインスタンス
	UPROPERTY()
	UDIalougeWidget* m_DialogueWidgetInstance;

	// オーディオコンポーネント
	UPROPERTY()
	UAudioComponent* m_AudioComponent;

// バトル関連
	UPROPERTY(EditAnywhere)
	FName m_TargetEnemyID;


// フラグ
	UPROPERTY(EditAnywhere, Category = "Event")
	// 一度発生フラグ
	bool m_PlayOnce;
	// イベント・カットシーン再生中フラグ
	bool m_IsEventPlaying;
	// Sequence再生中フラグ
	bool m_IsSepuencePlaying;
	// 会話自動再生フラグ
	bool m_CanAutoTalk;
	// バトル終了フラグ
	bool m_BattleEndsScript;
	// バトル終了待機フラグ
	bool m_WaitingBattleEnd;

// プレイヤー
	TWeakObjectPtr<APawn> m_HiddenPawn;
};
