// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MyTweeningSubsystem.generated.h"


USTRUCT()
struct FActiveCurveAnim
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UCurveFloat> Curve;

    float PlaybackRate = 1.0f;

    float CurrentTime = 0.0f;

    float MaxTime = 0.0f;

    TFunction<void (float)> Callback;

    FActiveCurveAnim()
    {
    }

    FActiveCurveAnim(UCurveFloat &InCurve, const TFunction<void (float)> &InCallback, const float InRate = 1.0f)
    {
        Curve = InCurve;
        Callback = InCallback;
        PlaybackRate = InRate;
    }

    void Tick(const float DeltaTime)
    {
        CurrentTime += DeltaTime * PlaybackRate;

        const float CurrentValue = Curve->GetFloatValue(CurrentTime);

        Callback(CurrentValue);

        if (CurrentTime > MaxTime)
        {
            Curve = nullptr;
        }

    }

    bool IsFinished() const
    {
        return Curve == nullptr;
    }


};


UCLASS()
class PRISTSWITHGUNS_API UMyTweeningSubsystem : public UTickableWorldSubsystem
{
    GENERATED_BODY()

    virtual void Tick(float DeltaTime) override;

    TArray<FActiveCurveAnim> ActiveCurveAnims;

public:
    
    /*
    ==== Usage Example =====
    UMyTweeningSubsystem *MyTweeningSubsystem = GetWorld()->GetSubsystem<UMyTweeningSubsystem>();
    MyTweeningSubsystem->PlayCurveAnim(*ColorShiftCurve, 1.0, [&](float CurrentValue)
    {
    
    });
    ========================
    */
    void PlayCurveAnim(UCurveFloat &Curve, float PlaybackRate, TFunction<void(float)> Callback);


};