// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AssetFactory_BTTask.h"

#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Kismet2/KismetEditorUtilities.h"

UAssetFactory_BTTask::UAssetFactory_BTTask()
{
	SupportedClass = UBTTask_BlueprintBase::StaticClass();
	ParentClass = UBTTask_BlueprintBase::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;

#if ENGINE_MAJOR_VERSION == 5
	bSkipClassPicker = true;
#endif
}

UObject* UAssetFactory_BTTask::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return Super::FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn);
}

UObject* UAssetFactory_BTTask::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}

FString UAssetFactory_BTTask::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewBTTask_BlueprintBase"));
}
