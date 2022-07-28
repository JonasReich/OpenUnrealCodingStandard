// Copyright (c) 2022 Jonas Reich
// [header.copyright] Every header file must start with the copy right notice above

// [h.divider] These divider lines may be used in long header files, but generally their presence is a sign of breaking
// the [basic.files] rule
//---------------------------------------------------------------------------------------------------------------------
// [basic.epic] Unless explicitly stated otherwise by the AUTHOR/STUDIO coding standard, follow the Epic coding standard as a
// baseline. Especially for header files / public API.
// See https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/

// [basic.language] Use US English for all names and comments.
//  BAD -> FColor SpezialisierteFarbe;
//  BAD -> FColor SpecialisedColour;
//  GOOD -> FColor SpecializedColor;

// [basic.format] Auto-format all of your source-files using the clang-format files and application provided with the
// project. Any rules enforced by the clang-format are omitted from this file.

// [basic.files] Types should be split into a sensible hierarchy of public/private files that is divided into plugins
// and modules with re-usability and clear dependencies in mind.
// Types that can be declared independently should receive independent header files.
// NOTE: This convention breaks with this rule, to keep the reading experience contained to as few files as possible.

// [basic.const] const is documentation as much as it is a compiler directive, so all code should strive to be
// const-correct. (As far as possible, note that in many places Unreal will make it very difficult to maintain
// const-correctness. Especially an code interfacing with blueprints has many restrictions there.)
//
// -> Passing function arguments by const pointer or reference if those arguments are not intended to be modified by the
// function.
// -> Flagging methods as const if they do not modify the object.
// -> Using const iteration over containers if the loop isn't intended to modify the container.
// -> constexpr allows the compiler to evaluate certain function calls at compile-time if the parameters are also known
// at compile-time. This can improve runtime performance, so consider using this wherever appropriate. Remember that
// constexpr can also be used on non-const member functions!

// [basic.noconst] const should be avoided in the following situations:
// - passing parameters by value -> see [func.param.types]
// - return values -> see [func.retval.noconst]

// [basic.uht] Some rules (like include order + public inheritance) are enforced by UnrealHeaderTool and are need not
// to be explicitly stated here.

// [basic.cppstandard] This project uses C++17 standard (or higher).
// Using modern C++ language features such as [[nodiscard]] and [[fallthrough]] attributes, constexpr,
// structured bindings, lambdas, etc. is strongly encouraged wherever applicable.

// [basic.stl] Unreal provides custom versions of most types and features provided by the C++ STL. Always prefer
// Unreal's types as it will make it easier to interface with existing Unreal code.
// Using Epic's macro wrappers such as CONSTEXPR instead of the built-in keywords is also encouraged for more portable
// code.

// [basic.disable_code] Never check-in commented-out code to deactivate it.
// Use preprocessor guards or completely remove the code instead.
// There is only one exception: Code examples accompanied by descriptory comments (e.g. in API docs).

// [basic.naming]
// Identifier names should be short but descriptive. Avoid abbreviations, slang or anything else that might lead to
// misunderstandings.
//
// Prefix plugins and their modules with one of the following:
// - YOUR STUDIO PREFIXES HERE
// - OUU for everything related to OpenUnrealUtilities plugin
// Classes may also receive the module prefix to avoid name clashes.

// [basic.doc] Write docs for all public API identifiers, especially types and functions. General rule:
// - Multi line typedoc comments for all types and functions.
// - Single line comments for fields and constants.
// Exceptions can be made for self-explanatory identifier names and types declared by Unreal boilerplate macros, such as
// log categories, module implementations, slate arguments, etc.

//---------------------------------------------------------------------------------------------------------------------
// [order.h] Header files are ordered roughly like this:
//  1. Include guard
//  2. Includes
//  3. Forward declarations
//  4. Aliases
//  5. Macro based type declarations
//  6. Enums
//  7. Structs
//  8. Classes
//  9. Functions
// 10. Namespaces -> Start from 4. again

//---------------------------------------------------------------------------------------------------------------------
// [header.guard] Always use #pragma once before any includes
#pragma once

// [include.iwyu] Use IWYU include style for all of your headers.
// See https://docs.unrealengine.com/latest/INT/Programming/BuildTools/UnrealBuildTool/IWYU/index.html
// [include.quotes] Use "double quotes" for include paths instead of <angled brackets>.
// [include.coreminimal] Always include CoreMinimal.h first in your headers.
#include "CoreMinimal.h"

// [include.root] Never use include paths relative to your file. This also applies to source files, but especially so to
// header files. Instead, make the include paths relative to the Public/ or Classes/ directory of the source module.
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Misc/EnumRange.h"

// [include.generated] Include the generated header file last.
#include "OUUCodingStandard.generated.h"

//---------------------------------------------------------------------------------------------------------------------
// [header.fwd] Use forward-declarations instead of includes wherever possible
class USkeletalMeshComponent;

// [macro.decl] Macro based declarations that do not rely on types declared in the header file itself should always come
// first after forward-declarations. Otherwise immediately after the related type.
// (e.g. log categories, delegates)
DECLARE_LOG_CATEGORY_EXTERN(LogOUUCodingStandard, Log, All);

// [naming.delegate.type] Delegate types have two permissible naming schemes:
// 1. FFooEvent -> Used for generic groups of events that share a common signature, e.g. FFileOperationEvent
// 2. FOnFoo -> Used for single purpose events, matching the delegate instance name, e.g. FOnActorDestroyed
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSomethingHappenedEvent);

//---------------------------------------------------------------------------------------------------------------------
// [doc.enum] Write type docs for enums
/**
 * The color for body parts of colorables.
 * See IExampleColorableInterface
 */
UENUM(BlueprintType)
// [enum.class] Always use enum classes in favor of c-style enums
// [naming.enum] Enum classes are prefixed with E
enum class EOUUExampleBodyPartColor : uint8
{
	// [doc.enum.case] Commenting enum cases is optional. Decide on case-by-case basis.
	// When commenting enum cases, place the comment above the case label, not behind it.

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

// [enum.cstyle] If for some reason you absolutely need a c-style enum, prefix the cases with k_ + enum name, like so:
enum ECStyleOUUExampleBodyPartColor
{
	k_CStyleOUUExampleBodyPartColor_Red,
	k_CStyleOUUExampleBodyPartColor_Green,
	k_CStyleOUUExampleBodyPartColor_Blue,

	k_CStyleOUUExampleBodyPartColor_Count
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnExampleColorablePartColorChanged,
FName,
BodyPartName,
EOUUExampleBodyPartColor,
OldBodyPartColor,
EOUUExampleBodyPartColor,
NewBodyPartColor);

// [naming.interface.uclass] Same as the corresponding IInterface class but with changed type prefix (U instead of I).
// [doc.interface.uclass] The UInterface does not need a type comment, as it's mainly required for the reflection data.
// [interface.cpponly] Mark intefaces as CannotImplementInterfaceInBlueprint if possible.
// This makes it possible to use regular casts and invoke functions directly instead of relying on Execute_ functions.
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UOUUExampleColorableInterface : public UInterface
{
	GENERATED_BODY()
};

// [doc.interface] The IInterface should receive the regular type-doc treatment.
/**
 * An entity (usually an AActor) that has colorable body-parts.
 */
class IOUUExampleColorableInterface
{
	GENERATED_BODY()
public:
	// [doc.function] Function declarations should also be documented with multiline type-doc comments.
	// Document parameters and return values with @param and @return notation respectively.
	/**
	 * Color a body part by name.
	 * @param		_BodyPartName	Name ID of the body part to be colored.
	 * @param		_BodyPartColor	Color preset to apply to the body-part.
	 * @returns		true if the body part was found and successfully colored.
	 */
	UFUNCTION(BlueprintCallable)
		virtual bool ColorBodyPart(FName _BodyPartName, EOUUExampleBodyPartColor _BodyPartColor) = 0;
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
// Use the following namespace structure: OUU::ModuleName or OUU::ModuleName::Private
namespace OUU::CodingStandard
{
	EAwesomenessLevel AwesomenessLevelFromIntValue(int32 _Awesomeness);

	// [string.conv] Overload the LexToString/TryLexFromString for custom primitive string conversion instead of
	// coming up with own names.
	// [naming.func.param.in] Prefix function input parameters with underscores '_'.
	FString LexToString(EAwesomenessLevel _AwesomenessLevel);

	// [naming.func.param.out] Prefix out-by-ref-parameters with '_Out' instead of '_'.
	bool TryLexFromString(EAwesomenessLevel& _OutAwesomenessLevel, const FString& _String);

	/**
	 * Track how awesome a character is.
	 */
	 // [naming.struct] Structs are prefixed with F
	struct FNumericAwesomeness
	{
	public:
		// [ctor.default] Use default notation instead of declaring an empty bodied constructor.
		FNumericAwesomeness() = default;

		// [ctor.initalizer.inline] An initializing constructor may be inlined.
		FNumericAwesomeness(int32 _Awesomeness, FString _AwesomenessReason)
			: AwesomenessReason(_AwesomenessReason)
			, Awesomeness(_Awesomeness)
		{
		}

		// [ctor.delegate] Delegate parameter contructors to a single one that takes all of them, unless impossible.
		// [ctor.explicit] Single-argument constructors must be declared as explicit unless implicit conversion is
		// specifically wanted. In that case, this conversion behavior needs to be documented.
		explicit FNumericAwesomeness(int32 _Awesomeness)
			: FNumericAwesomeness(_Awesomeness, TEXT("unknown reason"))
		{
		}

	public:
		// [struct.functions] Structs may only have constructor, operator and conversion functions.
		// If it gets any more complicated than that, you should declare them as class instead.

		// Get this character's numeric awesomeness converted to a fixed-step level.
		EAwesomenessLevel GetAwesomenessLevel() const;

		// [func.comp_ops] When implementing comparison operators, use the binary free form
		// as it provides the most flexibility with operands order and usage.
		// If it needs to access private members, make it 'friend'.
		// NOTE: Add working functionality only for '==' and '<' everything else can be inferred from them
		friend bool operator==(const FNumericAwesomeness& _LHS, const FNumericAwesomeness& _RHS);
		friend bool operator<(const FNumericAwesomeness& _LHS, const FNumericAwesomeness& _RHS);

	public:
		// Why the character is so awesome
		FString AwesomenessReason = TEXT("");

	private:
		// How awesome the character is
		int32 Awesomeness = 0;
	};

	// [member.inline] Declare inlined functions immediately after the owning type declaration, not within.
	// [order.members.function] Use the same order for function definitions as for the declarations.
	// This also applies to definitions in the cpp file.
	inline EAwesomenessLevel FNumericAwesomeness::GetAwesomenessLevel() const
	{
		return AwesomenessLevelFromIntValue(Awesomeness);
	}

	inline bool operator==(const FNumericAwesomeness& _LHS, const FNumericAwesomeness& _RHS)
	{
		return _LHS.Awesomeness == _RHS.Awesomeness;
	}

	inline bool operator<(const FNumericAwesomeness& _LHS, const FNumericAwesomeness& _RHS)
	{
		return _LHS.Awesomeness < _RHS.Awesomeness;
	}

	// [namespace.end] Add a namespace end comment. This is automatically done by clang-format, but it fails to update
	// renames, so you should keep an open eye.
} // namespace OUU::CodingStandard

//---------------------------------------------------------------------------------------------------------------------
// [namespace.nesting] Nested namespaces should receive an inline declaration in global scope instead of actually
// nesting the scope braces. This makes it easier to move the namespaced declarations to a separate file and see the
// fully-qualified namespace name at a glance.
namespace OUU::CodingStandard::Templates
{
	// [naming.template.type] Template types should always be prefixed with T
	// [naming.template.param] All template parameters should be prefixed with underscore and exposed to users via using
	// declaration
	// [naming.template.paramtype] Paramater types should be suffixed with 'Type' or 'Types' in case of a parameter pack
	// [template.paramtype] Parameter types should always use 'typename' instead of 'class'
	template <typename _ElementType, typename _AllocatorType, int32 _DefaultSlack>
	struct TMyContainer
	{
	public:
		// [naming.template.alias] No type prefix needed for the aliases.
		using ElementType = _ElementType;
		using AllocatorType = _AllocatorType;
		static CONSTEXPR int32 DefaultSlack = _DefaultSlack;

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
// dynamic linkage.
// [naming.class] Class names should be prefixed with Unreal prefix, then optionally the module prefix (OUU or project
// prefix -> see [naming.modules]).
// The unreal prefix depends on the type:
// - T: Template classes and structs T.
// - C: Concept traits -> see Template/Models.h
// - U: Classes that inherit from UObject
// - A: Classes that inherit from AActor
// - S: Classes that inherit from SWidget
// - I: Interface classes -> see [naming.iinterface]
// - F: All other classes -> same as structs -> see [naming.struct]
class OUUCODINGSTANDARD_API AOUUExampleCharacter
	: public ACharacter
	, public IOUUExampleColorableInterface
{
	// [order.members] Within any class/struct, members must be sorted by the following criteria in order:
	// 1. Category (see x. items below)
	// 2. Access Level -> see [order.access]
	// 3. Subcategory (see x.x items below)
	//
	// Categories are defined as follows:
	// 1. Generated body macros -> see [macro.genbody]
	// 2. Types, aliases and constants
	//     2.1 Enums
	//     2.2 Structs
	//     2.3 Classes
	//     2.4 Aliases
	//     2.5 Constants
	// 3. Constructors, destructors
	//     3.1 Default constructor
	//     3.2 Custom constructor
	//     3.3 Copy/move constructors
	//     3.4 Destructor
	// 4. Member functions
	//     4.1 Non-static, non-overridden, non-operator member functions
	//     4.2 Operators
	//     4.3 Static functions
	// 5. Interface functions / Overriden virtual functions
	//    Each base class counts as its own category (here: UObject, AActor, APawn, ACharacter,
	//    IExampleColorableInterface) -> see [class.overrides]
	// 6. Fields
	//     6.1 Constant fields
	//     6.2 Non-constant fields

	// [macro.genbody] Use GENERATED_BODY() instead of the deprecated GENERATED_UCLASS_BODY(),
	// GENERATED_IINTERFACE_BODY(), etc.
	GENERATED_BODY()

		// [order.access] Order and group members by access level: public > protected > private.
		// Each group requires unique access declarations even if the access level is the same as the previous group.
		// This is to prevent accidental access level changes.
		// Always add an initial access level declaration.
		// Do not rely on implicit access (i.e. struct = public, class = private).
public:
	// [alias.using] Use using declarations for aliasing instead of typedefs
	// [naming.alias] Adopt the prefix of the aliased type, except for the following exceptions:
	// - template type arguments -> see [naming.template.alias]
	// - concrete template instantiations -> see [naming.alias.template.instance]
	using FCharacterData = OUU::CodingStandard::FNumericAwesomeness;

	// [naming.alias.template.instance] When aliasing a template instance, use F as prefix, e.g.
	using FCharacterMeshPtr = TWeakObjectPtr<USkeletalMeshComponent>;

public:
	DECLARE_EVENT_OneParam(AOUUExampleCharacter, FOnAwesomenessChanged, EAwesomenessLevel);
	FOnAwesomenessChanged OnAwesomenessChanged;

public:
	// [naming.constant] Constants are prefixed with k_, both as members, globals or locally in functions.
	// [member.constant.primitive] Primitive constants should be declared as constexpr, if possible.
	// Prefer this any time over defines, c-style enums or static const values that are defined in cpp.
	static CONSTEXPR int32 k_NumBodyParts = 2;

	// [member.constant.complex] Complex constants (like FNames) that cannot be declared as constexpr should be declared
	// like this:
	static const FName k_HeadBodyPartName;
	static const FName k_TorsoBodyPartName;

private:
	struct FNestedStruct
	{
		// ...
	};

public:
	AOUUExampleCharacter();

public:
	// [member.accessor] Prefer declaring accessor functions (getters + setters) over making member field public.
	EAwesomenessLevel GetAwesomenessLevel() const;
	void SetAwesomeness(int32 _Awesomeness);

	// Checks if all possible colors are assigned to this character in any body part
	bool HasAllColorsPossible() const;

protected:
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
	UFUNCTION()
		void HandleOwnAwesomenessChanged(EAwesomenessLevel _Awesomeness);

	UFUNCTION()
		void OnRep_Score(int32 _ReplicatedScore);

	// [member.order.overrides] Overridden functions are grouped by the class where the function was first declared.
	// Each group must start with a comment indicating the originating parent class.
	// That is the parent class where the function was first declared.

	// [member.virtual] Overriden virtual functions must be marked as either override or final.
	// Do NOT use the virtual keyword.

	// [doc.member.virtual] Virtual overrides should not need to be documented as the API should be consistent with
	// the initial declaration.

	// -- AActor interface
public:
	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type _EndPlayReason) override;

	// -- IOUUExampleColorableInterface
public:
	bool ColorBodyPart(FName _BodyPartName, EOUUExampleBodyPartColor _BodyPartColor) override;

public:
	// [doc.delegate.instance] Do not duplicate parameter docs for delegate at the instance.
	// Instad, focus on the things that changes between instances.

	// This is called for any body part that is re-colored.
	// This delegate is called before the more specific OnXYColorChanged events below.

	// [naming.delegate.instance] Delegate instances are prefixed with 'On'. The name must be an event in past tense.
	UPROPERTY(BlueprintAssignable)
		FOnExampleColorablePartColorChanged OnBodyPartColorChanged;

	// [doc.omit] Omit doc comments that are obvious from the type/member naming.
	// Restating the obvious just wastes space and time.
	UPROPERTY(BlueprintAssignable)
		FOnExampleColorablePartColorChanged OnHeadColorChanged;

	UPROPERTY(BlueprintAssignable)
		FOnExampleColorablePartColorChanged OnTorsoColorChanged;

protected:
	// [naming.func.onrep] Functions bound to property replication events are named 'OnRep_' + VariableWithoutPrefix.
	UPROPERTY(ReplicatedUsing = OnRep_Score)
		int32 m_Score = 0;

private:
	// [member.init] Initialize member via assignment, unless it's a default constructible struct
	UPROPERTY(VisibleAnywhere)
		EOUUExampleBodyPartColor m_HeadColor = EOUUExampleBodyPartColor::Red;

	UPROPERTY(VisibleAnywhere)
		EOUUExampleBodyPartColor m_TorsoColor = EOUUExampleBodyPartColor::Red;

	// [naming.bool] Boolean variables and member fields should not be prefixed with b as in the Epic conventions.
	// Use verb as name with prefixes such as is, has, can or similar.
	// Should be in positive form to avoid double negatives or even triple negatives.
	bool m_WasColorChanged = false;

	FCharacterData m_CharacterData;

	// [nullptr] Use nullptr instead of NULL macro or 0 literal in all cases.
	// [naming.pointers] Pointers and pointer-like objects (i.e. smart pointers and everything else that can be null)
	// have an additional 'p' prefix after the context prefix, e.g. 'm_p'.
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* m_pHeadMeshComponent = nullptr;

	FDelegateHandle m_BoundDelegateHandle;
};

//---------------------------------------------------------------------------------------------------------------------
UCLASS()
class UOUUExampleBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// [doc.bp_func_lib] Always add a category for blueprint function library functions, so they are grouped properly in
	// the Blueprint editor. Use Plugin|Class nesting.
	UFUNCTION(BlueprintPure, Category = "OUUCodingStandard|Awesomeness")
		static int32 GetAwesomenessThreshold();
};
