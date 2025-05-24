// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory System/InventorySlotWidget.h"

#include "ItemInspectionDisplay.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PristsWithGuns/PristsWithGunsCharacter.h"

void UInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if(SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotButtonClicked);
    }

}

void UInventorySlotWidget::OnDoubleClick() 
{
    if (const APristsWithGunsCharacter* PlayerCharacter = Cast<APristsWithGunsCharacter>(
        UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        if(ItemInspectionClass)
        {
            PlayerCharacter->OpenItemInspection(ItemInspectionClass, ItemName); 
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("BAD!!! ItemInspectionClass was NULL in OnDoubleClick() !!"))
        }
    }
}

void UInventorySlotWidget::OnSlotButtonClicked()
{
    if(bWaitingForDoubleClick)
    {
        bWaitingForDoubleClick = false;
        GetWorld()->GetTimerManager().ClearTimer(DoubleClickTimerHandle);
        OnDoubleClick(); 
    }
    else
    {
        bWaitingForDoubleClick = true;
        GetWorld()->GetTimerManager().SetTimer(
             DoubleClickTimerHandle,
             [this]()
             {
                 bWaitingForDoubleClick = false;
                 // Handle single click here if needed
             },
             DoubleClickTime,
             false
         ); 
    }
}

void UInventorySlotWidget::SetItemInspectionData(const TSubclassOf<AItemInspectionDisplay> &_ItemInspectionClass, const FName& Name)
{
    ItemInspectionClass = _ItemInspectionClass;
    ItemName = FText::FromName(Name);
}