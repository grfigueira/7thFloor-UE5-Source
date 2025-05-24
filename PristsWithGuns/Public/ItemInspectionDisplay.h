// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInspectionDisplay.generated.h"

class USpotLightComponent;

UCLASS()
class PRISTSWITHGUNS_API AItemInspectionDisplay : public AActor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Item Display")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(EditAnywhere, Category = "Item Display")
    TObjectPtr<USceneComponent> Pivot;

    UPROPERTY(EditAnywhere, Category = "Item Display")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditAnywhere, Category = "Item Display")
    TObjectPtr<USceneCaptureComponent2D> SceneCapture;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USpotLightComponent> Spot_Light;

    UPROPERTY(EditAnywhere, Category = "Item Display|Rotation Settings")
    float RotationSpeed = 1.4f;
    
    UPROPERTY(EditAnywhere, Category = "Item Display|Rotation Settings", meta = (UIMin = "0.0", UIMax = "1.0", ClampMin = "0.0", ClampMax = "1.0"))
    float YLimit = 0.99f;
    
    bool bIsRotating;
    FVector2D LastMousePos;

public:
    AItemInspectionDisplay();

    UFUNCTION(BlueprintCallable)
    void SetIsRotating(const bool bValue);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;


};