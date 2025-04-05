// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue System/DialogOptionWidget.h"

#include "CrosshairWidget.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "PristsWithGuns/PristsWithGunsCharacter.h"

void UDialogOptionWidget::OnOptionButtonClicked()
{
    if(ClickedCallback)
    {
        
        if(APristsWithGunsCharacter* Character = Cast<APristsWithGunsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
        {
            if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
            {
                FInputModeGameOnly InputModeData;
                
                PC->SetInputMode(InputModeData);
                
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Exiting dialog"));
                
                PC->SetIgnoreMoveInput(false);
                PC->SetIgnoreLookInput(false);
                
                PC->ResetIgnoreMoveInput();
                PC->ResetIgnoreLookInput();

                
                PC->bShowMouseCursor = false; 
                
            }
        }
        
        ClickedCallback();
    }
}
