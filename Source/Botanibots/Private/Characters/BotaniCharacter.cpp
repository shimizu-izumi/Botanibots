// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Characters/BotaniCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/Component/CommonInventoryComponent.h"
#include "Inventory/Component/CommonInventoryContainerComponent.h"
#include "Inventory/Component/CommonEquipmentComponent.h"

ABotaniCharacter::ABotaniCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	InventoryComponent = CreateDefaultSubobject<UCommonInventoryComponent>(TEXT("InventoryComponent"));
}

UAbilitySystemComponent* ABotaniCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


USkeletalMeshComponent* ABotaniCharacter::GetFirstPersonMesh_Implementation() const
{
	return IBotaniCombatInterface::GetFirstPersonMesh_Implementation();
}

USkeletalMeshComponent* ABotaniCharacter::GetThirdPersonMesh_Implementation() const
{
	return GetMesh();
}

UCommonEquipmentComponent* ABotaniCharacter::GetEquipmentComponent() const
{
	return InventoryComponent->GetInventoryChildComponent<UCommonEquipmentComponent>();
}

UCommonInventoryComponent* ABotaniCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UCommonInventoryContainerComponent* ABotaniCharacter::GetInventoryContainerComponent() const
{
	return InventoryComponent->GetInventoryChildComponent<UCommonInventoryContainerComponent>();
}

void ABotaniCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABotaniCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABotaniCharacter::InitAbilityActorInfo()
{
	InventoryComponent->InitInventory(this, this);
}

void ABotaniCharacter::InitStartupAbilitySet()
{
}

