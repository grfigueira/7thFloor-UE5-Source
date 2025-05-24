#pragma once

#include "CoreMinimal.h"
#include "ItemInspectionDisplay.h"
#include "Engine/DataTable.h"
#include "Inventory System/BaseItem.h"
#include "ItemsData.generated.h"


USTRUCT(BlueprintType)
struct PRISTSWITHGUNS_API FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
    TSubclassOf<UBaseItem> ItemBPClass;
    
};