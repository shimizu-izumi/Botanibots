// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AssetFactory_BTDecorator.h"

#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "Kismet2/KismetEditorUtilities.h"

UAssetFactory_BTDecorator::UAssetFactory_BTDecorator()
{
	SupportedClass = UBTDecorator_BlueprintBase::StaticClass();
	ParentClass = UBTDecorator_BlueprintBase::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;

#if ENGINE_MAJOR_VERSION == 5
	bSkipClassPicker = true;
#endif
}

UObject* UAssetFactory_BTDecorator::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return Super::FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn);
}

UObject* UAssetFactory_BTDecorator::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}

FString UAssetFactory_BTDecorator::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewBTDecorator_BlueprintBase"));
}
