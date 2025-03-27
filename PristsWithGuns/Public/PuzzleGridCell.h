// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleGridCell.generated.h"

UENUM()
enum class EGridCellType : uint8
{
    WALL,
    INNER_WALL,
    EMPTY,
    MOVABLE,
    GOAL,
};

inline FString EGridCellTypeToString(EGridCellType CellType)
{
    switch (CellType)
    {
    case EGridCellType::WALL:
        return "#";
    case EGridCellType::INNER_WALL:
        return "x";
    case EGridCellType::EMPTY:
        return "o";
    case EGridCellType::MOVABLE:
        return "P";
    case EGridCellType::GOAL:
        return "G";
    default:
        return "?";
    }
}


UCLASS()
class PRISTSWITHGUNS_API APuzzleGridCell : public AActor
{
	GENERATED_BODY()
	
public:	
	APuzzleGridCell();
        UPROPERTY(EditAnywhere, Category="Grid")
        class UStaticMeshComponent* Mesh;

        UPROPERTY(EditAnywhere, Category="Grid")
        EGridCellType CellType;

        UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid")
        FIntPoint GridPosition;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
        TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;
    
        void InitializeCell();
    
        void LerpWallColor(FLinearColor StartColor, FLinearColor EndColor, float Duration);

    protected:
	virtual void BeginPlay() override;

private:
    FTimerHandle ColorLerpHandle;

public:	
	virtual void Tick(float DeltaTime) override;

};
