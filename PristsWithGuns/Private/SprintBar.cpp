// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintBar.h"

void USprintBar::NativeConstruct()
{
    Super::NativeConstruct();
}

void USprintBar::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if(bIsInCooldown)
    {
        return;
    }

    if (bIsStaminaDraining)
    {
        if (StaminaPercentage > 0.0f) // depleting
        {
            StaminaPercentage -= InDeltaTime * StaminaDrainageSpeed;
            UE_LOG(LogTemp, Display, TEXT("StaminaPercentage = %f"), StaminaPercentage);
        }
        else // Player ran out of stamina
        {
            bIsStaminaDraining = false;
            bIsInCooldown = true;
            GetWorld()->GetTimerManager().SetTimer(
                SprintTimerHandle,
                this,
                &USprintBar::StartRegenStamina,
                CooldownTime,
                false);
        }
    }

    if (bIsStaminaRegening && StaminaPercentage < 1.0f) // Regening
    {
        StaminaPercentage += InDeltaTime * StaminaDrainageSpeed;
        UE_LOG(LogTemp, Display, TEXT("StaminaPercentage = %f"), StaminaPercentage);
    }

}

void USprintBar::StartDrainingStamina()
{
    PlayRiseAnimation();
    bIsStaminaDraining = true;
    bIsStaminaRegening = false;
}

void USprintBar::StopDrainingStamina()
{
    bIsStaminaDraining = false;
    if(StaminaPercentage > 0.0f) // stopped before running out
    {
        StartRegenStamina();
    }
}

void USprintBar::StartRegenStamina()
{
    bIsInCooldown = false;
    bIsStaminaRegening = true;
}

float USprintBar::GetStaminaPercentage()
{
    return StaminaPercentage;
}

bool USprintBar::IsDrainingStamina() const
{
    return bIsStaminaDraining;
}