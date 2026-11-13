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
#include "Engine/Engine.h"

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

    // 最初の試行時だけ表示する
    if (m_InitializeAttempts == 0 && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 8.0f, FColor::Yellow,
            FString::Printf(
                TEXT("Raid Init Check: Authority=%s / Boss=%s"),
                HasAuthority() ? TEXT("TRUE") : TEXT("FALSE"),
                IsValid(m_Boss) ? *m_Boss->GetName() : TEXT("NONE")
            )
        );
    }

    ++m_InitializeAttempts;

    if (m_InitializeAttempts >= 120)
    {
        GetWorldTimerManager().ClearTimer(m_InitializeTimer);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 15.0f, FColor::Red,
                TEXT("Raid initialization FAILED")
            );
        }
    }
}

bool ARaidBattleManager::InitializeRaid()
{
    if (!HasAuthority() || m_bRaidStarted) return m_bRaidStarted;

    // ボスはレイドマップに配置し、ManagerのDetailsで指定する。
    if (!IsValid(m_Boss))
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, TEXT("Raid: Boss is NONE"));

        return false;
    }

    TArray<ACharacterBase*> Participants;

    int32 ControllerCount = 0;

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();

        if (!IsValid(PC))
            continue;

        ++ControllerCount;

        APawn* Pawn = PC->GetPawn();
        ACharacterBase* Character = Cast<ACharacterBase>(Pawn);

        if (GEngine)
        {
            FString PawnName = IsValid(Pawn)
                ? Pawn->GetClass()->GetName()
                : TEXT("NONE");

            GEngine->AddOnScreenDebugMessage(
                -1,
                15.0f,
                FColor::Cyan,
                FString::Printf(
                    TEXT("PC: %s / Pawn: %s / CharacterBase: %s"),
                    *PC->GetName(),
                    *PawnName,
                    IsValid(Character) ? TEXT("TRUE") : TEXT("FALSE")
                )
            );
        }

        if (!IsValid(Character) || Character == m_Boss)
            continue;

        Character->SetReplicates(true);
        Character->SetReplicateMovement(true);
        Participants.AddUnique(Character);
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.0f,
            FColor::Yellow,
            FString::Printf(
                TEXT("ControllerCount: %d / Participants: %d"),
                ControllerCount,
                Participants.Num()
            )
        );
    }

    if (Participants.IsEmpty())
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, TEXT("Raid: No Participants"));

        return false;
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
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 15.0f, FColor::Green,
            FString::Printf(TEXT("Raid Started! Players: %d"), Participants.Num())
        );
    }
    m_bRaidEnded = false;
    m_RaidResult = ERaidBattleResult::NONE;
    AdvanceTurn();
    RefreshReplicatedStates();
    OnRaidBattleChanged.Broadcast();
    ForceNetUpdate();

    UE_LOG(LogTemp, Warning, TEXT("Raid started. Participants: %d, Boss: %s"), Participants.Num(), *m_Boss->GetName());
    return true;
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

void ARaidBattleManager::HandlePlayerCommand(APlayerController* RequestingController,ERaidCommandType Command,ACharacterBase* Target)
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
    // CharacterBaseのHP自体は現状レプリケートされていないため、Managerの共有値を各クライアントへ反映する。
    for (const FRaidCharacterState& State : m_RaidStates)
    {
        if (IsValid(State.Character))
        {
            State.Character->SetHP(State.CurrentHP);
        }
    }

    OnRaidBattleChanged.Broadcast();
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
