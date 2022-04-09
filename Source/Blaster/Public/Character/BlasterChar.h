// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterChar.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class AWeapon;
class UCombatComponent;


UCLASS()
class BLASTER_API ABlasterChar : public ACharacter
{
	GENERATED_BODY()

public:
	
	ABlasterChar();

	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	

protected:
	
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EquipButtonPressed();


private:
	UPROPERTY(VisibleAnywhere,Category=Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,Category=Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere,Category=Components)
	UCombatComponent* Combat;

	UFUNCTION(Server,Reliable)
	void ServerEquipButtonPressed();


public:	
	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();


};
