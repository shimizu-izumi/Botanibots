// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "AssetFactory_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class COMMONAIEDITOR_API UAssetFactory_BTTask : public UBlueprintFactory
{
	GENERATED_BODY()

public:
	UAssetFactory_BTTask();

protected:
	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual FString GetDefaultNewAssetName() const override;
	//~ End UFactory Interface
};
