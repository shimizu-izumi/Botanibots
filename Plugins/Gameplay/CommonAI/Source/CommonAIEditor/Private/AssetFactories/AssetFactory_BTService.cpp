// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AssetFactory_BTService.h"

#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "Kismet2/KismetEditorUtilities.h"

UAssetFactory_BTService::UAssetFactory_BTService()
{
	SupportedClass = UBTService_BlueprintBase::StaticClass();
	ParentClass = UBTService_BlueprintBase::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;

#if ENGINE_MAJOR_VERSION == 5
	bSkipClassPicker = true;
#endif
}

UObject* UAssetFactory_BTService::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return Super::FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn);
}

UObject* UAssetFactory_BTService::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}

FString UAssetFactory_BTService::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewBTService_BlueprintBase"));
}
