#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BooksData.generated.h"


/***********************************************************************************
 *                                   Books Data
 *
 *      This holds the information needed to display the contents of a book
 *      Important: In order for the pages to be filled, I recommend using around
 *      360 characters for each page string.
 ***********************************************************************************/

USTRUCT(BlueprintType)
struct PRISTSWITHGUNS_API FBooksData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText BookName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Author;

    // Each string should have around 360 characters, give or take
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Pages; 
    
};
