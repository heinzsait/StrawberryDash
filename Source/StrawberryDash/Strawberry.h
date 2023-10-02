// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Strawberry.generated.h"

class UCharacterMovementComponent;

UCLASS()
class STRAWBERRYDASH_API AStrawberry : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStrawberry();

	UFUNCTION(BlueprintCallable)
	void Initialize(USceneComponent* _characterMeshComp, USceneComponent* _cameraComp, USceneComponent* _cherryMeshComp, USceneComponent* _dashFX, class USplineComponent* _Spline, USceneComponent* _ActorToMove);

	UFUNCTION(BlueprintCallable)
	FVector GetFireTargetPosition(class USceneComponent* charMesh, class USceneComponent* camera);

	UFUNCTION(BlueprintCallable)
	void FireCherry(USceneComponent* cherry, FVector target);

	UPROPERTY(BlueprintReadWrite)
	float HP;

	UPROPERTY(BlueprintReadWrite)
	bool isDead;

	bool canTakeDamage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> cherryActor;

	USceneComponent* characterMeshComp;
	USceneComponent* cameraComp;
	USceneComponent* cherryMeshComp;
	USceneComponent* dashFX;
	UCharacterMovementComponent* charMovement;

	ACharacter* playerChar;

	UPROPERTY(EditAnywhere)
	float baseLookUpRate;
	UPROPERTY(EditAnywhere)
	float baseTurnRate;
	UPROPERTY(EditAnywhere)
	float defaultGravity;
	UPROPERTY(EditAnywhere)
	float defaultWalkSpeed;
	UPROPERTY(BlueprintReadWrite)
	bool inWallSlide;
	UPROPERTY(EditAnywhere)
	bool canWallDash;
	UPROPERTY(EditAnywhere)
	bool isDashing;

	bool isMovementLocked;


	UPROPERTY(BlueprintReadWrite)
	FVector fireTarget;
	UPROPERTY(BlueprintReadWrite)
	FVector fireTargetNormal;

	UPROPERTY(EditAnywhere)
	UAnimMontage* animMontageDoubleJump;
	UPROPERTY(BlueprintReadWRite, EditAnywhere)
	TSubclassOf<AActor> BlueprintClass_Wall;


	UPROPERTY(BlueprintReadWRite, EditAnywhere)
	TSubclassOf<AActor> CrosshairClass;
	AActor* crosshair;

	class USplineComponent* Spline;

	USceneComponent* ActorToMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnRate(float Value);
	void LookUpRate(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void JumpPressed();
	void JumpReleased();
	void Fire();
	void FireReleased();
	void WalkPressed();
	void WalkReleased();
	void SprintPressed();
	void SprintReleased();
	void Dash();
	void WallDash();
	void MoveCherryAlongSpline();
	void SetCrossHairTransform();

	bool cherryFired;
	bool didDash;
	bool didAirDash;
	bool didWallDash;
	bool didWallSlideVelo;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		float TotalPathTimeController;

	UPROPERTY(EditAnywhere, Category = "SplineController")
		bool bSplineInLoop;

	bool bCanMoveActor;

	float StartTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

	void TakeDamage(float dmg);

};
