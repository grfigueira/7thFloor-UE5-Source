// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory System/InventoryWidget.h"

#include "Inventory System/InventorySlotWidget.h"
#include "Interfaces/Item.h"
#include "Components/WrapBox.h"

void UInventoryWidget::UpdateInventoryState(TArray<TScriptInterface<IItem>> &CurrentInventory) const
{
    if(!WrapBox)
    {
        TArray<UWidget*> SlotWidgets = WrapBox->GetAllChildren();

        for (int16 i = 0; i < SlotWidgets.Num(); ++i)
        {
            if(UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(SlotWidgets[i]))
            {
                if(CurrentInventory[i])
                {
                    SlotWidget->ItemImage = CurrentInventory[i].GetInterface()->GetItemSlotImage();
                    SlotWidget->ItemImage->SetRenderOpacity(1.0f);
                }
                else
                {
                    SlotWidget->ItemImage->SetRenderOpacity(0.0f);
                }
                
            }
        }
    }
}