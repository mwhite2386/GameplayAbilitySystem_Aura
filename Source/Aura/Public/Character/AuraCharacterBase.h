// TechStartInnovations

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

//Abstracts prevents this class from being dropped in Unreal Engine
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
protected:

	virtual void BeginPlay() override;
};
