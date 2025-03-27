// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODEvent.h"
#include "IInteractable.h"
#include "PuzzleData.h"
#include "PuzzleGridCell.h"
#include "GameFramework/Actor.h"
#include "PuzzleGrid.generated.h"

class APristsWithGunsCharacter;

USTRUCT(BlueprintType)
struct FGridState
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<EGridCellType> CellTypes; // for game checks. we are not conserved
                                     // with the cell objects themselves, just their types
    UPROPERTY()
    FIntPoint MovablePosition;

    UPROPERTY()
    FIntPoint GridSize;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleCompletedDelegate);

UCLASS()
class PRISTSWITHGUNS_API APuzzleGrid : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    APuzzleGrid();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void Interact(ACharacter *Character) override;
    virtual FText GetInteractionText() const override;
    void InitPuzzleData();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Puzzle|Visual")
    TObjectPtr<class UBoxComponent> StartPuzzleCollider;

    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    TObjectPtr<class UCameraComponent> InspectingCamera;
    
    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    TObjectPtr<class UArrowComponent> DirectionIndicator;
    
    virtual void HandleMovableInput(const FVector2D& Input);

    void SetPlayer(APristsWithGunsCharacter* NewPlayer);
    
    TMap<char, EGridCellType> CharToCellTypeMap;

    UPROPERTY(BlueprintReadOnly, Category="Puzzle")
    bool bReachedGoal = false;

protected:
    
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category="Puzzle")
    bool bGenerateInEditor = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Puzzle")
    FDataTableRowHandle PuzzleDataHandle;
    
    FPuzzleData* CurrentPuzzleData;
    
    UPROPERTY(EditAnywhere, Category="Puzzle")
    FName PuzzleName;
    
    int32 GridSizeX;
    
    int32 GridSizeY;
    
    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    float CellSize = 10.0f;
    
    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    float ViewOffset = 10.0f;

    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    TObjectPtr<class UStaticMesh> EmptyCellMesh;

    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    TObjectPtr<class UStaticMesh> WallMesh;

    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    TObjectPtr<class UStaticMesh> MovableMesh;

    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    TObjectPtr<class UStaticMesh> GoalMesh;

    UPROPERTY()
    TArray<TObjectPtr<APuzzleGridCell>> GridCells;
    
    UPROPERTY(BlueprintReadWrite, Category="Puzzle")
    TObjectPtr<APuzzleGridCell> MovableCell;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
    TObjectPtr<UFmodEvent> SlidePieceSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
    TObjectPtr<UFmodEvent> PuzzleCompleteSound;

    UPROPERTY()
    FGridState CurrentState;

    UPROPERTY(EditAnywhere, Category="Puzzle|Visual")
    float ColorTransitionDuration = 0.1f;
    
    void InitializeGrid();
    void PlayWallToGreenTransition();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Puzzle|Movement")
    UCurveFloat* MovementCurve;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<APristsWithGunsCharacter> PuzzlePlayer; // player interacting with the puzzle

    FVector StartLocation;
    FVector EndLocation;

    UPROPERTY(EditAnywhere, Category = "Puzzle|Movement")
    float SlideDuration = 0.5f;

    UPROPERTY(BlueprintReadWrite, Category = "Puzzle|Movement")
    FVector CurrentMovableRelativeLocation;

    UFUNCTION(BlueprintImplementableEvent, Category="Puzzle|Movement")
    void MoveCellAnim(FVector TargetLocation);
    
    
    void UpdateGridVisuals();
    bool IsValidMove(const FIntPoint& NewPosition);

    UFUNCTION(BlueprintCallable, Category="Puzzle")
    bool CheckPuzzleCompletion();
    
    int32 GetCellIndex(const FIntPoint& Position) const;
    APuzzleGridCell* AddCellAtPosition(int32 x, int32 y, int32 z, EGridCellType CellType, bool addToGridState = true);
    
    UFUNCTION()
    void PrintGridCellTypes();

private:

};