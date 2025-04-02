// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory System/Inventory.h"

FInventory::FInventory()
{
    InitializeInventory();
}

FInventory::~FInventory()
{
}

void FInventory::InitializeInventory(int32 Size)
{
    Items.SetNum(6);
}

void FInventory::AddItemWhereEmpty(const TScriptInterface<IItem> &NewItem)
{
    for (auto& Item: Items)
    {
        if(Item == nullptr)
        {
            Item = NewItem;
        }
    }   
}

// Slot must be empty (?)
void FInventory::AddItemAt(const TScriptInterface<IItem> &Item, const int16 SlotIndex)
{
    TScriptInterface<IItem>& CurrentItem = Items[SlotIndex];
    
    if(CurrentItem)
    {
        return;
    }
    
    CurrentItem = Item;
}

TScriptInterface<IItem> FInventory::TakeItemAt(const int16 SlotIndex)
{
    return Items[SlotIndex];
}
