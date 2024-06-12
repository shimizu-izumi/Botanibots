// Copyright © 2024 MajorT. All rights reserved.


#include "Inventory/Data/InventoryConfig_Container.h"
#include "Inventory/Component/CommonInventoryComponent.h"
#include "Inventory/Component/CommonInventoryContainerComponent.h"

UInventoryConfig_Container::UInventoryConfig_Container()
{
	ContainerComponentClass = UCommonInventoryContainerComponent::StaticClass();
}

TSubclassOf<UCommonEquipmentBaseComponent> UInventoryConfig_Container::GetAdditionalComponentClass() const
{
	return ContainerComponentClass;
}

void UInventoryConfig_Container::PostInventoryInitialize(UCommonInventoryComponent* InventoryComponent, AActor* InOwnerActor, AActor* InAvatarActor)
{
	TSubclassOf<UCommonEquipmentBaseComponent> AdditionalComponentClass = GetAdditionalComponentClass();
	if (AdditionalComponentClass != nullptr)
	{
		UActorComponent* NewComponent = InOwnerActor->AddComponentByClass(AdditionalComponentClass, false, FTransform::Identity, false);
		UCommonInventoryContainerComponent* ContainerComponent = Cast<UCommonInventoryContainerComponent>(NewComponent);
		ContainerComponent->InitialContainerConfig = ContainerInitializer;
		InventoryComponent->AddChildComponent(ContainerComponent);
	}
}
