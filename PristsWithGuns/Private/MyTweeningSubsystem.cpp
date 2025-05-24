// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTweeningSubsystem.h"

void UMyTweeningSubsystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    for(int i = ActiveCurveAnims.Num(); i >= 0; i--)
    {
        ActiveCurveAnims[i].Tick(DeltaTime);

        if(ActiveCurveAnims[i].IsFinished())
        {
            ActiveCurveAnims.RemoveAtSwap(i);
        }
    }
}

void UMyTweeningSubsystem::PlayCurveAnim(UCurveFloat& InCurve, float InPlaybackRate, TFunction<void(float)> InCallback)
{
    ActiveCurveAnims.Add(FActiveCurveAnim(InCurve, InCallback, InPlaybackRate));
}