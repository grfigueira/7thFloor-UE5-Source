// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "UObject/Interface.h"
#include "Item.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PRISTSWITHGUNS_API IItem
{
	GENERATED_BODY()

public:

    virtual FString GetName() = 0;
    
    virtual TObjectPtr<UImage> GetItemSlotImage() const = 0;

    // What happens when this item is selected from the inventory UI
    virtual void UseAction() = 0;

    virtual void InspectAction() = 0;

    
};
