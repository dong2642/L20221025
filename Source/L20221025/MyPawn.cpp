// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/FloatingPawnMovement.h"
#include "Engine/StaticMesh.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�ڽ� �����
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	//�ڽ��� �ٵ�
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);
	
	// Body �Ž� �ҷ�����, if�� ���� ���θ� ���
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	//�ٵ� ����Ʈ
	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	//�ٵ� ����Ʈ
	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	//�����緯 ���̱�
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Right->SetStaticMesh(SM_Propeller.Object);
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetRelativeLocation(FVector(36.269119f, 21.026630f, 0.310581f));
		Left->SetRelativeLocation(FVector(36.269119f, -21.026630f, 0.310581f));

	}

	// �ڽ��� �������� ���̱�
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm -> SetupAttachment(Box);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	//�������Ͽ� ī�޶� ���̱�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//�ڽ��� ���ο츦 ���̰� ��ġ�� X�� +80�� 
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(80.f, 0, 0));

	//������ �����Ʈ ����. �ӵ��� 100
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = MoveSpeed;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�����緯 ȸ��
	Left->AddLocalRotation(FRotator(0, 0, PropellerRoationSpeed * DeltaTime));
	Right->AddLocalRotation(FRotator(0, 0, PropellerRoationSpeed * DeltaTime));

	//AddActorLocalOffset(FVector(100.0f * DeltaTime, 0, 0));
	//���� ������ �̵�
	AddMovementInput(GetActorForwardVector());

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);

}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AActor>(AActor::StaticClass(),
		GetActorLocation(),
		GetActorRotation());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("StaticMesh'/Game/P38/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Rocket.Succeeded())
	{

	}
}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
		0,
		0
	));
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0, 0, Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())
	));
}

