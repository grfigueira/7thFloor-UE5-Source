// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.generated.h"

class UImage;
/***********************************************************************
 *   Every game item should inherit from this BaseItem
 *
 *   Ideally, we should never need to cast to the "actual" item
 *   All the behaviour should be abstracted here, so we don't
 *   have to keep track of ItemTypes or cast for every possible type
 **********************************************************************/
UCLASS(Blueprintable, Abstract)
class PRISTSWITHGUNS_API UBaseItem : public UObject
{
    GENERATED_BODY()

public:

    // What happens when this item is selected from the inventory UI
    virtual void UseAction();

    virtual void InspectAction();

    UPROPERTY(EditAnywhere, Category="Item Properties")
    FName DisplayName;

    UPROPERTY(EditAnywhere, Category="Item Properties")
    TObjectPtr<UTexture2D> SlotImage;; // In inventory
    

};