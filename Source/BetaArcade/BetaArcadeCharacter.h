// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

#include "BetaArcadeCharacter.generated.h"

UENUM(BlueprintType)
namespace CharacterState // Namespace because enum was throwing "Member ... is not a type name"
{
	enum State
	{
		None		UMETA(DisplayName = "None"),
		//Running		UMETA(DisplayName = "Running"),
		Jumping		UMETA(DisplayName = "Jumping"),
		Vaulting	UMETA(DisplayName = "Vaulting"),
		Sliding		UMETA(DisplayName = "Sliding"),
	};
}

UENUM(BlueprintType)
namespace PowerState
{
	enum State
	{
		None		UMETA(DisplayName = "None"),
		SpeedBoost	UMETA(DisplayName = "SpeedBoost"),
		ScoreBonus	UMETA(DisplayName = "ScoreBonus"),
		ExtraLife	UMETA(DisplayName = "ExtraLife"),
		LightBall	UMETA(DisplayName = "LightBall"),
		FullLight	UMETA(DisplayName = "FullLight"),
		SporeEffect	UMETA(DisplayName = "SporeEffect"),
	};
}

UCLASS(config = Game)
class ABetaArcadeCharacter : public ACharacter
{	
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	ABetaArcadeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(BlueprintReadWrite)
		bool swarmReacting = false;

	UFUNCTION(BlueprintCallable)
		void TurnCamera();

	UFUNCTION(BlueprintCallable)
		void LeftTurn();

	UFUNCTION(BlueprintCallable)
		void LeftTurnEnd();

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// FRAN STUFF
	// State control
	void HandleState();
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//PlayerState playerState = PlayerState::Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<CharacterState::State> characterState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<PowerState::State> currentPowerState;

	UPROPERTY()
		UCharacterMovementComponent* playerMovement;
	float playerSpeed = 0.0f;
	float initialPlayerSpeed = 0.0f;

	float time = 0.0f;
	float endTime = 0.0f;

	void BetaJump();
	void BetaJumpStop();
	void StartSlide();
	void Slide();
	void StopSliding();

	UPROPERTY(EditAnywhere)
	float slideTime = 0.0f; // How long the player slides for

	UPROPERTY()
	FRotator currentRot;

	// Swarm stuff
	bool isReacting = false;
	void SwarmReaction() { isReacting = true; }
	void SwarmReactionStop() { isReacting = false; }

	// FRAN - Camera zoom control
	//APlayerCameraManager* camera = GetCamera
	void CameraZoomIn();
	void CameraZoomOut();
	UPROPERTY(EditAnywhere)
	float cameraZoomValue = 50.0f;
	UPROPERTY(EditAnywhere)
	float minCameraZoom = 0.0f;
	UPROPERTY(EditAnywhere)
	float maxCameraZoom = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minCameraPitch = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxCameraPitch = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minCameraYaw = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxCameraYaw = 0.0f;

	UPROPERTY()
		FTransform camBoomTransform;

	const int MAX_PLAYER_LIVES = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int playerLives = 0;

	// Constant run toggle for testing!
	UPROPERTY(EditAnywhere)
		bool constantRun = false;

	FVector Direction;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// LIVES
	UFUNCTION(BlueprintCallable)
	int GetPlayerLives() { return playerLives; };
	UFUNCTION(BlueprintCallable)
	void AddPlayerLives(int lives) { playerLives += lives; }; // Adds however many lives are passed in, to take away lives just pass in a negative

	bool GetSwarmReaction() { return swarmReacting; };

	// SPEED
	UFUNCTION(BlueprintCallable)
	void ResetPlayerSpeed() { playerMovement->MaxWalkSpeed = initialPlayerSpeed; }; // Sets speed to original value
	UFUNCTION(BlueprintCallable)
	void SetPlayerSpeed(float speed) { playerMovement->MaxWalkSpeed = speed; };
};
