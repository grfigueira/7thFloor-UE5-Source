// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGrid.h"

#include "PuzzleData.h"
#include "PuzzleGridCell.h"
#include "FMODBlueprintStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "PristsWithGuns/PristsWithGunsCharacter.h"

// Sets default values
APuzzleGrid::APuzzleGrid()
{
    CharToCellTypeMap.Add('#', EGridCellType::WALL);
    CharToCellTypeMap.Add('x', EGridCellType::INNER_WALL);
    CharToCellTypeMap.Add('o', EGridCellType::EMPTY);
    CharToCellTypeMap.Add('P', EGridCellType::MOVABLE);
    CharToCellTypeMap.Add('G', EGridCellType::GOAL);

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    DirectionIndicator = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionIndicator"));
    RootComponent = DirectionIndicator;

    InspectingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InspectingCamera"));
    InspectingCamera->SetupAttachment(DirectionIndicator);
    InspectingCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

    StartPuzzleCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("StartPuzzleCollider"));
    StartPuzzleCollider->SetupAttachment(DirectionIndicator);
    StartPuzzleCollider->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));

    StartPuzzleCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // No physics, just traces/overlaps
    StartPuzzleCollider->SetCollisionObjectType(ECC_WorldDynamic);
    StartPuzzleCollider->SetCollisionResponseToAllChannels(ECR_Block); // Block traces by default

}

void APuzzleGrid::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);

    if (bGenerateInEditor)
    {
        // cleanup
        for (APuzzleGridCell *Cell : GridCells)
        {
            if (Cell)
            {
                Cell->Destroy();
            }
        }

        GridCells.Empty();

        TArray<UChildActorComponent *> CellChildComponents;
        GetComponents<UChildActorComponent>(CellChildComponents);

        for (UChildActorComponent *CellComponent : CellChildComponents)
        {
            if (CellComponent)
            {
                CellComponent->DestroyChildActor();
                if (CellComponent)
                {
                    CellComponent->DestroyComponent();
                }

            }
        }
        InitializeGrid();
    }
}

void APuzzleGrid::Interact(ACharacter *Character)
{
    if (bReachedGoal)
    {
        return;
    }

    if (APristsWithGunsCharacter *ActualCharacter = Cast<APristsWithGunsCharacter>(Character))
    {
        if (!ActualCharacter->bIsInteracting)
        {
            ActualCharacter->EnterGridPuzzle(this);
        }
        else
        {
            ActualCharacter->ExitGridPuzzle();
        }
    }

}

FText APuzzleGrid::GetInteractionText() const
{
    return FText::FromString("Attempt Puzzle");
}

void APuzzleGrid::InitPuzzleData()
{
    if (!PuzzleDataHandle.IsNull())
    {
        CurrentPuzzleData = PuzzleDataHandle.GetRow<FPuzzleData>("");
        if (CurrentPuzzleData)
        {
            GridSizeX = CurrentPuzzleData->GridRowsData[0].Len(); // Assuming square grid, for now.
            GridSizeY = CurrentPuzzleData->GridRowsData.Num();
            GridCells.SetNum(GridSizeX * GridSizeY);
            UE_LOG(LogTemp, Warning, TEXT("PuzzleGrid: grid size %d %d"), GridSizeX, GridSizeY);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("PuzzleGrid: failed to get puzzle data"));
        }
    }
}

// Called when the game starts or when spawned
void APuzzleGrid::BeginPlay()
{
    Super::BeginPlay();

    InitializeGrid();

    UE_LOG(LogTemp, Warning, TEXT("PuzzleGrid: gridcells size %d"), GridCells.Num());
    
}

void APuzzleGrid::InitializeGrid()
{
    InitPuzzleData();

    if (!CurrentPuzzleData)
    {
        UE_LOG(LogTemp, Error, TEXT("PuzzleGrid: No puzzle data"));
        return;
    }

    CurrentState.GridSize = FIntPoint(GridSizeX, GridSizeY);
    CurrentState.CellTypes.SetNum(GridSizeX * GridSizeY);

    //parse from the bottom up
    for (int32 y = GridSizeY - 1; y >= 0; y--)
    {
        FString RowData = CurrentPuzzleData->GridRowsData[y];

        for (int32 x = 0; x < GridSizeX; x++)
        {

            if (x >= RowData.Len())
            {
                UE_LOG(LogTemp, Error, TEXT("PuzzleGrid: x out of bounds of row data"));
                break;
            }

            char CellTypeChar = RowData[x];

            // unknown cell type error check
            if (!CharToCellTypeMap.Contains(CellTypeChar))
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                                 FString::Printf(TEXT("Unknown cell type %c"), CellTypeChar));
                return;
            }

            EGridCellType CellType = CharToCellTypeMap[CellTypeChar];

            if (CellType == EGridCellType::WALL || CellType == EGridCellType::INNER_WALL)
            {
                AddCellAtPosition(0 + ViewOffset, x, GridSizeY - 1 - y, CellType);
                AddCellAtPosition(1 + ViewOffset, x, GridSizeY - 1 - y, EGridCellType::EMPTY, false);
            }
            else if (CellType == EGridCellType::MOVABLE)
            {

                MovableCell = AddCellAtPosition(ViewOffset, x, GridSizeY - 1 - y, EGridCellType::MOVABLE, true);
                AddCellAtPosition(1 + ViewOffset, x, GridSizeY - 1 - y, EGridCellType::EMPTY, false);
                if (MovableCell == nullptr)
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to place movable cell"));
                    return;
                }
                CurrentState.MovablePosition = FIntPoint(x, GridSizeY - 1 - y);
            }
            else if (CellType == EGridCellType::EMPTY)
            {
                AddCellAtPosition(1 + ViewOffset, x, GridSizeY - 1 - y, EGridCellType::EMPTY);
            }
            else if (CellType == EGridCellType::GOAL)
            {
                AddCellAtPosition(1 + ViewOffset, x, GridSizeY - 1 - y, EGridCellType::GOAL);
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Purple,
                                                 FString::Printf(TEXT("Unknown cell type %c"), CellTypeChar));

                for (auto &Elem : CharToCellTypeMap)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Purple,
                                                     FString::Printf(TEXT("Key: %c Value: %d"), Elem.Key, Elem.Value));
                }
                return;
            }

        }
    }
}

void APuzzleGrid::PlayWallToGreenTransition()
{
    for (APuzzleGridCell *Cell : GridCells)
    {
        if (Cell && Cell->CellType == EGridCellType::WALL)
        {
            FLinearColor StartColor;
            Cell->DynamicMaterial->GetVectorParameterValue(TEXT("BaseColor"), StartColor);
            Cell->LerpWallColor(StartColor, FLinearColor::Green, ColorTransitionDuration);
        }
    }
}

// Probably not needed
void APuzzleGrid::UpdateGridVisuals()
{
}

void APuzzleGrid::HandleMovableInput(const FVector2D &Input)
{
    if (bReachedGoal)
    {
        return;
    }
    FIntPoint NewPosition = CurrentState.MovablePosition + FIntPoint(Input.X, Input.Y);

    if (IsValidMove(NewPosition))
    {
        int32 adjustedX = NewPosition.X;
        int32 adjustedY = NewPosition.Y;
        int32 adjustedZ = ViewOffset;

        MoveCellAnim(
            FVector(adjustedZ * CellSize, adjustedX * CellSize, adjustedY * CellSize));

        FFMODEventInstance InstanceWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SlidePieceSound, false);
        UFMODBlueprintStatics::EventInstanceSetParameter(InstanceWrapper, FName("Speed"), FMath::RandRange(0.95, 1.05));
        UFMODBlueprintStatics::EventInstancePlay(InstanceWrapper);

        CurrentState.CellTypes[GetCellIndex(CurrentState.MovablePosition)] = EGridCellType::EMPTY;
        CurrentState.CellTypes[GetCellIndex(NewPosition)] = EGridCellType::MOVABLE;
        CurrentState.MovablePosition = NewPosition;
    }

}

void APuzzleGrid::SetPlayer(APristsWithGunsCharacter *NewPlayer)
{
    PuzzlePlayer = NewPlayer;
}

bool APuzzleGrid::IsValidMove(const FIntPoint &NewPosition)
{
    if (CurrentState.CellTypes[GetCellIndex(NewPosition)] == EGridCellType::GOAL)
    {

        bReachedGoal = true;
        PlayWallToGreenTransition();
        return true;
    }
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Cell type is %s"), *EGridCellTypeToString(CurrentState.CellTypes[GetCellIndex(NewPosition)]))); 
    return CurrentState.CellTypes[GetCellIndex(NewPosition)] == EGridCellType::EMPTY;
}

bool APuzzleGrid::CheckPuzzleCompletion()
{
    if (bReachedGoal)
    {
        UFMODBlueprintStatics::PlayEvent2D(GetWorld(), PuzzleCompleteSound, true);
    }
    return bReachedGoal;
}

int32 APuzzleGrid::GetCellIndex(const FIntPoint &Position) const
{
    return Position.Y * GridSizeX + Position.X;
}

APuzzleGridCell *APuzzleGrid::AddCellAtPosition(int32 x, int32 y, int32 z, EGridCellType CellType, bool addToGridState)
{

    APuzzleGridCell *NewCell = GetWorld()->SpawnActor<APuzzleGridCell>(
        APuzzleGridCell::StaticClass(),
        FVector::ZeroVector,
        FRotator::ZeroRotator);

    if (!NewCell)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn cell"));
        return nullptr;
    }

    NewCell->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    FVector Location = FVector(x * CellSize, y * CellSize, z * CellSize);
    if (CellType == EGridCellType::MOVABLE)
    {
        CurrentMovableRelativeLocation = Location;
    }
    NewCell->SetActorRelativeLocation(Location);
    NewCell->CellType = CellType;
    switch (CellType)
    {
    case EGridCellType::EMPTY:
        NewCell->Mesh->SetStaticMesh(EmptyCellMesh);
        break;
    case EGridCellType::WALL:
        NewCell->Mesh->SetStaticMesh(WallMesh);
        NewCell->InitializeCell();
        break;
    case EGridCellType::INNER_WALL:
        NewCell->Mesh->SetStaticMesh(WallMesh);
        NewCell->InitializeCell();
        if(NewCell->DynamicMaterial.IsValid())
        {
            NewCell->DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(0.065f, 0.065f, 0.065f, 1.0f));    
        }
        break;
    case EGridCellType::MOVABLE:
        NewCell->Mesh->SetStaticMesh(MovableMesh);
        break;
    case EGridCellType::GOAL:
        NewCell->Mesh->SetStaticMesh(GoalMesh);
        break;
    default:
        UE_LOG(LogTemp, Error, TEXT("Unknown cell type"));
        break;
    }

    if (addToGridState)
    {
        NewCell->GridPosition = FIntPoint(y, z);

        int32 CellIndex = GetCellIndex(FIntPoint(y, z));

        if (GridCells[CellIndex])
        {
            GridCells[CellIndex]->Destroy();
        }

        GridCells[CellIndex] = NewCell;
        CurrentState.CellTypes[CellIndex] = CellType;
    }
    return NewCell;
}

void APuzzleGrid::PrintGridCellTypes()
{
    try
    {
        FString buffer;
        for (int x = 0; x < CurrentState.CellTypes.Num(); x++)
        {
            if (x % GridSizeX == 0)
            {
                GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Silver, FString::Printf(TEXT("%s"), *buffer));
                buffer = "";
            }
            buffer += EGridCellTypeToString(CurrentState.CellTypes[x]);
        }
        GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Silver, FString::Printf(TEXT("%s"), *buffer));
    }
    catch (const std::exception &e)
    {
        GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("Error: %s"), e.what()));
    }

}