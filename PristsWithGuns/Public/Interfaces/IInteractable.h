#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{

    GENERATED_BODY()
    
};

class PRISTSWITHGUNS_API IInteractable
{

    GENERATED_BODY()

public:
    
    virtual void Interact(class ACharacter* Character) = 0;
    
    virtual FText GetInteractionText() const = 0;
    
};
