// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SprintBar.generated.h"

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API USprintBar : public UUserWidget
{
    GENERATED_BODY()

public:
    
    UFUNCTION()
    void StartDrainingStamina();

    UFUNCTION()
    void StopDrainingStamina();
    
    UFUNCTION()
    void StartRegenStamina();
    
    UFUNCTION()
    float GetStaminaPercentage();

    UFUNCTION(BlueprintImplementableEvent)
    void PlayRiseAnimation();
    
    UFUNCTION(BlueprintImplementableEvent)
    void PlayHideAnimation();

    virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
    virtual void NativeConstruct() override;

protected:

    UPROPERTY(EditAnywhere)
    float StaminaDrainageSpeed;
    
    UPROPERTY(EditAnywhere)
    float CooldownTime;
    
    UPROPERTY(BlueprintReadOnly)
    float StaminaPercentage = 1.0; // 0 < StaminaPercentage < 1

    UFUNCTION(BlueprintCallable)
    bool IsDrainingStamina() const;

private:
    bool bIsStaminaDraining;
    bool bIsStaminaRegening;
    bool bIsInCooldown;
    FTimerHandle SprintTimerHandle;

    

};