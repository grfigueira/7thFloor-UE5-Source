// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerInventorySubsystem.generated.h"

class UBaseItem;
/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UPlayerInventorySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

protected:
    TArray<TObjectPtr<UBaseItem>> Items;

    UPROPERTY()
    TObjectPtr<UDataTable> AvailableItems; // Every possible item


public:
    // Adds an item in the first empty slot it finds
    void AddItemWhereEmpty(const  TObjectPtr<UBaseItem>);

    // Adds an item in a specific slot
    void AddItemAt(const TObjectPtr<UBaseItem>, int16);

    // Set the slot to null
    TObjectPtr<UBaseItem> TakeItemAt(int16 SlotIndex);

    void InitializeInventory(int32 Size = 6);

};