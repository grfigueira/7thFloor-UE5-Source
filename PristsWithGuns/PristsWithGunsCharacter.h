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

class UInventoryWidget;
class FInventory;
class UPauseMenu;
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

public:
    APristsWithGunsCharacter();

    virtual void Tick(float DeltaTime) override;

    /** Returns Mesh1P subobject **/
    USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
    
    /** Returns FirstPersonCameraComponent subobject **/
    UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
    virtual void BeginPlay() override;
    virtual void NotifyControllerChanged() override;

private:
    // ==========================================
    // Core Character Components
    // ==========================================
    
    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* Mesh1P;

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;

    // ==========================================
    // Input Mapping Contexts
    // ==========================================

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* PuzzleGridMappingContext;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* BookMappingContext;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* InventoryMappingContext;

    // ==========================================
    // Basic Character Input Actions
    // ==========================================

    /** Move Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    /** Look Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* LookAction;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* PauseGameAction;
    
    // ==========================================
    // Basic Character Input Handlers
    // ==========================================
    
    /** Called for movement input */
    void Move(const FInputActionValue& Value);

    /** Called for looking input */
    void Look(const FInputActionValue& Value);

public: 
    UFUNCTION()
    void PauseGame();

    UFUNCTION(BlueprintCallable)
    void UnpauseGame();
    
private:
    // ==========================================
    // Character Settings
    // ==========================================

    UPROPERTY(EditAnywhere, Category = Input)
    float LookSensitivity = 1.0;

    UPROPERTY(EditAnywhere, Category = Input)
    float CameraTransitionTime = 0.2;

    UPROPERTY(EditAnywhere, Category = Movement)
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, Category = Movement)
    TSubclassOf<UCameraShakeBase> CameraShake;

    // ==========================================
    // Interaction System
    // ==========================================

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* InteractAction;

    UPROPERTY(EditAnywhere, Category = "Interact")
    float InteractRange = 50.0f;

    // null if no interactable is in range
    TWeakObjectPtr<AActor> InteractableActorInRange;
    TScriptInterface<IInteractable> InteractableInRange;

    public:
    void AlertInteractableInRange(AActor* Interactable, bool bInRange)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player: Interactable in range"));
        }
        InteractableActorInRange = bInRange ? Interactable : nullptr;
    }

    bool bIsInteracting;

    protected:
    void Interact();
    
    UFUNCTION()
    void CheckForInteractableInRange();
    
    UActorComponent* HasInteractableComponent(AActor* Actor);

    // ==========================================
    // Grid Puzzle System
    // ==========================================

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Puzzle", meta = (AllowPrivateAccess = "true"))
    UInputAction* PuzzleMoveAction;

    protected:
    void MovePuzzle(const FInputActionValue& Value);
    void ResetPuzzleCanMove(const FInputActionValue& InputActionValue);
    
    public:
    UFUNCTION(BlueprintCallable)
    void ExitGridPuzzle();

    void EnterGridPuzzle(APuzzleGrid* PuzzleGrid);

    private:
    bool bPuzzleCanMove;

    // ==========================================
    // Book Viewing System
    // ==========================================
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* BookLeftSwipe;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* BookRightSwipe;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* CloseBook;

    // BookWidget is basically a unique pointer, but unreal doesn't like TUniquePtr with UObjects...
    // So we simulate it with the methods `TryAcquireBookWidgetOwnership` and `ReleaseBookWidget`
    UPROPERTY()
    TObjectPtr<UBookViewer> BookWidget;

    bool bIsBookBeingUsed;
    bool bCheckBookInput;

    protected:
    void BookPrevPage();
    void BookNextPage();

    public:
    void EnterBookContext();
    void ExitBookContext();
    bool TryAcquireBookWidgetOwnership(TObjectPtr<UBookViewer>& OutBookViewer);
    void ReleaseBookWidget();

    // ==========================================
    // UI Components
    // ==========================================

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> CrosshairWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> BookViewerWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> PauseMenuWidgetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> InventoryWidgetClass;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UPauseMenu> PauseMenuWidget;

    UPROPERTY()
    UCrosshairWidget* CrosshairWidget;

    private:
    bool bShowingCrosshair;
    void HideCrosshair();

    // ==========================================
    // Observation System
    // ==========================================

    UFUNCTION()
    void NextObservation();
    
    // ==========================================
    // Inventory system
    // ==========================================

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* OpenInventoryAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* CloseInventoryAction;
    
    TObjectPtr<FInventory> Inventory;
    
    UPROPERTY()
    TObjectPtr<UInventoryWidget> InventoryWidget;

    UFUNCTION()
    void OpenInventory();
    void CloseInventory();

    // ==========================================
    // Movement State Tracking
    // ==========================================

    private:
    bool bIsMoving;
    void StartedMoving(const FInputActionValue& Value);
    void SetMovingFalse() { bIsMoving = false; bCheckBookInput = false; }

    // ==========================================
    // Footstep and Sound System
    // ==========================================

    public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
    UFMODAudioComponent* FmodAudioComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound")
    TMap<UPhysicalMaterial*, float> PhysicalMaterialMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    bool bPrintDebug;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    float Offset = -150;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    float FootstepFrequencyAdjuster = 10.0f;

    private:
    float FootstepTimeAccumulator;

    protected:
    UFUNCTION(BlueprintCallable, Category="Sound")
    UPhysicalMaterial* GetPhysicalMaterialByLineTrace(const float& OffsetZ, const bool& bDebug);

    UFUNCTION(BlueprintCallable, Category="Sound")
    void SetFootstepsParameter(const UPhysicalMaterial* HitPhysicalMaterial, const bool& bDebug,
                              const FName ParameterName);
};