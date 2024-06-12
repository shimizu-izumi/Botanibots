// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Inventory/Item/CommonItem.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonItem)

ACommonItem::ACommonItem()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bAlwaysRelevant = false;
	NetUpdateFrequency = 10.f;
	NetCullDistanceSquared = 225000000.f;
}

void ACommonItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, OwningPawn, SharedParams);
}

void ACommonItem::SetOwningPawn(APawn* NewOwner)
{
	APawn* OldOwner = OwningPawn;
	OwningPawn = NewOwner;
	
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, OwningPawn, this);
	
	OnRep_OwningPawn(OldOwner);
	OldOwner = nullptr;
}

APawn* ACommonItem::GetOwningPawn() const
{
	return OwningPawn;
}

void ACommonItem::OnEquipped(APawn* NewOwner)
{
	SetOwningPawn(NewOwner);
}

void ACommonItem::OnUnequipped()
{
	SetOwningPawn(nullptr);
}


void ACommonItem::OnRep_OwningPawn(APawn* PreviousOwner)
{
	if (OwningPawn)
	{
		K2_OnEquipped(OwningPawn);
	}
	else
	{
		K2_OnUnequipped(PreviousOwner);
	}
}
