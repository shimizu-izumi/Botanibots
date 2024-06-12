// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/BotaniCombatInterface.h"
#include "Interfaces/EquipmentInterface.h"
#include "BotaniCharacter.generated.h"

class UCommonInventoryItemDefinition;
class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS()
class BOTANIBOTS_API ABotaniCharacter : public ACharacter, public IAbilitySystemInterface, public IBotaniCombatInterface, public IEquipmentInterface
{
	GENERATED_BODY()

public:
	ABotaniCharacter();

	//~Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	//~End IAbilitySystemInterface

	//~Begin IBotaniCombatInterface
	virtual USkeletalMeshComponent* GetFirstPersonMesh_Implementation() const override;
	virtual USkeletalMeshComponent* GetThirdPersonMesh_Implementation() const override;
	//~End IBotaniCombatInterface

	//~ Begin IEquipmentInterface
	virtual UCommonEquipmentComponent* GetEquipmentComponent() const override;
	virtual UCommonInventoryComponent* GetInventoryComponent() const override;
	virtual UCommonInventoryContainerComponent* GetInventoryContainerComponent() const override;
	//~ End IEquipmentInterface

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	
	/** Ability System */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/** Inventory Component */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	TObjectPtr<UCommonInventoryComponent> InventoryComponent;

	virtual void InitAbilityActorInfo();
	virtual void InitStartupAbilitySet();

};
