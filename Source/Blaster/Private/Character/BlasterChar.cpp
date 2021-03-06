// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapon/Weapon.h"
#include "Net/UnrealNetwork.h"
#include "BlasterComponents/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"


ABlasterChar::ABlasterChar()
{


	PrimaryActorTick.bCanEverTick = true;

	CameraBoom=CreateDefaultSubobject<USpringArmComponent>("CamerasBoom");
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength=600.f;
	CameraBoom->bUsePawnControlRotation=1;


	FollowCamera=CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation=0;
	
	bUseControllerRotationYaw=0;
	GetCharacterMovement()->bOrientRotationToMovement=1;

	OverheadWidget=CreateDefaultSubobject<UWidgetComponent>("OverheadWidget");
	OverheadWidget->SetupAttachment(RootComponent);
	
	Combat=CreateDefaultSubobject<UCombatComponent>("CombatComponent");
	Combat->SetIsReplicated(1);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = 1;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}


void ABlasterChar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABlasterChar,OverlappingWeapon,COND_OwnerOnly);



}


void ABlasterChar::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlasterChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
}




void ABlasterChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jumpx", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jumpx", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward / Backward", this, &ABlasterChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight / Left", this, &ABlasterChar::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ABlasterChar::Turn);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ABlasterChar::LookUp);
	
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterChar::EquipButtonPressed);
	
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABlasterChar::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABlasterChar::CrouchButtonReleased);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABlasterChar::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABlasterChar::AimButtonReleased);
	
}



void ABlasterChar::MoveForward(float Value){
	if(Controller!=nullptr&&Value!=0.f){
		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction,Value);
	}
}


void ABlasterChar::MoveRight(float Value){
	if(Controller!=nullptr&&Value!=0.f){
		const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction,Value);
	}
}


void ABlasterChar::Turn(float Value){
	AddControllerYawInput(Value*.7f);

}


void ABlasterChar::LookUp(float Value){
	AddControllerPitchInput(Value*.7f);
}

void ABlasterChar::OnRep_OverlappingWeapon(AWeapon* LastWeapon){
	if(OverlappingWeapon){
		OverlappingWeapon->ShowPickupWidget(1);

	}
	if(LastWeapon)LastWeapon->ShowPickupWidget(0);

}

void ABlasterChar::TurnInPlace(float DeltaTime){
	if (AO_Yaw>90.f) {
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if(AO_Yaw<-90.f) {
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurningInPlace!=ETurningInPlace::ETIP_NotTurning) {
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw,0.f,DeltaTime,4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw)<15.f) {
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);

		}
	}
}

void ABlasterChar::SetOverlappingWeapon(AWeapon* Weapon){
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(0);
	}
	OverlappingWeapon=Weapon;
    if (IsLocallyControlled()) {
        if (OverlappingWeapon) {
            OverlappingWeapon->ShowPickupWidget(1);
        }
    }
}


void ABlasterChar::EquipButtonPressed(){
	if(Combat){
		if(HasAuthority()){
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else{
			ServerEquipButtonPressed();
		}
		

	}
}

void ABlasterChar::ServerEquipButtonPressed_Implementation(){
	if(Combat){
		Combat->EquipWeapon(OverlappingWeapon);

	}
}

void ABlasterChar::CrouchButtonPressed(){
	Crouch();
	
}

void ABlasterChar::CrouchButtonReleased(){
	UnCrouch();
}

void ABlasterChar::PostInitializeComponents() {
	Super::PostInitializeComponents();
	if(Combat){
		Combat->Character=this;
	}
}



bool ABlasterChar::IsWeaponEquipped(){
	return (Combat&&Combat->EquippedWeapon);
	
}


bool ABlasterChar::IsAiming(){
	return (Combat&&Combat->bAiming);
}

AWeapon* ABlasterChar::GetEquippedWeapon()
{
	if (Combat == nullptr)return nullptr;
	return Combat->EquippedWeapon;
}

void ABlasterChar::AimButtonPressed(){
	if(Combat)Combat->SetAiming(1);
}

void ABlasterChar::AimButtonReleased(){
	if(Combat)Combat->SetAiming(0);
}

void ABlasterChar::AimOffset(float DeltaTime){
	if (Combat && Combat->EquippedWeapon == nullptr)return;

	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();
	if (Speed==0.f&&!bIsInAir) {
		FRotator CurrentAimRotation= FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator( CurrentAimRotation,StartingAimRotation);
		AO_Yaw = DeltaRotation.Yaw;
		if (TurningInPlace==ETurningInPlace::ETIP_NotTurning) {
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = 1;
		TurnInPlace(DeltaTime);
	}
	if (Speed>0.f||bIsInAir) {
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = 1;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch>90.f&&!IsLocallyControlled()) {
		//
		FVector2D InRange(270.f,360.f);
		FVector2D OutRange(-90.f,0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);

	}
}












//
















//ABlasterChar
