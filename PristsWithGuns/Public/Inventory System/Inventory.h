// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"


class IItem;
/**
 * 
 */
class PRISTSWITHGUNS_API FInventory
{
public:
	FInventory();
	~FInventory();

protected:

        TArray<TScriptInterface<IItem>> Items;
        
        void InitializeInventory(int32 Size = 6);

public:

    // Adds an item in the first empty slot it finds
    void AddItemWhereEmpty(const TScriptInterface<IItem>& );

    // Adds an item in a specific slot
    void AddItemAt(const TScriptInterface<IItem> &, int16);

    // Set the slot to null
    TScriptInterface<IItem> TakeItemAt(int16 SlotIndex);

        
};
