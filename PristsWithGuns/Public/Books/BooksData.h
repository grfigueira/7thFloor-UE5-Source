#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BooksData.generated.h"

USTRUCT(BlueprintType)
struct PRISTSWITHGUNS_API FBooksData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText BookName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Author;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Pages;
    
};
