#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PuzzleData.generated.h"

USTRUCT(BlueprintType)
struct PRISTSWITHGUNS_API FPuzzleData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> GridRowsData;
};