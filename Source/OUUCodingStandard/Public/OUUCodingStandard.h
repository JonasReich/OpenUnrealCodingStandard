// Copyright (c) 2025 Jonas Reich

// [basic.epic] Unless explicitly stated otherwise by the AUTHOR/STUDIO coding standard, follow the Epic coding standard
// as a baseline. Especially for header files / public API. See
// https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/

// [basic.uht] Some rules (like include order + public inheritance) are enforced by UnrealHeaderTool and are need not
// to be explicitly stated here.

// [basic.files] Types should be split into a sensible hierarchy of public/private files that is divided into plugins
// and modules with re-usability and clear dependencies in mind.
// Types that can be declared independently should receive independent header files.
// NOTE: This plugin breaks with this rule, to allow reading as much as possible in as few files as possible.

// [basic.const] const is documentation of intent as much as it is a compiler directive, so all code should strive to be
// const-correct. (As far as possible, note that in many places Unreal will make it very difficult to maintain
// const-correctness. Especially code interfacing with blueprints has many restrictions there.)
//
// -> Passing function arguments by const pointer or reference if those arguments are not intended to be modified by the
// function.
// -> Flagging member functions as const if they do not modify the object.
// -> Using const iteration over containers if the loop isn't intended to modify the container.
// -> constexpr allows the compiler to evaluate certain function calls at compile-time if the parameters are also known
// at compile-time. This can improve runtime performance, so consider using this wherever appropriate. Remember that
// constexpr can also be used on non-const member functions!

// [basic.noconst] There are exceptions to the rule above:
// - passing parameters by value -> see [func.param.types]
// - return values -> see [func.retval.noconst]

// [basic.cppstandard] This project and Unreal Engine in general are using C++17 and subsets of C++20.
// Using modern C++ language features such as [[nodiscard]] and [[fallthrough]] attributes, constexpr,
// structured bindings, lambdas, etc. is strongly encouraged wherever applicable.

// [basic.stl] Unreal provides custom versions of many types and features provided by the C++ STL. Always prefer
// Unreal's types as it will make it easier to interface with existing Unreal code.
// This is mainly intended to ensure cross-platform support - but sometimes also to make subtle performance tweaks.
// This affects all areas from memory allocations to smart pointers, containers, math and geometry, file system access
// and much more.

// [basic.disable_code] Avoid checking in commented-out or permanently unreachable code.
// There are two exceptions:
// - Code examples accompanied by descriptive comments (e.g. in API docs).
// - Removed code accompanied by a comment explaining why this code is NOT here anymore (esp. in engine code changes)
// For these two cases commmented out code is preferrable, because it won't trip up static analyzers or IDE linters.

// [naming.identifiers]
// Identifier names should be short but descriptive. Avoid abbreviations, slang or anything else that might lead to
// misunderstandings.

// [naming.prefixes]
// Prefix plugins and their modules with one of the following:
// - YOUR STUDIO PREFIXES HERE
// - OUU for everything related to OpenUnrealUtilities plugin
// Classes should also receive the module prefix to avoid name clashes.
// This is particularly important for types affected by Unreal reflection (UCLASS, USTRUCT, UENUM, etc).
// It's already cumbersome to add core-redirectors after a rename on your side but even more so if the name conflicts
// with a type introduced by Epic in an Engine upgrade or a different third party plugin you don't even know about.

// [basic.doc] Write docs for all public API identifiers, especially types and functions.
// Exceptions can be made for self-explanatory identifier names and types declared by Unreal boilerplate macros, such as
// log categories, module implementations, slate arguments, etc.
// It's generally fine to assume a basic understanding of C++ and Unreal principles, but err on the side of caution and
// overexplain intent (to make sure your implementation is not miusunderstood) and underexplain implementation details
// (to avoid comment and implementation going out of sync).

//---------------------------------------------------------------------------------------------------------------------
// [header.guard] Always use #pragma once before any includes (UHT already enforces this for reflected files)
#pragma once

// [include.iwyu] Use IWYU include style for all of your headers.
// See https://docs.unrealengine.com/latest/INT/Programming/BuildTools/UnrealBuildTool/IWYU/index.html

#include "CoreMinimal.h"

// [include.root] Never use include paths relative to your file. This also applies to source files, but especially so to
// header files. Instead, make the include paths relative to the Public/ or Classes/ directory of the source module.
// This is to make sure that the include paths resolve correctly for dependent plugins.
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Misc/EnumRange.h"

#include "OUUCodingStandard.generated.h"

//---------------------------------------------------------------------------------------------------------------------
// [header.fwd] Use forward-declarations instead of includes wherever possible.
// Forward declarations should always be made here instead of inline to make it easier to get an overview of "soft
// dependencies" and reduces keyword duplication in the code below making it easier to read.
class USkeletalMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogOUUCodingStandard, Log, All);

namespace OUU::CodingStandard
{
	DECLARE_DELEGATE(FFileOperationEvent);
	DECLARE_DELEGATE(FOnActorDestroyed);
} // namespace OUU::CodingStandard

//---------------------------------------------------------------------------------------------------------------------
/**
 * The color for body parts of colorables.
 * See IExampleColorableInterface
 */
UENUM(BlueprintType)
// [enum.class] Always use enum classes in favor of c-style enums
enum class EOUUExampleBodyPartColor : uint8
{
	// The color associated with love, blood and anger.
	Red,
	// The color associated with nature and calmness.
	Green,
	// The color associated with water, the sky and the ocean.
	Blue,

	// [enum.count] If you need to iterate over enum entries, or otherwise need the enum count, declare a last case
	// called 'Count' or 'NumOf'. Consider adding a UMETA(Hidden) attribute to hide it from Blueprint usage.
	Count UMETA(Hidden)
};

// [enum.range.decl] Prefer to declare an enum range when possible over integer based iteration.
// If you do, declare it immediately after the enum itself.
// -> see Engine/Source/Runtime/Core/Public/Misc/EnumRange.h
// -> see [enum.range.use]
ENUM_RANGE_BY_COUNT(EOUUExampleBodyPartColor, EOUUExampleBodyPartColor::Count);

// [doc.delegate.type] Prefer documenting the meaning of parameters at the delegate type declaration over documenting
// the parameters at the delegate instance. This makes it easier to reuse the same delegate without duplicating docs.
/**
 * @param	BodyPartName		Name ID of the body part that was re-colored.
 * @param	OldBodyPartColor	Color preset that was applied before the change.
 * @param	NewBodyPartColor	Color preset that is applied now.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnExampleColorablePartColorChanged,
	FName,
	BodyPartName,
	EOUUExampleBodyPartColor,
	OldBodyPartColor,
	EOUUExampleBodyPartColor,
	NewBodyPartColor);

// [interface.cpponly] Mark interfaces as CannotImplementInterfaceInBlueprint if possible.
// This makes it possible to use regular casts and invoke functions directly instead of relying on Execute_ functions.
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UOUUExampleColorableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * An entity (usually an AActor) that has colorable body-parts.
 */
class IOUUExampleColorableInterface
{
	GENERATED_BODY()
public:
	/**
	 * Color a body part by name.
	 * @param		BodyPartName	Name ID of the body part to be colored.
	 * @param		BodyPartColor	Color preset to apply to the body-part.
	 * @returns		true if the body part was found and successfully colored.
	 */
	UFUNCTION(BlueprintCallable)
	virtual bool ColorBodyPart(FName BodyPartName, EOUUExampleBodyPartColor BodyPartColor) = 0;
};

//---------------------------------------------------------------------------------------------------------------------
UENUM()
enum class EAwesomenessLevel
{
	NotAwesome,
	SemiAwesome,
	Awesome,

	NumOf
};

// [namespace] Reflected types (uclass, ustruct, uenum, etc) cannot be put into namespaces.
// Everything else should be put into namespaces, especially free functions that could otherwise result in name clashes.

namespace OUU::CodingStandard
{
	// [function.export] Add DLL export macros (here: OUUCODINGSTANDARD_API) to all functions declared in headers that
	// need dynamic linkage. Only exception: If you add a function inside a "Private"/"Implementation only" namespace to
	// clearly mark that it's not expected to be called by external dependents.
	OUUCODINGSTANDARD_API EAwesomenessLevel AwesomenessLevelFromIntValue(int32 Awesomeness);

	// [string.conv] Overload the LexToString/TryLexFromString for custom primitive string conversion instead of
	// coming up with own names.
	OUUCODINGSTANDARD_API FString LexToString(EAwesomenessLevel InAwesomenessLevel);

	OUUCODINGSTANDARD_API bool TryLexFromString(EAwesomenessLevel& OutAwesomenessLevel, const FString& String);

	/**
	 * Track how awesome a character is.
	 */
	struct FNumericAwesomeness
	{
	public:
		// [ctor.default] Use default notation instead of declaring an empty bodied constructor.
		FNumericAwesomeness() = default;

		// [ctor.initializer.inline] An initializing constructor may be inlined.
		FNumericAwesomeness(int32 InAwesomeness, const FString& InAwesomenessReason) :
			AwesomenessReason(InAwesomenessReason), Awesomeness(InAwesomeness)
		{
		}

		// [ctor.delegate] Delegate parameter constructors to a single one that takes all of them, unless impossible.
		// [ctor.explicit] Single-argument constructors must be declared as explicit unless implicit conversion is
		// specifically wanted. In that case, this conversion behavior needs to be documented.
		explicit FNumericAwesomeness(int32 InAwesomeness) : FNumericAwesomeness(InAwesomeness, TEXT("unknown reason"))
		{
		}

		// [func.comp_ops] When implementing comparison operators, use the binary free form
		// as it provides the most flexibility with operands order and usage.
		// If it needs to access private members, make it 'friend'.
		// NOTE: Add working functionality only for '==' and '<' everything else can be inferred from them
		// This does NOT work with member operators (in C++17).
		friend bool operator==(const FNumericAwesomeness& LHS, const FNumericAwesomeness& RHS);
		friend bool operator<(const FNumericAwesomeness& LHS, const FNumericAwesomeness& RHS);

		// Why the character is so awesome
		FString AwesomenessReason = TEXT("");

		// Get this character's numeric awesomeness converted to a fixed-step level.
		EAwesomenessLevel GetAwesomenessLevel() const;

	private:
		// How awesome the character is
		int32 Awesomeness = 0;
	};

	// [order.inline_funcs] Declare inlined functions with a non-empty body immediately after the owning type
	// declaration, not within. Use the same order for function definitions as for the declarations.
	inline EAwesomenessLevel FNumericAwesomeness::GetAwesomenessLevel() const
	{
		return AwesomenessLevelFromIntValue(Awesomeness);
	}

	inline bool operator==(const FNumericAwesomeness& LHS, const FNumericAwesomeness& RHS)
	{
		return LHS.Awesomeness == RHS.Awesomeness;
	}

	inline bool operator<(const FNumericAwesomeness& LHS, const FNumericAwesomeness& RHS)
	{
		return LHS.Awesomeness < RHS.Awesomeness;
	}
} // namespace OUU::CodingStandard

//---------------------------------------------------------------------------------------------------------------------
// [namespace.nesting] Nested namespaces should receive an inline declaration in global scope instead of actually
// nesting the scope braces. This makes it easier to move the namespaced declarations to a separate file and see the
// fully-qualified namespace name at a glance.
namespace OUU::CodingStandard::Templates
{
	template <typename InElementType, typename InAllocatorType, int32 InDefaultSlack>
	struct TMyContainer
	{
	public:
		using ElementType = InElementType;
		using AllocatorType = InAllocatorType;
		static constexpr int32 DefaultSlack = InDefaultSlack;

		// [static_assert] Use static assert in templates to improve compile safety and error verbosity
		static_assert(DefaultSlack >= 8, "A default slack size of 8 or more is required, because xyz");
	};

} // namespace OUU::CodingStandard::Templates

//---------------------------------------------------------------------------------------------------------------------
/**
 * This is a sample character class that is not meant to be exported or used in other modules.
 */
UCLASS()
// [class.export] Add DLL export macros (here: OUUCODINGSTANDARD_API) to all types declared in headers that need
// dynamic linkage. It's generally a bad habit to declare public functions that are then not usable because they are not
// DLL exported.
class OUUCODINGSTANDARD_API AOUUExampleCharacter : public ACharacter, public IOUUExampleColorableInterface
{
	// [macro.genbody] Use GENERATED_BODY() instead of the deprecated GENERATED_UCLASS_BODY(),
	// GENERATED_IINTERFACE_BODY(), etc.
	GENERATED_BODY()

public:
	// [alias.using] Use using declarations for aliasing instead of typedefs
	using FCharacterData = OUU::CodingStandard::FNumericAwesomeness;
	using FCharacterMeshPtr = TWeakObjectPtr<USkeletalMeshComponent>;

	DECLARE_EVENT_OneParam(AOUUExampleCharacter, FOnAwesomenessChanged, EAwesomenessLevel);
	FOnAwesomenessChanged OnAwesomenessChanged;

	// [member.constant.primitive] Primitive constants should be declared as constexpr, if possible.
	// Prefer this any time over defines, c-style enums or static const values that are defined in cpp.
	static constexpr int32 NumBodyParts = 2;

	// [member.constant.complex] Complex constants (like FNames) that cannot be declared as constexpr should be declared
	// like this:
	static const FName HeadBodyPartName;
	static const FName TorsoBodyPartName;

	// [uclass.ctor] Prefer the parameterless default constructor for UObjects instead of the one using
	// FObjectInitializer.
	AOUUExampleCharacter();

	AOUUExampleCharacter(USkeletalMesh* InSkeletalMesh, EOUUExampleBodyPartColor InHeadColor);

	UPROPERTY(BlueprintAssignable)
	FOnExampleColorablePartColorChanged OnBodyPartColorChanged;

	UPROPERTY(BlueprintAssignable)
	FOnExampleColorablePartColorChanged OnHeadColorChanged;

	UPROPERTY(BlueprintAssignable)
	FOnExampleColorablePartColorChanged OnTorsoColorChanged;

	// Checks if all possible colors are assigned to this character in any body part
	bool HasAllColorsPossible() const;

	// [order.overrides] Overridden functions are grouped by the class where the function was first declared.
	// Each group must start with a comment indicating the originating parent class.
	// That is the parent class where the function was first declared.

	// [member.virtual.overrides] Overriden virtual functions must be marked as either override or final.
	// Do NOT use the virtual keyword. Using virtual for an override declaration technically works but is a tripmine for
	// later changes: If the function of the base class is changed to not be virtual anymore you expect to get a compile
	// error, but instead the function is now a silent "new" virtual function.

	// [doc.member.virtual] Virtual overrides should not need to be documented as the API should be consistent with
	// the initial declaration. Anything else is a breach of contract and MUST be commented.

	// -- AActor interface
	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	// -- IOUUExampleColorableInterface
	bool ColorBodyPart(FName BodyPartName, EOUUExampleBodyPartColor BodyPartColor) override;

protected:
	// [naming.func.onrep] Functions bound to property replication events are named 'OnRep_' + VariableWithoutPrefix.
	UPROPERTY(ReplicatedUsing = OnRep_Score)
	int32 Score = 0;

	// [naming.func.rpc] Remote procedure calls should be prefixed with the type of RPC + '_'.
	UFUNCTION(Server, reliable)
	void Server_SendDataToServer();

	UFUNCTION(Client, reliable)
	void Client_SendDataToClient();

	// [rpc.reliability] Functions should be marked as unreliable whenever possible. This is mostly for cosmetic events
	// that are sent to clients.
	UFUNCTION(NetMulticast, unreliable)
	void Multicast_SendDataToEveryone();

private:
	struct FNestedStruct
	{
		// ...
	};

	// [member.init] Initialize member via assignment, unless it's a default constructible struct or initialized from a
	// constructor parameter. The editor also checks for this during startup.
	// Since this is the prevelant style in Unreal and default values (esp. if they are "invalid" defaults like
	// INDEX_NONE, they can be considered part of the API) so I would discourage the constructor initializer list for
	// default values.
	UPROPERTY(VisibleAnywhere)
	EOUUExampleBodyPartColor HeadColor = EOUUExampleBodyPartColor::Red;

	UPROPERTY(VisibleAnywhere)
	EOUUExampleBodyPartColor TorsoColor = EOUUExampleBodyPartColor::Red;

	bool bWasColorChanged = false;

	FCharacterData CharacterData;

	// [nullptr] Use nullptr instead of NULL macro or 0 literal in all cases.
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HeadMeshComponent = nullptr;

	FDelegateHandle BoundDelegateHandle;

	UFUNCTION()
	void HandleOwnAwesomenessChanged(EAwesomenessLevel Awesomeness) const;

	UFUNCTION()
	void OnRep_Score(int32 ReplicatedScore);
};

//---------------------------------------------------------------------------------------------------------------------
UCLASS()
class UOUUExampleBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// [doc.bp_func_lib] Always add a category for blueprint function library functions, so they are grouped properly in
	// the Blueprint editor. Use Plugin|Class nesting. (This is now enforced by Epic for engine code and marketplace
	// plugins)
	UFUNCTION(BlueprintPure, Category = "OUUCodingStandard|Awesomeness")
	static int32 GetAwesomenessThreshold();
};
