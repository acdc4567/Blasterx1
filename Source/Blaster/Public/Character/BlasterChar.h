// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterChar.generated.h"

class USpringArmComponent;
class UCameraComponent;


UCLASS()
class BLASTER_API ABlasterChar : public ACharacter
{
	GENERATED_BODY()

public:
	
	ABlasterChar();

	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	

private:
	UPROPERTY(VisibleAnywhere,Category=Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,Category=Camera)
	UCameraComponent* FollowCamera;

public:	
	



};