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

    // Currently held items by the player
    TArray<TObjectPtr<UBaseItem>> Items;

    // Every possible item in the game 
    UPROPERTY()
    TObjectPtr<UDataTable> ItemRegistry;

public:

    void Initialize(FSubsystemCollectionBase &Collection) override;
    virtual void Deinitialize() override;
    
    // Adds an item in the first empty slot it finds.
    // returns: false if the inventory is full,
    //          true otherwise

    UFUNCTION(BlueprintCallable)
    bool AddItemWhereEmpty(FName RowName);

    // Adds an item in a specific slot.
    // returns: false if there was a item already in that slot
    //          true if it succeeded.
    bool AddItemAt(const TObjectPtr<UBaseItem> NewItem, int16 SlotIndex);

    // Set the slot to null
    // returns: false if there slot was empty
    //          true if it succeeded, with the taken item being
    //          passed to `OutItem` argument

    bool TakeItemAt(const int16 SlotIndex, TObjectPtr<UBaseItem>& OutItem);

    const TArray<TObjectPtr<UBaseItem>>& GetHeldItems();

    void InitializeInventory(int32 Size = 6);

    
};