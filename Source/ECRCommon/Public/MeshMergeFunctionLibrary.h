// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Customization/CustomizationAttachmentAsset.h"
#include "Customization/CustomizationMaterialAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshMergeFunctionLibrary.generated.h"

class UCustomizationElementaryAsset;

/**
* Struct containing all parameters used to perform a Skeletal Mesh merge.
*/
USTRUCT(BlueprintType)
struct ECRCOMMON_API FSkeletalMeshMergeParams
{
	GENERATED_BODY()
	FSkeletalMeshMergeParams()
	{
		StripTopLODS = 0;
		bNeedsCpuAccess = false;
		bSkeletonBefore = false;
		Skeleton = nullptr;
	}

	// The list of skeletal meshes to merge.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USkeletalMesh*> MeshesToMerge;
	// The number of high LODs to remove from input meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StripTopLODS;
	// Whether or not the resulting mesh needs to be accessed by the CPU for any reason (e.g. for spawning particle effects).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bNeedsCpuAccess : 1;
	// Update skeleton before merge. Otherwise, update after.
	// Skeleton must also be provided.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bSkeletonBefore : 1;
	// Skeleton that will be used for the merged mesh.
	// Leave empty if the generated skeleton is OK.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeleton* Skeleton;
};

//////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FCustomizationElementaryAssetsSelectionEntry
{
	GENERATED_BODY()

	// Assets to apply if the tag matches
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCustomizationElementaryAsset*> Assets;

	// Cosmetic tags required (all of these must be present to be considered a match)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FCustomizationElementaryAssetsSelectionSet
{
	GENERATED_BODY()

	// List of CEA rules to apply, first one that matches will be used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(TitleProperty=RequiredTags))
	TArray<FCustomizationElementaryAssetsSelectionEntry> CeaRules;

	// The assets to use if none of the CeaRules matches
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCustomizationElementaryAsset*> DefaultAssets;
	
	// Choose the best assets given the rules
	TArray<UCustomizationElementaryAsset*> SelectBestAsset(const FGameplayTagContainer& CosmeticTags) const;
};

//////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FCustomizationMaterialAssetsSelectionEntry
{
	GENERATED_BODY()

	// Assets to apply if the tag matches
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCustomizationMaterialAsset*> Assets;

	// Cosmetic tags required (all of these must be present to be considered a match)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FCustomizationMaterialAssetsSelectionSet
{
	GENERATED_BODY()

	// List of CMA rules to apply, first one that matches will be used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(TitleProperty=RequiredTags))
	TArray<FCustomizationMaterialAssetsSelectionEntry> CmaRules;

	// The assets to use if none of the CmaRules matches
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCustomizationMaterialAsset*> DefaultAssets;

	// Modules to apply selected CMA to
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Modules;

	// Comment to describe this set
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Comment;
	
	// Choose the best assets given the rules
	TArray<UCustomizationMaterialAsset*> SelectBestAsset(const FGameplayTagContainer& CosmeticTags) const;
};

//////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FCustomizationAttachmentAssetsSelectionEntry
{
	GENERATED_BODY()

	// Assets to apply if the tag matches
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCustomizationAttachmentAsset*> Assets;

	// Cosmetic tags required (all of these must be present to be considered a match)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FCustomizationAttachmentAssetsSelectionSet
{
	GENERATED_BODY()

	// List of CAA rules to apply, first one that matches will be used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(TitleProperty=RequiredTags))
	TArray<FCustomizationAttachmentAssetsSelectionEntry> CaaRules;

	// The assets to use if none of the CaaRules matches
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCustomizationAttachmentAsset*> DefaultAssets;

	// Comment to describe this set
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Comment;

	// Choose the best assets given the rules
	TArray<UCustomizationAttachmentAsset*> SelectBestAsset(const FGameplayTagContainer& CosmeticTags) const;
};

//////////////////////////////////////////////////////////////////////


/**
*
*/
UCLASS()
class ECRCOMMON_API UMeshMergeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Merges the given meshes into a single mesh.
	* @return The merged mesh (will be invalid if the merge failed).
	*/
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge", meta = (UnsafeDuringActorConstruction = "true"))
	static USkeletalMesh* MergeMeshes(const FSkeletalMeshMergeParams& Params);

	UFUNCTION(BlueprintCallable)
	static TArray<UCustomizationElementaryAsset*> MergeCustomizationElementaryAssets(
		TArray<UCustomizationElementaryAsset*> Assets1,
		TArray<UCustomizationElementaryAsset*> Assets2);

	UFUNCTION(BlueprintCallable)
	static TArray<UCustomizationMaterialAsset*> MergeCustomizationMaterialAssets(
		TArray<UCustomizationMaterialAsset*> Materials1, TArray<UCustomizationMaterialAsset*> Materials2);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<UCustomizationElementaryAsset*> SelectBestCeaArray(
		const FCustomizationElementaryAssetsSelectionSet Set,
		FGameplayTagContainer CosmeticTags);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<UCustomizationMaterialAsset*> SelectBestCmaArray(const FCustomizationMaterialAssetsSelectionSet Set,
	                                                               FGameplayTagContainer CosmeticTags);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<UCustomizationAttachmentAsset*> SelectBestCaaArray(
		const FCustomizationAttachmentAssetsSelectionSet Set,
		FGameplayTagContainer CosmeticTags);
};
