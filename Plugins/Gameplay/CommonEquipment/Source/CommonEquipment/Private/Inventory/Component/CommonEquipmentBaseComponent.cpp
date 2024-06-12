// Copyright © 2024 MajorT. All rights reserved.


#include "Inventory/Component/CommonEquipmentBaseComponent.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonEquipmentBaseComponent)

UCommonEquipmentBaseComponent::UCommonEquipmentBaseComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UCommonEquipmentBaseComponent::InitInventory(AActor* InOwnerActor, AActor* InAvatarActor)
{
	OwnerActor = InOwnerActor;
	AvatarActor = InAvatarActor;

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, OwnerActor, this);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, AvatarActor, this);
}

void UCommonEquipmentBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, OwnerActor, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AvatarActor, Params);
}
