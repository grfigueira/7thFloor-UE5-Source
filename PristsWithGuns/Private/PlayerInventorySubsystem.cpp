// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory System/PlayerInventorySubsystem.h"

#include "Inventory System/ItemsData.h"


void UPlayerInventorySubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);

    static const FString ItemsDataTablePath = TEXT("/Game/Data/DT_ItemsRegistry");
    ItemRegistry = LoadObject<UDataTable>(nullptr, *ItemsDataTablePath);

    // Initialize the inventory with default size
    InitializeInventory();

    // Add first item
    if (const FItemData *Row = ItemRegistry->FindRow<FItemData>("ElevatorKey01", TEXT("Item Lookup")); Row
        && Row->ItemBPClass)
    {
        TObjectPtr<UBaseItem> NewItem = NewObject<UBaseItem>(this, Row->ItemBPClass);
        AddItemWhereEmpty("PICard");
    }

}

void UPlayerInventorySubsystem::Deinitialize()
{
    Super::Deinitialize();
}

bool UPlayerInventorySubsystem::AddItemWhereEmpty(FName RowName)
{

    if (const FItemData *Row = ItemRegistry->FindRow<FItemData>(RowName, TEXT("Item Lookup")); Row
        && Row->ItemBPClass)
    {
        if (TObjectPtr<UBaseItem> NewItem = NewObject<UBaseItem>(this, Row->ItemBPClass))
        {
            for (int16 i = 0; i < Items.Num(); ++i)
            {
                if (!Items[i])
                {
                    Items[i] = NewItem;
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool UPlayerInventorySubsystem::AddItemAt(const TObjectPtr<UBaseItem> NewItem, int16 SlotIndex)
{
    if (!Items[SlotIndex])
    {
        Items[SlotIndex] = NewItem;
        return true;
    }
    return false;
}

bool UPlayerInventorySubsystem::TakeItemAt(const int16 SlotIndex, TObjectPtr<UBaseItem> &OutItem)
{
    if (SlotIndex < 0 || SlotIndex > Items.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("TakeItemAt: SlotIndex out of bounds"));
        return false;
    }
    OutItem = Items[SlotIndex];
    Items[SlotIndex] = nullptr;
    return true;
}

const TArray<TObjectPtr<UBaseItem>> &UPlayerInventorySubsystem::GetHeldItems()
{
    return Items;
}

void UPlayerInventorySubsystem::InitializeInventory(int32 Size)
{
    if (Size >= 0)
    {
        Items.SetNum(Size);
    }

}