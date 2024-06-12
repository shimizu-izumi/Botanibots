// Copyright © 2024 MajorT. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConversionsFunctionLibrary.generated.h"

struct FCommonMath
{
	template<typename Value>
	static float FindBiggestValue(const TArray<Value>& InArray)
	{
		float OutValue = InArray[0];
		for (int32 i = 1; i < InArray.Num(); i++)
		{
			if (InArray[i] > OutValue)
			{
				OutValue = InArray[i];
			}
		}
		return OutValue;
	}
};

/**
 * 
 */
UCLASS()
class COMMONMATH_API UConversionsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	/**
	 * Gets the Gravitational Constant in m/s².
	 * @return The Gravitational Constant in m/s².
	 */
	UFUNCTION(BlueprintPure, Category = "Common Projectile|Gravity", meta = (DisplayName = "Get Gravity Constant in m/s²", CompactNodeTitle = "G in m/s²", Keywords = "gravity constant g m/s²"))
	static float GetGravityConstant() {return 9.81f;}

	/**
	 * Gets the Gravitational Constant in cm/s².
	 * @return The Gravitational Constant in cm/s².
	 */
	UFUNCTION(BlueprintPure, Category = "Common Projectile|Gravity", meta = (DisplayName = "Get Gravity Constant in cm/s²", CompactNodeTitle = "G in cm/s²", Keywords = "gravity constant g cm/s²"))
	static float GetGravityConstantInCmS() {return 981.0f;}

	/**
	 * Converts cm/s into km/h.
	 * @param CmS	Centimeters per second.
	 * @return Kilometers per hour.
	 */
	UFUNCTION(BlueprintPure, Category = "Common Math|Conversions", meta = (DisplayName = "Convert cm/s to km/h", CompactNodeTitle = "cm/s to km/h", Keywords = "convert cm/s km/h"))
	static float ConvertCmSToKmH(float CmS) {return CmS * 0.036f;}

	/**
	 * Converts km/h into cm/s.
	 * @param KmH	Kilometers per hour.
	 * @return Centimeters per second.
	 */
	UFUNCTION(BlueprintPure, Category = "Common Math|Conversions", meta = (DisplayName = "Convert km/h to cm/s", CompactNodeTitle = "km/h to cm/s", Keywords = "convert km/h cm/s"))
	static float ConvertKmHToCmS(float KmH) {return KmH * 27.7778f;}

	/**
	 * Finds the greatest value in an array of floats.
	 * @param InArray The array to search.
	 * @return The greatest value in the array.
	 */
	UFUNCTION(BlueprintPure, Category = "CommonMath|Array", meta = (DisplayName = "Get Biggest Value (float)", Keywords = "biggest value float"))
	static float GetBiggestValueFloat(const TArray<float>& InArray) {return FCommonMath::FindBiggestValue<float>(InArray); }

	/**
	 * Finds the greatest value in an array of integers.
	 * @param InArray The array to search.
	 * @return The greatest value in the array.
	 */
	UFUNCTION(BlueprintPure, Category = "CommonMath|Array", meta = (DisplayName = "Get Biggest Value (int)", Keywords = "biggest value int"))
	static int32 GetBiggestValueInt(const TArray<int32>& InArray) {return FCommonMath::FindBiggestValue<int32>(InArray); }

	/**
	 * Returns the maximum value for an integer.
	 * In C++ this is 2147483647 (2024).
	 */
	UFUNCTION(BlueprintPure, Category = "CommonMath|Int", meta = (DisplayName = "INT MAX", CompactNodeTitle = "INT MAX"))
	static int32 GetIntMax() {return INT_MAX;}

	/**
	 * Returns the minimum value for an integer.
	 * In C++ this is -2147483648 (2024).
	 */
	UFUNCTION(BlueprintPure, Category = "CommonMath|Int", meta = (DisplayName = "INT MIN", CompactNodeTitle = "INT MIN"))
	static int32 GetIntMin() {return INT_MIN;}

	/**
	 * Returns Index None.
	 * In C++ this is -1 (2024).
	 */
	UFUNCTION(BlueprintPure, Category = "CommonMath|Int", meta = (DisplayName = "INDEX NONE", CompactNodeTitle = "INDEX NONE"))
	static int32 GetIndexNone() {return INDEX_NONE;}
};
