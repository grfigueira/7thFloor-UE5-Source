// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BookViewer.h"
#include "PuzzleGrid.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FMODBlueprintStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PristsWithGunsCharacter.generated.h"

class UCrosshairWidget;
class UWidgetAnimation;
class IInteractable;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APristsWithGunsCharacter : public ACharacter
{
    GENERATED_BODY()

    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent *Mesh1P;

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent *FirstPersonCameraComponent;

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext *DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext *PuzzleGridMappingContext;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction *JumpAction;

    /** Move Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction *MoveAction;

    /** Look Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction *LookAction;

    ///////////////////
    /// Observation ///
    ///////////////////

    UFUNCTION()
    void NextObservation();

    //////////////
    /// Puzzle ///
    //////////////

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction *InteractAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Puzzle", meta = (AllowPrivateAccess = "true"))
    UInputAction *PuzzleMoveAction;

    // null if no interactable is in range
    TWeakObjectPtr<AActor> InteractableActorInRange;

    TScriptInterface<IInteractable> InteractableInRange;

public:
    APristsWithGunsCharacter();

    virtual void Tick(float DeltaTime) override;

    void AlertInteractableInRange(AActor *Interactable, bool bInRange)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player: Interactable in range"));
        }
        InteractableActorInRange = bInRange ? Interactable : nullptr;
    }

    UFUNCTION(BlueprintCallable)
    void ExitGridPuzzle();

    void EnterGridPuzzle(APuzzleGrid *PuzzleGrid);

protected:
    /** Called for movement input */
    void Move(const FInputActionValue &Value);

    /** Called for looking input */
    void Look(const FInputActionValue &Value);

    UFUNCTION()
    void CheckForInteractableInRange();

    UPROPERTY(EditAnywhere, Category = "Interact")
    float InteractRange = 50.0f;

    UPROPERTY(EditAnywhere, Category = Input)
    float LookSensitivity = 1.0;

    UPROPERTY(EditAnywhere, Category = Input)
    float CameraTransitionTime = 0.2;

    UPROPERTY(EditAnywhere, Category = Movement)
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> CrosshairWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> BookViewerWidgetClass;

    UPROPERTY(EditAnywhere, Category = Movement)
    TSubclassOf<UCameraShakeBase> CameraShake;

    //////// Character Sounds //////////

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
    UFMODAudioComponent *FmodAudioComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound")
    TMap<UPhysicalMaterial *, float> PhysicalMaterialMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    bool bPrintDebug;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    float Offset = -150;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    float FootstepFrequencyAdjuster = 10.0f;

    UFUNCTION(BlueprintCallable, Category="Sound")
    UPhysicalMaterial *GetPhysicalMaterialByLineTrace(const float &OffsetZ, const bool &bDebug);

    UFUNCTION(BlueprintCallable, Category="Sound")
    void SetFootstepsParameter(const UPhysicalMaterial *HitPhysicalMaterial, const bool &bDebug,
                               const FName ParameterName);

    ///////////////////////////////////////

    void MovePuzzle(const FInputActionValue &Value);
    void Interact();
    bool bPuzzleCanMove;

    // APawn interface
    virtual void NotifyControllerChanged() override;
    void ResetPuzzleCanMove(const FInputActionValue &InputActionValue);

    virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;
    virtual void BeginPlay() override;
    // End of APawn interface

public:
    /** Returns Mesh1P subobject **/
    USkeletalMeshComponent *GetMesh1P() const { return Mesh1P; }
    /** Returns FirstPersonCameraComponent subobject **/
    UCameraComponent *GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
    
    TUniquePtr<UBookViewer> GetBookWidgetOwnership();

    bool bIsInteracting;

    UPROPERTY()
    UCrosshairWidget *CrosshairWidget;

private:
    float FootstepTimeAccumulator;
    TUniquePtr<UBookViewer> BookWidget;
    bool bIsMoving;
    bool bShowingCrosshair;
    void SetMovingTrue();
    void SetMovingFalse() { bIsMoving = false; }
    void HideCrosshair();
    UActorComponent *HasInteractableComponent(AActor *Actor);
};