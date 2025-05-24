// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInspection.h"

#include "ItemInspectionDisplay.h"
#include "Components/TextBlock.h"

UItemInspection::UItemInspection(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer),
ItemName(nullptr), CurrentItem(nullptr), CurrentItemClass(nullptr)
{
    CurrentItem = nullptr;
}

void UItemInspection::SetItemToDisplay(const TSubclassOf<AItemInspectionDisplay>& ItemClass, const FText &Name)
{
    
    if (CurrentItem.IsValid())
    {
        CurrentItem->Destroy();
        CurrentItem = nullptr;
    }

    CurrentItemClass = ItemClass;

    if (ItemName)
    {
        ItemName->SetText(Name);
    }

    if (!CurrentItemClass || !GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("Can't spawn display actor! No display class or no world!"));
        return;
    }
    
    // Spawn the item actor (with scene capture) somewhere in the level
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const FVector SpawnLocation = FVector(0, 0, -10000);
    const FRotator SpawnRotation = FRotator::ZeroRotator;

    CurrentItem = MakeWeakObjectPtr(GetWorld()->SpawnActor<AItemInspectionDisplay>(
                                            CurrentItemClass,
                                            SpawnLocation,
                                            SpawnRotation,
                                            SpawnParameters));
}

void UItemInspection::SetIsRotating(const bool bNewIsRotating) const
{
    if(CurrentItem.IsValid())
    {
        CurrentItem->SetIsRotating(bNewIsRotating);
    }   
}
