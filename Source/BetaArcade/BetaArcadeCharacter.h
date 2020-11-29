// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

#include "BetaArcadeCharacter.generated.h"

UENUM(BlueprintType)
namespace CharacterState // Namespace because enum was throwing "Member ... is not a type name"
{
	enum State
	{
		None		UMETA(DisplayName = "None"),
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* PlayerCamera;
public:
	ABetaArcadeCharacter();

	UFUNCTION(BlueprintCallable)
		void LeftTurn();
	UFUNCTION(BlueprintCallable)
		void RightTurn();

	UPROPERTY(BlueprintReadWrite)
		FVector currentCamPosition = { 0,0,0 };
	UPROPERTY(BlueprintReadWrite)
		FRotator currentCamRotation = { 0,0,0 };
	UPROPERTY(BlueprintReadWrite)
		FRotator currentPlayerRotation = { 0,0,0 };
	UPROPERTY(BlueprintReadWrite)
		bool swarmReacting = false;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	
	FRotator currentRot;
	FVector Direction;
	FVector currentPos;

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// FRAN STUFF
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<CharacterState::State> characterState;
	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<PowerState::State> currentPowerState;

	UPROPERTY()
		UCharacterMovementComponent* playerMovement;
	float playerSpeed = 0.0f;
	float initialPlayerSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float slideTime = 0.0f; // How long the player slides for
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float vaultTime = 0.0f; // How long the player vaults for

	//// FRAN - Camera zoom control
	////APlayerCameraManager* camera = GetCamera
	//void CameraZoomIn();
	//void CameraZoomOut();
	//UPROPERTY(EditAnywhere)
	//float cameraZoomValue = 50.0f;
	//UPROPERTY(EditAnywhere)
	//float minCameraZoom = 0.0f;
	//UPROPERTY(EditAnywhere)
	//float maxCameraZoom = 0.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float minCameraPitch = 0.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float maxCameraPitch = 0.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float minCameraYaw = 0.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float maxCameraYaw = 0.0f;

	//UPROPERTY()
	//	FTransform camBoomTransform;

	const int MAX_PLAYER_LIVES = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int playerLives = 0;

	// Constant run toggle for testing!
	UPROPERTY(EditAnywhere)
		bool constantRun = false;

	bool hasCameraRotated = false;
	UPROPERTY(BlueprintReadWrite)
	bool isCameraBackwards = false;
	UPROPERTY(BlueprintReadWrite)
	bool isCameraZoomed = false; 
	UPROPERTY(EditAnywhere)
	FRotator cameraFlipRotation = { 15,180,0 };
	UPROPERTY(EditAnywhere)
	FVector camZoomPos = { 360,-30,-100 };

	// Swarm stuff
	bool isReacting = false;
	UPROPERTY(BlueprintReadWrite)
		bool isJumping = false;
	UPROPERTY(BlueprintReadWrite)
		bool canVault = false;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	// State control
	void HandleState();
	void CameraFlipControl();
	UFUNCTION(BlueprintImplementableEvent)
		void CameraFlip();

	void BetaJump();
	void JumpEndCheck();
	void BetaJumpStop();

	UFUNCTION(BlueprintCallable)
		bool StartSlide();
	void Slide();
	UFUNCTION(BlueprintCallable)
		void StopSliding();

	UFUNCTION(BlueprintImplementableEvent)
		void VaultControl();
	UFUNCTION(BlueprintCallable)
		bool StartVault();
	UFUNCTION(BlueprintCallable)
		void Vault();
	UFUNCTION(BlueprintCallable)
		void StopVaulting();

	void SwarmReaction() { isReacting = true; }
	void SwarmReactionStop() { isReacting = false; }

public:
	virtual void Tick(float DeltaTime) override;

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
