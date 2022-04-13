// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterAnimInstance.h"
#include "Character/BlasterChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Weapon.h"


void UBlasterAnimInstance::NativeInitializeAnimation(){
    Super::NativeInitializeAnimation();

    BlasterCharacter=Cast<ABlasterChar>(TryGetPawnOwner());

}



void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime){
    Super::NativeUpdateAnimation(DeltaTime);
    if(BlasterCharacter==nullptr){
        BlasterCharacter=Cast<ABlasterChar>(TryGetPawnOwner());

    }
    if(BlasterCharacter==nullptr)return;


    FVector Velocity=BlasterCharacter->GetVelocity();
    Velocity.Z=0.f;
    Speed=Velocity.Size();
    bIsInAir=BlasterCharacter->GetCharacterMovement()->IsFalling();
    bIsAccelerating=BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size()>0.f?1:0;
    bWeaponEquipped=BlasterCharacter->IsWeaponEquipped();
    EquippedWeapon = BlasterCharacter->GetEquippedWeapon();
    bIsCrouched=BlasterCharacter->bIsCrouched;
    bAiming=BlasterCharacter->IsAiming();
    TurningInPlace = BlasterCharacter->GetTurningInPlace();
    
    FRotator AimRotation= BlasterCharacter->GetBaseAimRotation();
    FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());
    FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
    DeltaRotation = FMath::RInterpTo(DeltaRotation,DeltaRot,DeltaTime,15.f);
    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation = BlasterCharacter->GetActorRotation();
    YawOffset = DeltaRotation.Yaw;
    const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
    const float Target = Delta.Yaw / DeltaTime;
    const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
    Lean = FMath::Clamp(Interp, -90.f, 90.f);

    AO_Yaw = BlasterCharacter->GetAO_Yaw();
    AO_Pitch = BlasterCharacter->GetAO_Pitch();
    if (bWeaponEquipped&&EquippedWeapon&&EquippedWeapon->GetWeaponMesh()&&BlasterCharacter->GetMesh()) {
        LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
        FVector OutPosition;
        FRotator OutRotation;
        BlasterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"),LeftHandTransform.GetLocation(),FRotator::ZeroRotator, OutPosition, OutRotation);
        LeftHandTransform.SetLocation(OutPosition);
        LeftHandTransform.SetRotation(FQuat(OutRotation));


    }



}









//
















//UBlasterAnimInstance