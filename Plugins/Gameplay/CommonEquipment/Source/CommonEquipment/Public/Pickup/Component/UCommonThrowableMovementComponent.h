// Copyright © 2024 Botanibots Team. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "UCommonThrowableMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementStopSignature, const FHitResult&, ImpactResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnProjectileBounceSignature, const FHitResult&, ImpactResult, const FVector&, ImpactVelocity );

/**
 * PickupMovementComponent updates the position of another component during its tick.
 * Behavior such as bouncing and sliding can be enabled.
 *
 * By default, the component will not move unless the Velocity is set.
 * The root component of the owning actor will be moved, unless another component is specified.
 * 
 * @see UMovementComponent
 */
UCLASS(ClassGroup=("Pickup"), meta=(BlueprintSpawnableComponent))
class COMMONEQUIPMENT_API UCommonThrowableMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	UCommonThrowableMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UMovementComponent Interface
	virtual float GetMaxSpeed() const override { return 0.f; }
	virtual void InitializeComponent() override;
	virtual void UpdateTickRegistration() override;
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) override;
	//~End UMovementComponent Interface

	//~Begin UActorComponent Interface
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~End UActorComponent Interface

	virtual bool CheckStillInWorld();

#pragma region blueprint_exposed
	/**
	 * Moves the UpdatedComponent, which is also the interpolation target for the interpolated component. If there is not interpolated component, this simply moves UpdatedComponent.
	 * Use this typically from PostNetReceiveLocationAndRotation() or similar from an Actor.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Components|ProjectileMovement|Interpolation")
	virtual void MoveInterpolationTarget(const FVector& NewLocation, const FRotator& NewRotation);
	
	/**
	 * Assigns the component that will be used for network interpolation/smoothing. It is expected that this is a component attached somewhere below the UpdatedComponent.
	 * When network updates use MoveInterpolationTarget() to move the UpdatedComponent, the interpolated component's relative offset will be maintained and smoothed over
	 * the course of future component ticks. The current relative location and rotation of the component is saved as the target offset for future interpolation.
	 * @see MoveInterpolationTarget(), bInterpMovement, bInterpRotation
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Components|ProjectileMovement|Interpolation")
	virtual void SetInterpolatedComponent(USceneComponent* Component);

	/**
	 * Resets interpolation so that interpolated component snaps back to the initial location/rotation without any additional offsets.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Components|ProjectileMovement|Interpolation")
	virtual void ResetInterpolation();
	
	/**
	 * Returns true if velocity magnitude is less than BounceVelocityStopSimulatingThreshold.
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Components|ProjectileMovement")
	bool IsVelocityUnderSimulationThreshold() const;

	/**
	 * Set the velocity of the component in world space.
	 * @param NewVelocity - The new velocity in world space.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Components|PickupMovement")
	virtual void SetVelocityInLocalSpace(FVector NewVelocity);

	/**
	 * Clear the velocity of the component.
	 * Fires the stop event, and stops ticking
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Components|PickupMovement")
	virtual void StopSimulating(const FHitResult& HitResult);

	UPROPERTY(BlueprintAssignable)
	FOnMovementStopSignature OnMovementStop;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileBounceSignature OnProjectileBounce;

#pragma endregion

#pragma region properties
	// Drop Velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	float MovementSpeed;

	// Drop Velocity in local space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	uint8 bInitialVelocityInLocalSpace:1;

	// Should Bounce
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bouncing")
	uint8 bShouldBounce:1;

	// Bounciness
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bouncing", meta=(ClampMin="0", UIMin="0", EditCondition="bShouldBounce"))
	float Bounciness;

	// Max Bounces
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bouncing", meta=(ClampMin="0", UIMin="0", EditCondition="bShouldBounce"))
	uint8 MaxBounces;

	// Sliding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding")
	uint8 bShouldSlide:1;

	// Sliding
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Sliding")
	uint8 bIsSliding:1;

	// Slide Friction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding")
	float SlideFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding", AdvancedDisplay, meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float MinFrictionFraction;

	// Gravity Scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GravityScale;

	// Saved HitResult Time (0 to 1) from previous simulation step. Equal to 1.0 when there was no impact.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Bouncing")
	float PreviousHitTime;

	// Saved HitResult Normal from previous simulation step that resulted in an impact. If PreviousHitTime is 1.0, then the hit was not in the last step
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Bouncing")
	FVector PreviousHitNormal;

	// Should the pickup home in on a target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Homing")
	uint8 bIsHoming:1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Homing")
	TWeakObjectPtr<USceneComponent> HomingTargetComponent;

	/** The magnitude of our acceleration towards the homing target. Overall velocity magnitude will still be limited by MaxSpeed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Homing")
	float HomingAccelerationMagnitude;

	/*
	 Movement Threshold
	 If the movement is less than this value, the movement will stop simulating.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	uint8 bSimulationEnabled:1;

	// Movement Interpolation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpolation")
	uint8 bInterpolateMovement:1;

	// Interpolation Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpolation", meta=(EditCondition="bInterpolateMovement"))
	float InterpolationSpeed;

	uint8 bInterpolationUseScopedMovement:1;
#pragma endregion

protected:

public:
	USceneComponent* GetInterpolatedComponent() const { return InterpolatedComponentPtr.Get(); }
	bool HasStoppedSimulation() const { return (UpdatedComponent == nullptr) || (IsActive() == false); }

#pragma region interpolation
protected:
	/**
	 * Custom hook to reset throttle interpolation tracking when it was throttling previously.
	 */
	virtual void ResetThrottleInterpolation(float DeltaTime);
	
	virtual void TickInterpolation(float DeltaTime);
	bool UpdateThrottleInterpolation(float DeltaTime, USceneComponent* InterpComponent);

	/**
	* Determine the number of frames to allow to skip when interpolating. Returning 0 means not to allow throttling.
	* Default implementation chooses between ThrottleInterpolationThresholdNotRenderedShortTime and ThrottleInterpolationThresholdNotRenderedLongTime based on WasRecentlyRendered(),
	* extend or override this for custom behavior.
	*/
	virtual int32 ComputeThrottleInterpolationMaxFrames(float DeltaTime, USceneComponent* InterpComponent);
	
	/**
	 * True if interpolation has reached the target, false if there is more interpolation required.
	 */
	uint8 bInterpolationComplete:1;

	/**
	 * Tracks the number of frames since the last interpolation.
	 */
	int32 ThrottleInterpolationFramesSinceInterp;

	TWeakObjectPtr<USceneComponent> InterpolatedComponentPtr;
	FVector InterpLocationOffset;
	FVector InterpInitialLocationOffset;
	FQuat InterpRotationOffset;
	FQuat InterpInitialRotationOffset;

#pragma endregion

#pragma region substepping
public:
	/**
	 * Determine whether or not to use substepping in the projectile motion update.
	 * If true, GetSimulationTimeStep() will be used to time-slice the update. If false, all remaining time will be used during the tick.
	 * @return Whether or not to use substepping in the projectile motion update.
	 * @see GetSimulationTimeStep()
	 */
	virtual bool ShouldUseSubStepping() const;

	
	/**
	 * Compute remaining time step given remaining time and current iterations.
	 * The last iteration (limited by MaxSimulationIterations) always returns the remaining time, which may violate MaxSimulationTimeStep.
	 *
	 * @param RemainingTime		Remaining time in the tick.
	 * @param Iterations		Current iteration of the tick (starting at 1).
	 * @return The remaining time step to use for the next sub-step of iteration.
	 * @see MaxSimulationTimeStep, MaxSimulationIterations
	 * @see ShouldUseSubStepping()
	 */
	float GetSimulationTimeStep(float RemainingTime, int32 Iterations) const;
#pragma endregion
	
#pragma region simulation
public:
	void AddForce(FVector Force);
	void ClearPendingForce(bool bClearImmediateForce = false);
	
	/**
	 * Handle case where projectile is sliding along a surface.
	 * Velocity will be parallel to the impact surface upon entry to this method.
	 * 
	 * @param  Hit				Hit result of impact causing slide. May be modified by this function to reflect any subsequent movement.
	 * @param  SubTickTimeRemaining		Time remaining in the tick. This function may update this time with any reduction to the simulation time requested.
	 * @return True if simulation of the projectile should continue, false otherwise.
	 */
	virtual bool HandleSliding(FHitResult& Hit, float& SubTickTimeRemaining);
	
	/**
	 * Handle a blocking hit after HandleBlockingHit() returns a result indicating that deflection occured.
	 * Default implementation increments NumBounces, checks conditions that could indicate a slide, and calls HandleSliding() if necessary.
	 * 
	 * @param  Hit					Blocking hit that occurred. May be changed to indicate the last hit result of further movement.
	 * @param  OldVelocity			Velocity at the start of the simulation update sub-step. Current Velocity may differ (as a result of a bounce).
	 * @param  NumBounces			Number of bounces that have occurred thus far in the tick.
	 * @param  SubTickTimeRemaining	Time remaining in the simulation sub-step. May be changed to indicate change to remaining time.
	 * @return True if simulation of the projectile should continue, false otherwise.
	 * @see HandleSliding()
	 */
	virtual bool HandleDeflection(FHitResult& Hit, const FVector& OldVelocity, const uint32 NumBounces, float& SubTickTimeRemaining);
	
	enum class EHandleBlockingHitResult
	{
		Deflect,				/** Assume velocity has been deflected, and trigger HandleDeflection(). This is the default return value of HandleBlockingHit(). */
		AdvanceNextSubstep,		/** Advance to the next simulation update. Typically used when additional slide/multi-bounce logic can be ignored,
									such as when an object that blocked the projectile is destroyed and movement should continue. */
		Abort,					/** Abort all further simulation. Typically used when components have been invalidated or simulation should stop. */
	};

	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining);
	
	/** Compute the distance we should move in the given time, at a given a velocity. */
	virtual FVector ComputeMoveDelta(const FVector& InVelocity, float DeltaTime) const;

	/**
	 * Given an initial velocity and a time step, compute a new velocity.
	 * Default implementation applies the result of ComputeAcceleration() to velocity.
	 * 
	 * @param  InitialVelocity Initial velocity.
	 * @param  DeltaTime Time step of the update.
	 * @return Velocity after DeltaTime time step.
	 */
	virtual FVector ComputeVelocity(FVector InitialVelocity, float DeltaTime) const;

	/** Compute the acceleration that will be applied */
	virtual FVector ComputeAcceleration(const FVector& InVelocity, float DeltaTime) const;

	/** Allow the projectile to track towards its homing target. */
	virtual FVector ComputeHomingAcceleration(const FVector& InVelocity, float DeltaTime) const;

	/** Computes result of a bounce and returns the new velocity. */
	virtual FVector ComputeBounceResult(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta);

	// Double-buffer of pending force so that updates can use the accumulated value and reset the data so other AddForce() calls work correctly.
	// Also prevents accumulation over frames where the update aborts for whatever reason, and works with substepping movement.
	FVector PendingForceThisUpdate;
#pragma endregion
	
protected:
	static const float MIN_TICK_TIME;

	// Pending force for next tick.
	FVector PendingForce;
	
	int32 MaxSimulationIterations;
	float MaxSimulationTimeStep;
	uint8 bSweepCollision:1;
	int32 BounceAdditionalIterations;
	uint8 Bounces;
};
