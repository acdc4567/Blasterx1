// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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

}

void ABlasterChar::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlasterChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




void ABlasterChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jumpx", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jumpx", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ABlasterChar::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ABlasterChar::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ABlasterChar::Turn);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ABlasterChar::LookUp);
	
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










//









//ABlasterChar
