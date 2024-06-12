// Copyright © 2024 Botanibots Team. All rights reserved.


#include "Pickup/ProxyActor/CommonPickupProxy.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameplayTagStacks/Components/TagStackComponent.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonPickupProxy)

ACommonPickupProxy::ACommonPickupProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	/* Network optimizations */
	bReplicates = true;
	bAlwaysRelevant = false;
	NetCullDistanceSquared = 2560000.f;
	NetUpdateFrequency = 2.f;

	/* Cosmetics */
	ElementColor = FLinearColor::White;

	/* Setup the hitbox to work properly for the pickup */
	PickupHitboxComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Hitbox"));
	if (PickupHitboxComponent)
	{
		SetRootComponent(PickupHitboxComponent);
		PickupHitboxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PickupHitboxComponent->SetGenerateOverlapEvents(true);
		PickupHitboxComponent->SetSphereRadius(50.0f);
		PickupHitboxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		PickupHitboxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		PickupHitboxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		PickupHitboxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		PickupHitboxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}

	/* Setup the pickup display mesh to be very lightweight */
	PickupDisplayMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Display Mesh"));
	if (PickupDisplayMeshComponent)
	{
		PickupDisplayMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickupDisplayMeshComponent->SetGenerateOverlapEvents(false);
		PickupDisplayMeshComponent->SetCastShadow(false);
		PickupDisplayMeshComponent->SetVisibility(true);
		PickupDisplayMeshComponent->SetupAttachment(PickupHitboxComponent);
		PickupDisplayMeshComponent->SetAffectDistanceFieldLighting(false);
		PickupDisplayMeshComponent->SetAffectDynamicIndirectLighting(false);
		PickupDisplayMeshComponent->SetOverlayMaterialMaxDrawDistance(8000.f);
	}

	/* Setup the prompt widget component */
	PromptWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Prompt Widget Component"));
	if (PromptWidgetComponent)
	{
		PromptWidgetComponent->SetVisibility(false);
		PromptWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PromptWidgetComponent->SetGenerateOverlapEvents(false);
		PromptWidgetComponent->SetupAttachment(PickupHitboxComponent);
		PromptWidgetComponent->SetDrawAtDesiredSize(true);
		PromptWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}

	/* Setup the tag stack component */
	TagStackComponent = CreateDefaultSubobject<UTagStackComponent>(TEXT("Tag Stack Component"));
}

void ACommonPickupProxy::TouchInteractable(AActor* InteractingActor)
{
	K2_TouchInteractable(InteractingActor);

	APawn* InteractingPawn = Cast<APawn>(InteractingActor);
	if (!InteractingPawn)
	{
		return;
	}

	if (InteractingPawn->IsLocallyControlled())
	{
		ShowInteractionPrompt(true);
	}
}

void ACommonPickupProxy::LeaveInteractable(AActor* InteractingActor)
{
	K2_LeaveInteractable(InteractingActor);

	APawn* InteractingPawn = Cast<APawn>(InteractingActor);
	if (!InteractingPawn)
	{
		return;
	}

	if (InteractingPawn->IsLocallyControlled())
	{
		ShowInteractionPrompt(false);
	}
}

UCommonItemDefinition* ACommonPickupProxy::GetItemDefinition() const
{
	return CachedItemDefinition;
}

void ACommonPickupProxy::OnPickupDropped(UCommonItemDefinition* ItemDefinition)
{
	SetupPickupProxy(ItemDefinition);
	K2_OnPickupDropped(ItemDefinition);
}

#pragma region blueprint_functions
bool ACommonPickupProxy::CanInteract(AActor* InteractingActor) const
{
	return K2_CanInteract(InteractingActor);
}

bool ACommonPickupProxy::CommitInteraction(AActor* InteractingActor)
{
	K2_CommitInteraction(InteractingActor);
	return true;
}

bool ACommonPickupProxy::K2_CanInteract_Implementation(AActor* InteractingActor) const
{
	return true;
}
#pragma endregion

void ACommonPickupProxy::BeginPlay()
{
	Super::BeginPlay();

	/* Setup the pickup proxy if it should */
	if (CachedItemDefinition && CachedItemDefinition->bIsWorldItem)
	{
		SetupPickupProxy(CachedItemDefinition);
	}
}

void ACommonPickupProxy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	/* Unload all objects from system memory */
	UnloadObjects();
}

void ACommonPickupProxy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	/* Handles the pickup proxy in the editor */
	if (CachedItemDefinition && CachedItemDefinition->bIsWorldItem)
	{
		SetupPickupProxy(CachedItemDefinition);
	}
	else
	{
		ResetPickupProxy();
	}
}

void ACommonPickupProxy::SetupPickupProxy(UCommonItemDefinition* NewItemDefinition)
{
	/* Check that we have a valid item definition */
	if (!NewItemDefinition)
	{
		return;
	}

	/* Update the item definition */
	SetItemDefinition(NewItemDefinition);

	/* Check that the item supports being placed in world */
	if (!CachedItemDefinition->bIsWorldItem)
	{
		return;
	}

	/* Load the display mesh into system memory and display it */
	if (!CachedItemDefinition->PickupData.PickupDisplayMesh)
	{
		TArray<FSoftObjectPath> AssetsToLoad;
		AssetsToLoad.AddUnique(CachedItemDefinition->PickupData.PickupDisplayMesh.ToSoftObjectPath());
		AssetLoader.RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &ThisClass::OnDisplayMeshLoaded));
	}
	else
	{
		/* if its already loaded, just display it */
		OnDisplayMeshLoaded();
	}

	/* Update the hitbox */
	PickupHitboxComponent->SetSphereRadius(CachedItemDefinition->PickupData.PickupCollisionRadius);
}

void ACommonPickupProxy::ResetPickupProxy()
{
	// Clear the item cosmetics
	PickupDisplayMeshComponent->SetStaticMesh(nullptr);
	PickupDisplayMeshComponent->SetRelativeTransform(FTransform::Identity);

	PickupHitboxComponent->SetSphereRadius(50.0f);
	
	// Unload all objects from system memory
	SetItemDefinition(nullptr);
	UnloadObjects();
}

void ACommonPickupProxy::OnDisplayMeshLoaded()
{
	/* Setup the display mesh */
	if (CachedItemDefinition && CachedItemDefinition->PickupData.PickupDisplayMesh)
	{
		PickupDisplayMeshComponent->SetStaticMesh(CachedItemDefinition->PickupData.PickupDisplayMesh.Get());
		PickupDisplayMeshComponent->SetRelativeTransform(CachedItemDefinition->PickupData.PickupRelativeTransform);
	}
}

void ACommonPickupProxy::SetItemDefinition(UCommonItemDefinition* NewItemDefinition)
{
	/* Check if we have a valid new item definition */
	if (NewItemDefinition == CachedItemDefinition || !NewItemDefinition)
	{
		return;
	}

	CachedItemDefinition = NewItemDefinition;
	OnRep_ItemDefinition();
}

void ACommonPickupProxy::ShowInteractionPrompt(const bool bShow)
{
	PromptWidgetComponent->SetHiddenInGame(!bShow);
	PromptWidgetComponent->SetVisibility(bShow, true);
}

void ACommonPickupProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ThisClass, CachedItemDefinition);
}

void ACommonPickupProxy::OnRep_ItemDefinition()
{
}

void ACommonPickupProxy::UnloadObjects()
{
	for (int Index = 0; Index < LoadedObjects.Num(); ++Index)
	{
		const FSoftObjectPath& Path = LoadedObjects[Index];
		AssetLoader.Unload(Path);
	}

	LoadedObjects.Empty();
}
