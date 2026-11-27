#include "RaidBattleManager.h"
#include "RaidBattleTurnManager.h"
#include "RaidBattleCommandProxy.h"
#include "../../Character/CharacterBase.h"
#include "../../Character/Enemy/EnemyBase.h"
#include "../../Battle/Combat/Components/CombatComponent.h"
#include "../../Battle/Combat/Components/Status/CombatStatusComponent.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "../../UI/Battle/Raid/RaidBattleUIWidget.h"
#include "../../UI/Battle/DamageWidget.h"

namespace
{
    // 画面にデバッグ表示を出す（同じKeyは上書きされるので、連続で呼んでも増えない）
    void RaidScreenMessage(int32 Key, const FColor& Color, const FString& Message)
    {
#if !UE_BUILD_SHIPPING
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(Key, 10.0f, Color, Message);
        }
#endif
    }
}

ARaidBattleManager::ARaidBattleManager()
{
    bReplicates = true;
    SetReplicateMovement(false);
    PrimaryActorTick.bCanEverTick = false;
}

void ARaidBattleManager::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        GetWorldTimerManager().SetTimer(
            m_InitializeTimer,
            this,
            &ARaidBattleManager::TryInitializeRaid,
            0.25f,
            true
        );
    }
}

void ARaidBattleManager::TryInitializeRaid()
{
    if (InitializeRaid())
    {
        GetWorldTimerManager().ClearTimer(m_InitializeTimer);
        return;
    }

    ++m_InitializeAttempts;
    if (m_InitializeAttempts >= 120)
    {
        GetWorldTimerManager().ClearTimer(m_InitializeTimer);
        UE_LOG(LogTemp, Error, TEXT("Raid initialization failed. Assign a boss and make sure every player has an ACharacterBase pawn."));
        RaidScreenMessage(1003, FColor::Red, TEXT("Raid initialization FAILED"));
    }
}

bool ARaidBattleManager::InitializeRaid()
{
    if (!HasAuthority() || m_bRaidStarted) return m_bRaidStarted;

    // ボスはレイドマップに配置し、ManagerのDetailsで指定する。
    if (!IsValid(m_Boss))
    {
        RaidScreenMessage(1001, FColor::Red, TEXT("Raid: Boss is NONE (set m_Boss in the Manager Details)"));
        return false;
    }

    TArray<ACharacterBase*> Participants;
    bool bAllHavePawn = true;
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!IsValid(PC)) continue;

        ACharacterBase* Character = Cast<ACharacterBase>(PC->GetPawn());
        if (!IsValid(Character) || Character == m_Boss)
        {
            bAllHavePawn = false;   // まだポーンが来ていない人がいる
            continue;
        }

        Character->SetReplicates(true);
        Character->SetReplicateMovement(true);
        Participants.AddUnique(Character);
    }

    if (Participants.IsEmpty())
    {
        RaidScreenMessage(1002, FColor::Red, TEXT("Raid: No Participants (no player has an ACharacterBase pawn)"));
        return false;
    }

    // 全員そろうまで待つ
    const float Now = GetWorld()->GetTimeSeconds();
    if (!bAllHavePawn)
    {
        m_LastParticipantCount = -1;
        RaidScreenMessage(1006, FColor::Yellow, TEXT("Raid: waiting for players' pawns..."));
        return false;
    }
    if (m_ExpectedPlayers > 0)
    {
        if (Participants.Num() < m_ExpectedPlayers)
        {
            RaidScreenMessage(1006, FColor::Yellow,
                FString::Printf(TEXT("Raid: waiting players %d/%d"), Participants.Num(), m_ExpectedPlayers));
            return false;
        }
    }
    else
    {
        // 人数が変わらない状態が m_StartDelay 秒続いたら開始
        if (Participants.Num() != m_LastParticipantCount)
        {
            m_LastParticipantCount = Participants.Num();
            m_StableSince = Now;
            return false;
        }
        if (Now - m_StableSince < m_StartDelay) return false;
    }

    m_Boss->SetReplicates(true);
    m_Boss->SetReplicateMovement(true);

    TArray<ACharacterBase*> AllCharacters = Participants;
    AllCharacters.Add(m_Boss);

    for (ACharacterBase* Character : AllCharacters)
    {
        UCombatComponent* Combat = Character->GetComponent<UCombatComponent>();
        if (!Combat)
        {
            UE_LOG(LogTemp, Error, TEXT("Raid character %s has no CombatComponent."), *Character->GetName());
            return false;
        }

        // CharacterBase::InitComponent()で通常は無効化されるため、レイド開始時に有効化する。
        Combat->SetActiveComponent(true);
        if (UCombatStatusComponent* Status = Character->GetComponent<UCombatStatusComponent>())
        {
            Status->SetActiveComponent(true);
        }
    }

    m_TurnManager = NewObject<URaidBattleTurnManager>(this);
    m_TurnManager->Initialize(AllCharacters);

    m_TurnOrder.Reset();
    for (ACharacterBase* Character : m_TurnManager->GetTurnOrder())
    {
        m_TurnOrder.Add(Character);
    }

    CreateCommandProxies();

    m_bRaidStarted = true;
    RaidScreenMessage(1004, FColor::Green, FString::Printf(TEXT("Raid Started! Players: %d"), Participants.Num()));
    m_bRaidEnded = false;
    m_RaidResult = ERaidBattleResult::NONE;
    AdvanceTurn();
    RefreshReplicatedStates();
    OnRaidBattleChanged.Broadcast();
    CreateRaidBattleUI();
    if (m_Boss) m_Boss->SetStatusUIVisibility(true);
    ForceNetUpdate();

    UE_LOG(LogTemp, Warning, TEXT("Raid started. Participants: %d, Boss: %s"), Participants.Num(), *m_Boss->GetName());
    return true;
}

void ARaidBattleManager::CreateRaidBattleUI()
{
    if (m_RaidBattleWidget) return;

    APlayerController* LocalPC = nullptr;
    if (GetWorld())
    {
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* Candidate = It->Get();
            if (IsValid(Candidate) && Candidate->IsLocalController())
            {
                LocalPC = Candidate;
                break;
            }
        }
    }
    if (!IsValid(LocalPC)) return;

    if (!m_RaidUIClass)
    {
        RaidScreenMessage(1005, FColor::Red, TEXT("Raid: Raid UI Class is NONE (set it in the Manager Details)"));
        return;
    }

    m_RaidBattleWidget = CreateWidget<URaidBattleUIWidget>(LocalPC, m_RaidUIClass);
    if (!m_RaidBattleWidget) return;

    m_RaidBattleWidget->SetRaidManager(this);
    m_RaidBattleWidget->AddToViewport();
}

void ARaidBattleManager::CreateCommandProxies()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!IsValid(PC) || !Cast<ACharacterBase>(PC->GetPawn())) continue;

        FActorSpawnParameters Params;
        Params.Owner = PC;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ARaidBattleCommandProxy* Proxy = GetWorld()->SpawnActor<ARaidBattleCommandProxy>(
            ARaidBattleCommandProxy::StaticClass(),
            FTransform::Identity,
            Params
        );

        if (Proxy)
        {
            Proxy->Initialize(this, PC);
        }
    }
}

void ARaidBattleManager::HandlePlayerCommand(
    APlayerController* RequestingController,
    ERaidCommandType Command,
    ACharacterBase* Target)
{
    if (!HasAuthority() || !m_bRaidStarted || m_bRaidEnded || !m_bWaitingForCommand) return;
    if (!IsValid(RequestingController) || !IsValid(m_CurrentCharacter)) return;

    // RPC送信者が現在行動中の主人公本人であることをサーバーで確認する。
    if (RequestingController->GetPawn() != m_CurrentCharacter) return;

    UCombatComponent* ActingCombat = m_CurrentCharacter->GetComponent<UCombatComponent>();
    if (!ActingCombat) return;

    switch (Command)
    {
    case ERaidCommandType::ATTACK:
    {
        // 攻撃対象は全員共有のボス1体だけ。
        if (Target && Target != m_Boss) return;

        UCombatComponent* BossCombat = m_Boss ? m_Boss->GetComponent<UCombatComponent>() : nullptr;
        if (!BossCombat) return;

        ActingCombat->SetCurrentState(CombatState::ATTACK);
        BossCombat->TakeDamage(m_CurrentCharacter->GetBaseStatus().m_ATK);
        break;
    }
    case ERaidCommandType::GUARD:
    {
        ActingCombat->SetCurrentState(CombatState::GUARD);
        if (UCombatStatusComponent* Status = m_CurrentCharacter->GetComponent<UCombatStatusComponent>())
        {
            Status->SetGuarding(true);
            Status->AddStatus(StatusType::GUARD);
        }
        break;
    }
    case ERaidCommandType::END_TURN:
        break;
    case ERaidCommandType::SKILL:
    case ERaidCommandType::ITEM:
        // 現在の通常戦闘側でもSkill()/Item()本体は未実装のため、ここではターンを消費しない。
        return;
    default:
        return;
    }

    ActingCombat->ResetState();
    if (UCombatStatusComponent* Status = m_CurrentCharacter->GetComponent<UCombatStatusComponent>())
    {
        Status->UpdateTurn();
    }
    RefreshReplicatedStates();
    CheckRaidEnd();
    if (m_bRaidEnded) return;

    m_bWaitingForCommand = false;
    GetWorldTimerManager().SetTimer(
        m_ActionTimer,
        this,
        &ARaidBattleManager::AdvanceTurn,
        0.2f,
        false
    );
    ForceNetUpdate();
}

void ARaidBattleManager::AdvanceTurn()
{
    if (!HasAuthority() || !m_bRaidStarted || m_bRaidEnded || !m_TurnManager) return;

    m_CurrentCharacter = m_TurnManager->AdvanceTurn();
    if (!IsValid(m_CurrentCharacter))
    {
        CheckRaidEnd();
        return;
    }

    RefreshReplicatedStates();

    if (m_CurrentCharacter == m_Boss)
    {
        m_bWaitingForCommand = false;
        GetWorldTimerManager().SetTimer(
            m_ActionTimer,
            this,
            &ARaidBattleManager::ExecuteBossTurn,
            0.5f,
            false
        );
    }
    else
    {
        m_bWaitingForCommand = true;
    }

    OnRaidBattleChanged.Broadcast();
    ForceNetUpdate();
}

void ARaidBattleManager::ExecuteBossTurn()
{
    if (!HasAuthority() || m_bRaidEnded || m_CurrentCharacter != m_Boss) return;

    TArray<ACharacterBase*> AlivePlayers;
    for (ACharacterBase* Character : m_TurnOrder)
    {
        if (Character != m_Boss && IsPlayerCharacterAlive(Character))
        {
            AlivePlayers.Add(Character);
        }
    }

    if (AlivePlayers.IsEmpty())
    {
        CheckRaidEnd();
        return;
    }

    // 仮のボス行動：生存している主人公1人を選んで通常攻撃する。
    ACharacterBase* Target = AlivePlayers[FMath::RandRange(0, AlivePlayers.Num() - 1)];
    UCombatComponent* BossCombat = m_Boss->GetComponent<UCombatComponent>();
    UCombatComponent* TargetCombat = Target->GetComponent<UCombatComponent>();
    if (BossCombat && TargetCombat)
    {
        BossCombat->SetCurrentState(CombatState::ATTACK);
        TargetCombat->TakeDamage(m_Boss->GetBaseStatus().m_ATK);
        BossCombat->ResetState();
    }

    if (UCombatStatusComponent* Status = m_Boss->GetComponent<UCombatStatusComponent>())
    {
        Status->UpdateTurn();
    }
    RefreshReplicatedStates();
    CheckRaidEnd();
    if (m_bRaidEnded) return;

    GetWorldTimerManager().SetTimer(
        m_ActionTimer,
        this,
        &ARaidBattleManager::AdvanceTurn,
        0.2f,
        false
    );
}

void ARaidBattleManager::CheckRaidEnd()
{
    if (!HasAuthority() || m_bRaidEnded) return;

    if (m_Boss && m_Boss->GetCurrentHP() <= 0)
    {
        FinishRaid(ERaidBattleResult::VICTORY);
        return;
    }

    bool bAnyPlayerAlive = false;
    for (ACharacterBase* Character : m_TurnOrder)
    {
        if (Character != m_Boss && IsPlayerCharacterAlive(Character))
        {
            bAnyPlayerAlive = true;
            break;
        }
    }

    if (!bAnyPlayerAlive)
    {
        FinishRaid(ERaidBattleResult::DEFEAT);
    }
}

bool ARaidBattleManager::IsPlayerCharacterAlive(ACharacterBase* Character) const
{
    if (!IsValid(Character) || Character == m_Boss || Character->GetCurrentHP() <= 0) return false;
    UCombatComponent* Combat = Character->GetComponent<UCombatComponent>();
    return !Combat || Combat->GetCurrentState() != CombatState::DEAD;
}

void ARaidBattleManager::FinishRaid(ERaidBattleResult Result)
{
    if (!HasAuthority() || m_bRaidEnded) return;

    m_bRaidEnded = true;
    m_bWaitingForCommand = false;
    m_RaidResult = Result;
    GetWorldTimerManager().ClearTimer(m_ActionTimer);
    RefreshReplicatedStates();
    OnRaidBattleChanged.Broadcast();
    ForceNetUpdate();
}

void ARaidBattleManager::RefreshReplicatedStates()
{
    if (!HasAuthority()) return;

    m_RaidStates.Reset();
    for (ACharacterBase* Character : m_TurnOrder)
    {
        if (!IsValid(Character)) continue;

        FRaidCharacterState State;
        State.Character = Character;
        State.CharacterName = Character->GetCharacterName();
        State.CurrentHP = Character->GetCurrentHP();
        State.MaxHP = Character->GetMaxHP();
        State.Speed = Character->GetBaseStatus().m_SPD;
        State.bIsBoss = Character == m_Boss;
        m_RaidStates.Add(State);
    }

    ForceNetUpdate();
}

void ARaidBattleManager::OnRep_RaidState()
{
    // 複製される値ごとに呼ばれるので、次のフレームに1回へまとめる
    if (m_bNotifyPending) return;

    m_bNotifyPending = true;
    GetWorldTimerManager().SetTimerForNextTick(this, &ARaidBattleManager::NotifyRaidChanged);
}

void ARaidBattleManager::NotifyRaidChanged()
{
    m_bNotifyPending = false;

    // CharacterBaseのHP自体は現状レプリケートされていないため、Managerの共有値を各クライアントへ反映する。
    for (const FRaidCharacterState& State : m_RaidStates)
    {
        if (IsValid(State.Character))
        {
            State.Character->SetHP(State.CurrentHP);
        }
    }

    if (m_bRaidStarted)
    {
        CreateRaidBattleUI();
        for (const FRaidCharacterState& State : m_RaidStates)
        {
            if (State.bIsBoss)
            {
                if (AEnemyBase* Boss = Cast<AEnemyBase>(State.Character)) Boss->SetStatusUIVisibility(true);
                break;
            }
        }
    }

    OnRaidBattleChanged.Broadcast();
}

ARaidBattleCommandProxy* ARaidBattleManager::GetMyCommandProxy(APlayerController* PC) const
{
    UWorld* World = GetWorld();
    if (!World || !IsValid(PC)) return nullptr;

    for (TActorIterator<ARaidBattleCommandProxy> It(World); It; ++It)
    {
        if (It->GetOwner() == PC)
        {
            return *It;
        }
    }
    return nullptr;
}

bool ARaidBattleManager::IsMyTurn(APlayerController* PC) const
{
    return IsValid(PC)
        && m_bRaidStarted
        && !m_bRaidEnded
        && m_bWaitingForCommand
        && IsValid(m_CurrentCharacter)
        && PC->GetPawn() == m_CurrentCharacter;
}

void ARaidBattleManager::ShowDamage(ACharacterBase* Target, int32 Damage)
{
    if (!HasAuthority() || !IsValid(Target)) return;

    for (TActorIterator<ARaidBattleCommandProxy> It(GetWorld()); It; ++It)
    {
        It->ClientShowDamage(Target, Damage);
        It->ClientPlayHitFX(Target->GetActorLocation());
    }
}

void ARaidBattleManager::DisplayDamage(ACharacterBase* Target, int32 Damage)
{
    if (!IsValid(Target) || !m_DamageWidgetClass) return;

    APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
    if (!LocalPC) return;

    UDamageWidget* Widget = CreateWidget<UDamageWidget>(LocalPC, m_DamageWidgetClass);
    if (!Widget) return;

    Widget->Init(Target, Damage);
    Widget->AddToViewport();
}

void ARaidBattleManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARaidBattleManager, m_RaidStates);
    DOREPLIFETIME(ARaidBattleManager, m_TurnOrder);
    DOREPLIFETIME(ARaidBattleManager, m_CurrentCharacter);
    DOREPLIFETIME(ARaidBattleManager, m_bRaidStarted);
    DOREPLIFETIME(ARaidBattleManager, m_bWaitingForCommand);
    DOREPLIFETIME(ARaidBattleManager, m_bRaidEnded);
    DOREPLIFETIME(ARaidBattleManager, m_RaidResult);
}

