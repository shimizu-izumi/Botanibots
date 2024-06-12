// Copyright © 2024 Botanibots Team. All rights reserved.


#include "AssetFactory_EQSTestingPawn.h"

#include "EnvironmentQuery/EQSTestingPawn.h"
#include "Kismet2/KismetEditorUtilities.h"

UAssetFactory_EQSTestingPawn::UAssetFactory_EQSTestingPawn()
{
	SupportedClass = AEQSTestingPawn::StaticClass();
	ParentClass = AEQSTestingPawn::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;

#if ENGINE_MAJOR_VERSION == 5
	bSkipClassPicker = true;
#endif
}

UObject* UAssetFactory_EQSTestingPawn::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name,
                                                        EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return Super::FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn);
}

UObject* UAssetFactory_EQSTestingPawn::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, InName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
}

FString UAssetFactory_EQSTestingPawn::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewEQSTestingPawn"));
}
