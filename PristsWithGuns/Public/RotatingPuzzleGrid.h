// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODBlueprintStatics.h"
#include "PuzzleGrid.h"
#include "Components/TimelineComponent.h"
#include "RotatingPuzzleGrid.generated.h"

struct FFMODEventInstance;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleExitDelegate);

UCLASS()
class PRISTSWITHGUNS_API ARotatingPuzzleGrid : public APuzzleGrid
{
    GENERATED_BODY()

public:
    ARotatingPuzzleGrid();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void HandleMovableInput(const FVector2D &Input) override;
    
    UPROPERTY(BlueprintAssignable, Category = "Puzzle")
    FOnPuzzleExitDelegate OnPuzzleExit;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor *PivotActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    AActor *MovableCellActor;

    UPROPERTY()
    UTimelineComponent* RotationTimeline;

    UFUNCTION()
    APuzzleGridCell* GetCellActorFromGridCoords(FIntPoint GridCoords);

    UPROPERTY(EditAnywhere)
    float fallSpeed = 0.4;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
    class UFMODAudioComponent* FmodAudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    UFmodEvent* WinningSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    UFmodEvent* RotatingSound;

private:
    FVector2D Directions[4];
    int8 NumDirections;
    int8 CurrentDirectionIndex;
    FVector MiddleLocation;
    
    FRotator InitialRotation;
    FRotator TargetRotation;

    FVector InitialCellPosition;
    FVector TargetCellPosition;

    UFUNCTION()
    bool UpdateTargetCellPosition(FVector2D Direction);

    UFUNCTION()
    void UpdateGridRotation(float Alpha);

    AActor* GetMovableCellActor() const;

    bool IsValidMoveNoGoalCheck(const FIntPoint &NewPosition);

    UPROPERTY(EditAnywhere)
    UCurveFloat* RotationCurve;

    bool bMovingCell;
    bool bMoveHappening;

    FFMODEventInstance CurrentSlidePieceSoundInstance;


};