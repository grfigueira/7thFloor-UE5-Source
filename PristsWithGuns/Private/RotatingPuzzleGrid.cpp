// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPuzzles/RotatingPuzzleGrid.h"
#include "Components/TimelineComponent.h"
#include "Engine/World.h"
#include "PristsWithGuns/PristsWithGunsCharacter.h"

#define PRINT_DEBUG_MESSAGE(Duration, Message) \
if (GEngine) \
{ \
GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Cyan, Message); \
}

ARotatingPuzzleGrid::ARotatingPuzzleGrid()
{

    PrimaryActorTick.bCanEverTick = true;

    RotationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotationTimeline"));

    // Create an FMOD Component
    FmodAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Footsteps Audio Component"));
    FmodAudioComponent->SetupAttachment(RootComponent);
    FmodAudioComponent->SetAutoActivate(false);
    FmodAudioComponent->Event = RotatingSound;

    Directions[0] = FVector2D(0, -1);
    Directions[1] = FVector2D(1, 0);
    Directions[2] = FVector2D(0, 1);
    Directions[3] = FVector2D(-1, 0);
    NumDirections = std::size(Directions);

    // User presses right -> index++ && rotate +90(?)
    // User presses left -> index--
    CurrentDirectionIndex = 0;

}


void ARotatingPuzzleGrid::BeginPlay()
{
    Super::BeginPlay();

    if (RotationCurve)
    {
        // Rotate board
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("UpdateGridRotation"));
        RotationTimeline->AddInterpFloat(RotationCurve, TimelineCallback);
    }

    MovableCellActor = GetMovableCellActor();
    if (!MovableCellActor)
    {
        return;
    }

    TArray<AActor *> AttachedActors;
    GetAttachedActors(AttachedActors);

    for (AActor *AttachedActor : AttachedActors)
    {
        if (AttachedActor != PivotActor)
        {
            AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            AttachedActor->AttachToActor(PivotActor, FAttachmentTransformRules::KeepWorldTransform);
        }
    }

}

void ARotatingPuzzleGrid::HandleMovableInput(const FVector2D &Input)
{
    if (Input.X == 0 || bReachedGoal || bMovingCell || RotationTimeline->IsPlaying())
    {
        PRINT_DEBUG_MESSAGE(5.0, TEXT("Can't handle input right now"));
        PRINT_DEBUG_MESSAGE(5.0, FString::Printf(TEXT("bReachedGoal: %d"), bReachedGoal));
        PRINT_DEBUG_MESSAGE(5.0, FString::Printf(TEXT("bMovingCell: %d"), bMovingCell));
        PRINT_DEBUG_MESSAGE(5.0, FString::Printf(TEXT("bMoveHappening: %d"), bMoveHappening));
        return;
    }

    if (!MovableCellActor)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MovableCellActor is null in HandleMovableInput"));
        return;
    }

    // should be true until the movable piece has stopped moving
    bMoveHappening = true;
    FmodAudioComponent->Play();

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
                                     FString::Printf(TEXT("static cast is %d"), static_cast<int>(Input.X)));

    // update direction for gravity
    CurrentDirectionIndex = (CurrentDirectionIndex + static_cast<int>(Input.X) + NumDirections) % NumDirections;

    InitialRotation = PivotActor->GetRootComponent()->GetRelativeRotation();
    TargetRotation = InitialRotation + FRotator(0, 0, 90 * Input.X);

    // the rotation of the board itself is handled by the timeline
    RotationTimeline->PlayFromStart();
}

void ARotatingPuzzleGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RotationTimeline->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);

    if (bMovingCell)
    {
        FVector Temp = TargetCellPosition;
        
        // Makes sure the Y value (depth) doesn't change
        Temp.X = MovableCellActor->GetRootComponent()->GetRelativeLocation().X;

        FVector NewLocation = FMath::VInterpConstantTo(
            MovableCellActor->GetRootComponent()->GetRelativeLocation(),
            Temp,
            DeltaTime,
            fallSpeed
            );
        MovableCellActor->SetActorRelativeLocation(NewLocation);

        float DistanceSquared = FVector::DistSquared(NewLocation, TargetCellPosition);
        UE_LOG(LogTemp, Warning, TEXT("Distance Squared: %f"), DistanceSquared);
        
        // This magic number is the visibly acceptable distance from which we can declare
        // the cell has reached its destination.
        if (FVector::DistSquared(NewLocation, TargetCellPosition) <= 10000.0f)
        {

            MovableCellActor->SetActorRelativeLocation(Temp);
            bMoveHappening = false;
            bMovingCell = false;
            UFMODBlueprintStatics::EventInstanceStop(CurrentSlidePieceSoundInstance);
            PRINT_DEBUG_MESSAGE(5.0, TEXT("Stopped moving cell"));
        }

    }
}

/**
 * Updates the target cell position based on the given direction.
 *
 * @param Direction - 2D vector direction in which the cell should move.
 * @return True if the cell moves to a new position, false if it is already at the target position
 *         (e.g., in a corner or blocked by a wall).
 */
bool ARotatingPuzzleGrid::UpdateTargetCellPosition(FVector2D Direction)
{
    GEngine->AddOnScreenDebugMessage(-1, 900.f, FColor::Emerald,
                                     FString::Printf(TEXT("Direction: %s"), *Direction.ToString()));

    FIntPoint TempPosition = CurrentState.MovablePosition + FIntPoint(Direction.X, Direction.Y);

    // first, check if the move isn't against a wall and can't move
    if (!IsValidMoveNoGoalCheck(TempPosition))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Invalid move from the start"));
        return false;
    }

    // now keep updating the position until we hit a wall
    while (IsValidMoveNoGoalCheck(TempPosition + FIntPoint(Direction.X, Direction.Y)))
    {
        TempPosition = TempPosition + FIntPoint(Direction.X, Direction.Y);
    }

    // get the cell currently in the target position
    if (APuzzleGridCell *TargetCell = GetCellActorFromGridCoords(TempPosition))
    {
        TargetCellPosition = TargetCell->GetRootComponent()->GetRelativeLocation();
        CurrentState.MovablePosition = TempPosition;

        // this one is a bit hacky
        // if we know that the cell will reach the goal spot, we use some timers to wait a bit and then play the winning
        // behavior
        if (CurrentState.CellTypes[GetCellIndex(TempPosition)] == EGridCellType::GOAL)
        {
            bReachedGoal = true;

            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(TimerHandle, [this]()
            {
                PlayWallToGreenTransition();
                FmodAudioComponent->SetEvent(WinningSound);
                FmodAudioComponent->Play();
                FTimerHandle TimerHandle2;
                GetWorldTimerManager().SetTimer(TimerHandle2, [this]()
                {
                    PuzzlePlayer->ExitGridPuzzle();

                    FTimerHandle TimerHandle3;
                    GetWorldTimerManager().SetTimer(TimerHandle3, [this]()
                    {
                        OnPuzzleExit.Broadcast();
                    }, 0.8f, false);

                }, 1.0f, false);
            }, .2f, false);

        }
        return true;
    }

    return false;

}

// Timeline that rotates the grid itself
void ARotatingPuzzleGrid::UpdateGridRotation(float Alpha)
{
    FRotator NewRotation = FMath::Lerp(InitialRotation, TargetRotation, Alpha);

    PivotActor->SetActorRelativeRotation(NewRotation);

    // move the movable cell only when grid rotation finishes
    if (1 - Alpha < 0.001)
    {
        CurrentMovableRelativeLocation = MovableCellActor->GetActorLocation();

        // check what the target cell position will be and update the variable
        if (UpdateTargetCellPosition(Directions[CurrentDirectionIndex]))
        {
            // True until the movable piece has stopped moving.
            // This bool "signals" the tick function to start moving the piece.
            bMovingCell = true;
            
            CurrentSlidePieceSoundInstance = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SlidePieceSound, true);
        }
    }

}

AActor *ARotatingPuzzleGrid::GetMovableCellActor() const
{
    for (APuzzleGridCell *Cell : GridCells)
    {
        if (Cell->CellType == EGridCellType::MOVABLE)
        {
            return Cell;
        }
    }
    return nullptr;
}

APuzzleGridCell *ARotatingPuzzleGrid::GetCellActorFromGridCoords(FIntPoint GridCoords)
{
    return GridCells[GetCellIndex(GridCoords)];
}

bool ARotatingPuzzleGrid::IsValidMoveNoGoalCheck(const FIntPoint &NewPosition)
{
    return CurrentState.CellTypes[GetCellIndex(NewPosition)] == EGridCellType::EMPTY ||
           CurrentState.CellTypes[GetCellIndex(NewPosition)] == EGridCellType::GOAL;
}