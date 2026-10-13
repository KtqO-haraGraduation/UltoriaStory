// Fill out your copyright notice in the Description page of Project Settings.


#include "EventScriptEngine.h"
#include "SequenceMappingRow.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "../Battle/BattleManager.h"

// Sets default values for this component's properties
UEventScriptEngine::UEventScriptEngine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_CurrentIndex = 0;
	m_IsSepuencePlaying = false;
	//m_SequenceActor = nullptr;
	//m_SequencePlayer = nullptr;
}

void UEventScriptEngine::BeginPlay()
{
	Super::BeginPlay();

	// 全コマンド関数の追加
	m_CommandMap.Add(TEXT("Talk"), &UEventScriptEngine::Command_Talk);
	m_CommandMap.Add(TEXT("Talk_Auto"), &UEventScriptEngine::Command_AutoTalk);
	m_CommandMap.Add(TEXT("Camera"), &UEventScriptEngine::Command_Camera);
	m_CommandMap.Add(TEXT("BGM"), &UEventScriptEngine::Command_BGM);
	m_CommandMap.Add(TEXT("Fade"), &UEventScriptEngine::Command_Fade);
	m_CommandMap.Add(TEXT("Wait"), &UEventScriptEngine::Command_Wait);
	m_CommandMap.Add(TEXT("Anim"), &UEventScriptEngine::Command_Anim);
	m_CommandMap.Add(TEXT("Sequence"), &UEventScriptEngine::Command_Sequence);
	m_CommandMap.Add(TEXT("BattlePause"), &UEventScriptEngine::Command_BattlePause);
	m_CommandMap.Add(TEXT("BattleResume"), &UEventScriptEngine::Command_BattleResume);
	m_CommandMap.Add(TEXT("BattleStart"), &UEventScriptEngine::Command_BattleStart);
}


void UEventScriptEngine::StartScript(UDataTable* ScriptTable)
{
	// スクリプトデータが無ければ終了
	if (!ScriptTable)return;

	// イベントシーン再生中フラグ
	m_IsEventPlaying = true;

	// スクリプトを初期化
	m_ScriptTable = ScriptTable;
	m_CurrentIndex = 0;
	m_Rows.Empty();

	// 全コマンドを取得
	m_ScriptTable->GetAllRows<FEventScriptRow>(TEXT(""), m_Rows);

	// 行にデータが無ければ終了
	if (m_Rows.IsEmpty())return;

	// 最初のコマンドを実行
	ExcCommand(m_Rows[m_CurrentIndex]);
}

void UEventScriptEngine::NextCommand()
{
	// 現在の順番を更新
	m_CurrentIndex++;

	// 全てのコマンドを実行していれば
	if (!m_Rows.IsValidIndex(m_CurrentIndex))
	{	
		// スクリプトを終了
		FinishedScript();
		return;
	}

	// コマンドを実行
	ExcCommand(m_Rows[m_CurrentIndex]);
}

void UEventScriptEngine::ExcCommand(FEventScriptRow* Row)
{
	// もしコマンドがなければ
	if (!Row) return;

	// 字幕用コマンドだったら
	if (Row->Type == TEXT("Subtitle"))
	{
		if (m_IsSepuencePlaying)
		{
			return;
		}
		NextCommand();
		return;
	}

	// コマンドに対応した関数を検索
	CommandFunc* func = m_CommandMap.Find(Row->Type);
	// 関数があれば
	if (func)
	{
		// コマンドに対応した関数を呼び出す
		(this->* * func)(Row);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("不明なコマンド: %s"), *Row->Type);
	}
}

void UEventScriptEngine::FinishedScript()
{
	UE_LOG(LogTemp, Log, TEXT("スクリプト終了"));

	// widgetがあれば消す
	if (m_DialogueWidgetInstance)
	{
		m_DialogueWidgetInstance->RemoveFromParent();
		m_DialogueWidgetInstance = nullptr;
	}

	// BGMを停止
	if (m_AudioComponent)
	{
		m_AudioComponent->Stop();
		m_AudioComponent = nullptr;
	}

	// PlayerControllerからカメラマネージャーを取得
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)return;
	APlayerCameraManager* cameraManager = PC->PlayerCameraManager;
	if (!cameraManager)return;

	// フェードアウト
	cameraManager->StartCameraFade(1.0f, 1.0f, 1.0f, FLinearColor::Black, false, true);

	// 暗転中にカメラをプレイヤーに戻す
	FTimerHandle fadeOutHandle;
	GetWorld()->GetTimerManager().SetTimer(fadeOutHandle, [this, PC]()
	{
		// カメラをプレイヤーに戻す
		if (PC->GetPawn())
		{
			PC->SetViewTargetWithBlend(PC->GetPawn(), 0.0f);
		}

		// フェードイン
		PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 0.5f, FLinearColor::Black, false, false);

	}, 0.5f, false);

	// イベントシーン再生中フラグ
	m_IsEventPlaying = false;
}

void UEventScriptEngine::FinishedSequence()
{
	m_IsSepuencePlaying = false;

	// 字幕キューの分だけインデックスを進める
	if (m_SubtitleQueue.Num() > 0)
	{
		m_CurrentIndex += m_SubtitleQueue.Num();
		m_SubtitleQueue.Empty();
		m_SubtitleQueueIndex = 0;

		if (m_SubtitleWidgetInstance)
		{
			m_SubtitleWidgetInstance->HideSubtitle();
		}
	}

	NextCommand();

}

void UEventScriptEngine::Command_Talk(FEventScriptRow* Row)
{
	UE_LOG(LogTemp, Log, TEXT("[Talk] %s: %s"), *Row->Param1, *Row->Param2);

	// Widgetが無ければ生成
	if (!m_DialogueWidgetInstance && m_DialogueWidgetClass)
	{
		m_DialogueWidgetInstance = CreateWidget<UDIalougeWidget>(
			GetWorld()->GetFirstPlayerController(),
			m_DialogueWidgetClass
		);
		m_DialogueWidgetInstance->AddToViewport();
	}

	UE_LOG(LogTemp, Log, TEXT("Widget Instance: %s"), m_DialogueWidgetInstance ? TEXT("There is") : TEXT("Empty"));

	// 話者名とセリフを設定
	m_DialogueWidgetInstance->SetSpeakerText(Row->Param1);
	m_DialogueWidgetInstance->SetDialogueText(Row->Param2);
}

void UEventScriptEngine::Command_AutoTalk(FEventScriptRow* Row)
{
	// 会話処理を実行
	Command_Talk(Row);

	// 次のコマンドへ
	NextCommand();
}

bool UEventScriptEngine::IsTyping()
{
	if (!m_DialogueWidgetInstance)return false;
	return m_DialogueWidgetInstance->GetTyping();
}

void UEventScriptEngine::SkipTyping()
{
	if (!m_DialogueWidgetInstance)return;
	m_DialogueWidgetInstance->SkipTyping();
}

void UEventScriptEngine::AdvanceSubtitle()
{
	if (!m_SubtitleQueue.IsValidIndex(m_SubtitleQueueIndex))
	{
		return;
	}

	if (!m_SubtitleWidgetInstance && m_DialogueWidgetClass)
	{
		m_SubtitleWidgetInstance = CreateWidget<USubtitleWidget>(
			GetWorld()->GetFirstPlayerController(),
			m_SubtitleWidgetClass
		);

		m_SubtitleWidgetInstance->AddToViewport();
	}

	FEventScriptRow& subtitleRow = m_SubtitleQueue[m_SubtitleQueueIndex];

	if (m_SubtitleWidgetInstance)
	{
		m_SubtitleWidgetInstance->SetSubtitle(subtitleRow.Param1, subtitleRow.Param2);
		m_SubtitleWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}

	m_SubtitleQueueIndex++;
}

void UEventScriptEngine::Command_Camera(FEventScriptRow* Row)
{
	// シーケンサー再生中なら終了
	if (m_IsSepuencePlaying)return;

	// Param1 = カメラ名
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*Row->Param1), foundActors);

	// アクターが見つからなければ終了
	if (foundActors.IsEmpty())return;

	// ブレンド時間を取得してカメラを設定
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	float blendTime = Row->Param2.IsEmpty() ? 0.0f : FCString::Atof(*Row->Param2);
	PC->SetViewTargetWithBlend(foundActors[0], blendTime);

	NextCommand();
}

void UEventScriptEngine::Command_BGM(FEventScriptRow* Row)
{
	UE_LOG(LogTemp, Log, TEXT("[BGM] %s"), *Row->Param1);

	USoundBase* sound = LoadObject<USoundBase>(nullptr, *Row->Param1);
	if (!sound)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BGM] サウンドが見つからない: %s"), *Row->Param1);
		NextCommand();
		return;
	}

	if (m_AudioComponent)
	{
		m_AudioComponent->Stop();
	}

	m_AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), sound);

	NextCommand();
}

void UEventScriptEngine::Command_Fade(FEventScriptRow* Row)
{
	float fadeTime = Row->Param2.IsEmpty() ? 1.0f : FCString::Atof(*Row->Param2);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)return;

	APlayerCameraManager* cameraManager = PC->PlayerCameraManager;
	if (!cameraManager)return;

	if (Row->Param1 == TEXT("Out"))
	{
		// フェードアウト
		cameraManager->StartCameraFade(0.0f, 1.0f, fadeTime, FLinearColor::Black, false, true);
	}
	else if(Row->Param1 == TEXT("In"))
	{
		// フェードイン
		cameraManager->StartCameraFade(1.0f, 0.0f, fadeTime, FLinearColor::Black, false, true);
	}

	// フェード時間後に次のコマンドへ
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this,
		&UEventScriptEngine::NextCommand, fadeTime, false);

	UE_LOG(LogTemp, Log, TEXT("[Fade] %s %s"), *Row->Param1, *Row->Param2);
}

void UEventScriptEngine::Command_Wait(FEventScriptRow* Row)
{
	float waitTime = FCString::Atof(*Row->Param1);
	UE_LOG(LogTemp, Log, TEXT("[Wait] %f秒"), waitTime);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UEventScriptEngine::NextCommand, waitTime, false);
}

void UEventScriptEngine::Command_Anim(FEventScriptRow* Row)
{
	UE_LOG(LogTemp, Log, TEXT("[Anim] %s: %s"), *Row->Param1, *Row->Param2);
}

void UEventScriptEngine::Command_Sequence(FEventScriptRow* Row)
{
	UE_LOG(LogTemp, Log, TEXT("[Sequence] %s"), *Row->Param1);

	if (!m_SequenceMappingTable)return;

	// マッピングテーブルから検索
	TArray<FSequenceMappingRow*> mappingRows;
	m_SequenceMappingTable->GetAllRows<FSequenceMappingRow>(TEXT(""), mappingRows);

	FString sequencePath = TEXT("");
	for (FSequenceMappingRow* mapping : mappingRows)
	{
		if (mapping->SequenceName == Row->Param1)
		{
			sequencePath = mapping->SequencePath;
			break;
		}
	}

	if (sequencePath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Sequence] Sequence Not Found: %s"), *Row->Param1);
		NextCommand();
		return;
	}

	// シーケンサーを再生
	ULevelSequence* sequence = LoadObject<ULevelSequence>(nullptr, *sequencePath);
	if (!sequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Sequence] Failed to Load Sequence: %s"), *sequencePath);
		NextCommand();
		return;
	}

	FMovieSceneSequencePlaybackSettings settings;
	settings.bAutoPlay = false;

	m_SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), sequence, settings, m_SequenceActor
	);

	if (m_SequencePlayer)
	{
		m_IsSepuencePlaying = true;
		m_SequencePlayer->OnFinished.AddDynamic(this, &UEventScriptEngine::FinishedSequence);
		m_SequencePlayer->Play();
	}

	// 後続のSubtitle行をキューに読み込む
	m_SubtitleQueue.Empty();
	m_SubtitleQueueIndex = 0;

	// 現在の実行行の次から
	int32 lookAheadIndex = m_CurrentIndex + 1;
	while (lookAheadIndex < m_Rows.Num())
	{
		FEventScriptRow* nextRow = m_Rows[lookAheadIndex];
		if (nextRow->Type != TEXT("Subtitle"))
		{
			break;
		}
		m_SubtitleQueue.Add(*nextRow);
		lookAheadIndex++;
	}

	// 字幕キューが1件以上あれば、最初の1件を即表示
	if (m_SubtitleQueue.Num() > 0)
	{
		AdvanceSubtitle();
	}
}

void UEventScriptEngine::Command_BattlePause(FEventScriptRow* Row)
{
	UE_LOG(LogTemp, Log, TEXT("[BattlePause]"));
}

void UEventScriptEngine::Command_BattleResume(FEventScriptRow* Row)
{
	UE_LOG(LogTemp, Log, TEXT("[BattleResume]"));
}

void UEventScriptEngine::Command_BattleStart(FEventScriptRow* Row)
{
	UE_LOG(LogTemp, Log, TEXT("[BattleStart]"));

	/*UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	ACharacterBase* player = nullptr;

	ACharacterBase* enemy = nullptr;

	battleMg->BattaleStart(player, enemy, false);*/
}

