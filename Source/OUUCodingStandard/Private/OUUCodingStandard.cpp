// Copyright (c) 2022 Jonas Reich
// [source.copyright] Every source file must start with the copy right notice above

// @STUDIO_FILE: <description>

// [markup.engine.file] If this file is being added to an engine location, mark this
// with the above comment at the start of the file. This removes the need for the
// STUDIO engine guards around modifications in the file -> see [markup.engine]

// [order.cpp] Source files should match the header order as much as possible.
// Symbols that are only declared in the source files should be avoided or put into the following order:
// - includes
// - source declarations + inline definitions of source declarations
// - definitions of header declarations

// [resharper.disables] These disables hide some of the ReSharper static code inspections in this file.
// They are not a good example for responsible coding.
// ReSharper disable CppDeclaratorNeverUsed
// ReSharper disable CppLambdaCaptureNeverUsed

// [cpp.include.header] Always include the header file corresponding to your cpp file first.
#include "OUUCodingStandard.h"

#include "Modules/ModuleManager.h"
#include "Net/UnrealNetwork.h"

// [order.macro.impl] Implementation macros (e.g. log categories, modules) should come before any other implementations
IMPLEMENT_MODULE(FDefaultModuleImpl, OUUCodingStandard)
DEFINE_LOG_CATEGORY(LogOUUCodingStandard);

// [cpp.namespace.private] use a namespace to wrap free functions defined only in the cpp file.
// Default naming for such a namespace would be ModulePrefix::ModuleName::Private, but you are free to diverge.
namespace OUU::CodingStandard::Private
{
	// [globals] Global variables are generally not permitted. The only exception to this are console variables.
	// But even they should be put into a namespace like here.

	// [order.cvar] Console variables should appear towards the top of the file, in the same groups as constants.
	// For the most part, game code will treat them the same way, and it's easier to find the cvar declarations that
	// way. [naming.cvar] Every console variable should start with an appropriate prefix. Use the built-in cvars as
	// reference.
	// - r: render
	// - s: scalability
	// - a: animation
	// - vt: virtual texture
	// - ... etc
	// The C++ variable itself should be prefixed with CVar_
	TAutoConsoleVariable<int32> CVar_MinAwesomeness(
		TEXT("ouu.CodingStandard.MinAwesomeness"),
		100,
		TEXT("Sample cvar that defines the minimum int value above 0 at which true awesomeness starts."));

	// [doc.namespace] Namespaces do not need doc comments at the beginning, but ending braces should be followed by a
	// matching comment like this (will be auto-enforced by clang-format).
} // namespace OUU::CodingStandard::Private

namespace OUU::CodingStandard::Private::IsolatedSamples
{
	// [func.retval.noconst] Returning const values from functions is always bad. See functions below.
	class FConstReturnValues
	{
		// Bad - returning a const array
		// May inhibit copy-elision.
		// Return non-const new value or const ref to existing member is always better.
		const TArray<FString> GetSomeArray_A();

		// Fine - returning a reference to a const array
		// -> user can decide whether to read existing array elements or create a working copy.
		const TArray<FString>& GetSomeArray_B();

		// Bad - returning a const pointer to a const array -> overly verbose with no practical difference
		// ReSharper disable once CppConstValueFunctionReturnType
		const TArray<FString>* const GetSomeArray_C() { return nullptr; }

		void GetSomeArray_C_NoBenefit()
		{
			// Proof that GetSomeArray_C() does not benefit from making return type const:
			// This compiles just fine, because the const pointer is simply copied into a new non-const pointer.
			const TArray<FString>* Ptr = GetSomeArray_C();
		}

		// Fine - returning a pointer to a const array
		const TArray<FString>* GetSomeArray_D();
	};

	// [func.param.types] Prefer simple to understand ways of passing parameters. When using an "unusual and clever" way
	// to pass a parameter, document the reason (e.g. don't add explicit support for move semantics without good
	// reason).
	//
	// The C++ Core Guidelines F.15 make a valuable distinction between the following three kinds of C++ types:
	// - A) Cheap or impossible to copy type (e.g. int, FUnqiuePtr<T>)
	// - B) Cheap to move (e.g. TArray<T>, FString) OR moderate cost to move (e.g. TArray<TMap<T, U>>, BigPOD) OR don't
	// know
	// - C) Expensive to move (e.g. BigPOD[])
	//
	// For all types without explicit move support, you can stick to the following reference table.
	// Document decisions whenever you pick a different approach!
	//
	// | parameter functionality | Type A | Type B                     | Type C      |
	// |-------------------------|--------|----------------------------|-------------|
	// | Out                     | X f()  | X f()                      | f(X&)       |
	// | In/Out                  | f(X&)  | f(X&)                      | f(X&)       |
	// | In & read only          | f(X)   | f(const X&)                | f(const X&) |
	// | In & retain copy        | f(X)   | f(const X&)                | f(const X&) |
	//
	// When you really want to add explicit move semantics support to your type, use the following reference instead.
	//
	// | parameter functionality | Type A | Type B                      | Type C      |
	// |-------------------------|--------|-----------------------------|-------------|
	// | Out                     | X f()  | X f()                       | f(X&)       |
	// | In/Out                  | f(X&)  | f(X&)                       | f(X&)       |
	// | In & read only          | f(X)   | f(const X&)                 | f(const X&) |
	// | In & retain copy        | f(X)   | f(const X&) + f(X&&) & move | f(const X&) |
	// | In & move from          | f(X&&) | f(X&&)                      | f(X&&)      |
	//
	// See
	// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f15-prefer-simple-and-conventional-ways-of-passing-information

	// [cpp.divider] These divider lines may be used in long cpp files to give some more visual structure to the file.
	// Using them is not mandatory, but you should stay consistent - at least within a single file.
	//---------------------------------------------------------------------------------------------------------------------
	void Auto()
	{
		// [auto] almost always auto: Prefer using auto especially in cases where...
		// - the type can be easily inferred from context (e.g. cast results)
		// - the exact type is not important (e.g. because it's only forwarded to another function)
		// - spelling out the type name would be too verbose (e.g. template/iterator types)

		// type is already explicit on right side of assignment
		auto* pMeshComponent = NewObject<USkeletalMeshComponent>();

		// no need to know specific type of pMaterial
		auto* pMaterial = pMeshComponent->GetMaterial(0);
		pMeshComponent->SetMaterial(1, pMaterial);

		// Explicit type of Iterator is
		// TIndexedContainerIterator<const TArray<UMaterialInterface *>, UMaterialInterface *const, int32>
		auto Iterator = pMeshComponent->GetMaterials().CreateConstIterator();

		// [auto.init] auto forces to have initialization - this is always good

		// [auto.numeric] However the following requires being careful with literal suffixes.
		// More explicit variable declaration as int32, uint32, etc. may be preferred to enforce storage size.
		{
			auto SignedInt_Maybe32Bit = 42;
			auto UnsignedInt_Maybe32Bit = 42u;
			auto Float = 42.f;
			auto Double = 42.0;
			auto LongLong = 42ll;
		}
		{
			int32 SignedInt_Maybe32Bit = 42;
			uint32 UnsignedInt_Maybe32Bit = 42u;
			float Float = 42.f;
			double Double = 42.0;
			int64 LongLong = 42ll;
		}

		// [auto.qualifiers] ALWAYS mark auto with the appropriate qualifiers: const, &, * - even if it's superfluous
		int32 Value;
		int32* Pointer = &Value;
		const int32& Reference = Value;
		auto& ProperReference = Value;
		auto& EnforcedReference = Reference;
		auto* ExplicitPointer = Pointer;
		const auto& ExplicitReference = Reference;

		// BAD even if it works: It's not visible that this is a pointer
		auto HiddenPointer = &Value;
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Lambdas()
	{
		struct FFoo
		{
		} ExternalFoo;

		// [cpp.lambda.general] use lambda's to your advantage
		// especially when they will isolate work in the implementation
		// rather than pollute the interface with helper methods
		//
		// but don't abuse them - if their body becomes complex enough
		// extract into separate function/method

		// [lambda.dangling] biggest problem in production is creating dangling references
		// by capturing objects by reference that die before the lambda gets called
		auto dangling_lambda = [ExternalFoo]() {
			// will ExternalFoo still be valid when the lambda is called?
		};

		// [lambda.capture] don't capture 'this', by ref '&' or everything by value '='!
		// Instead use the named captures to cherry-pick (see below)
		// auto Lambda = [this]() {};
		// auto Lambda = [&]() {};
		// auto Lambda = [=]() {};

		// [lambda.capture.auto] Cheat sheet for capture type deduction:
		const int32 Original = 0;
		const int32& Reference = Original;
		auto lambda_auto = [Original,
							ExplicitCopy = Original,
							&ProperReference = Original,
							AccidentalCopy = Reference,
							AccidentalPointer = &Reference]() {
			// ConstCopy -> const int32
			// ExplicitCopy -> int32
			// ProperReference -> const int32 &
			// AccidentalCopy -> int32
			// AccidentalPointer -> const int32 *
		};
	}

	//---------------------------------------------------------------------------------------------------------------------
	void NumericValues()
	{
		// [numeric.types] Use Unreal aliases for guaranteed sized integers, i.e. int32, uint32, int64, uint64, etc.
		int32 SignedInt32Bits = 0;
		uint64 UnsignedInt64Bits = 0lu;

		// [numeric.limits] Use TNumericLimits instead of #defines such as FLT_MAX
		// -> see http://api.unrealengine.com/INT/API/Runtime/Core/Math/TNumericLimits/
		// e.g. For all floating point types
		// ReSharper disable CppDeclaratorNeverUsed
		constexpr float MaxPositiveFloatValue = TNumericLimits<float>::Max();
		constexpr float MinPositiveFloatValue = TNumericLimits<float>::Min();
		constexpr float MinNegativeFloatValue = TNumericLimits<float>::Lowest();
		// E.g. for integral types
		constexpr int32 MaxPositiveIntValue = TNumericLimits<int32>::Max();
		// This is the same as Lowest() for all integral types.
		constexpr int32 MinNegativeIntValue = TNumericLimits<int32>::Min();
		// ReSharper restore CppDeclaratorNeverUsed
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Macros()
	{
		// [macro] Macros should be avoided at all costs.
		// Keep their scope as small as possible (undef as soon as possible).

		// [naming.macro] Use UPPER_SNAKE_CASE for macro names.
#define LOCAL_MACRO(x) (42 + x)
		int32 LocalInt = LOCAL_MACRO(0);
#undef LOCAL_MACRO
	}
} // namespace OUU::CodingStandard::Private::IsolatedSamples

// [namespace.func.impl] Create namespace scopes in the cpp file instead of inlining the namespace name into the
// function signatures, e.g. here: FString OUU::CodingStandards::LexToString(EAwesomenessLevel AwesomenessLevel).
namespace OUU::CodingStandard
{
	//---------------------------------------------------------------------------------------------------------------------
	EAwesomenessLevel AwesomenessLevelFromIntValue(int32 Awesomeness)
	{
		// [earlyreturn] Try to use early-return where possible to reduce scope nesting.
		// This is the only case where omitting braces is permissible, unless the return value requires a line-break
		if (Awesomeness < 0)
			return EAwesomenessLevel::NotAwesome;

		// [magic.number] Do not use magic numbers. Instead, use named global constants or cvars.
		// if (Awesomeness < 100)
		if (Awesomeness < Private::CVar_MinAwesomeness.GetValueOnAnyThread())
			return EAwesomenessLevel::SemiAwesome;

		return EAwesomenessLevel::Awesome;
	}

	//---------------------------------------------------------------------------------------------------------------------
	FString LexToString(EAwesomenessLevel AwesomenessLevel)
	{
		// [switch.braces] Braces are optional around cases in switch/case blocks.
		// When placing braces around a case block, the final break or return statement is placed inside the brace
		// scope.
		switch (AwesomenessLevel)
		{
		case EAwesomenessLevel::NotAwesome:
			// [string.literal] String literals in production code should always use the TEXT() macro
			return TEXT("NotAwesome");
			// not to be confused with INVTEXT() macro for FText literals!
			// return INVTEXT("NotAwesome");
		case EAwesomenessLevel::SemiAwesome: return TEXT("SemiAwesome");
		case EAwesomenessLevel::Awesome: return TEXT("Awesome");
		default: return TEXT("<invalid>");
		}
	}

	//---------------------------------------------------------------------------------------------------------------------
	bool TryLexFromString(EAwesomenessLevel& OutAwesomenessLevel, const FString& String) { return false; }
} // namespace OUU::CodingStandard

// [cpp.divider.class] If a cpp file contains function definitions for multiple classes, place a separator
// in the following format above the first function definition of each class
//---------------------------------------------------------------------------------------------------------------------
// AOUUExampleCharacter
//---------------------------------------------------------------------------------------------------------------------
const FName AOUUExampleCharacter::HeadBodyPartName = TEXT("Head");
const FName AOUUExampleCharacter::TorsoBodyPartName = TEXT("Body");

//---------------------------------------------------------------------------------------------------------------------
AOUUExampleCharacter::AOUUExampleCharacter() : AOUUExampleCharacter(nullptr, EOUUExampleBodyPartColor::Red) {}

//---------------------------------------------------------------------------------------------------------------------
// [ctor.overload] You may overload the constructor, but should always forward parameters to one declaration if
// possible.
// [ctor.initialization] Member initialization should only happen in a single constructor. Other constructors should
// call the same delegating constructor to initialize any member variables.
AOUUExampleCharacter::AOUUExampleCharacter(USkeletalMesh* InSkeletalMesh, EOUUExampleBodyPartColor InHeadColor) :
	HeadColor(InHeadColor)
{
	HeadMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	// Attach the head mesh to the character mesh = body mesh
	HeadMeshComponent->SetupAttachment(GetMesh());
	HeadMeshComponent->SetSkeletalMesh(InSkeletalMesh);
}

//---------------------------------------------------------------------------------------------------------------------
EAwesomenessLevel AOUUExampleCharacter::GetAwesomenessLevel() const
{
	return CharacterData.GetAwesomenessLevel();
}

//---------------------------------------------------------------------------------------------------------------------
void AOUUExampleCharacter::SetAwesomeness(int32 Awesomeness)
{
	const auto AwesomenessLevelBefore = CharacterData.GetAwesomenessLevel();

	CharacterData = FCharacterData(Awesomeness, TEXT("set by SetAwesomeness"));
	const auto NewAwesomenessLevel = CharacterData.GetAwesomenessLevel();

	if (NewAwesomenessLevel != AwesomenessLevelBefore)
	// [braces.one_per_line] Follow "Allman" style aka one line per brace
	{
		OnAwesomenessChanged.Broadcast(NewAwesomenessLevel);
	}
}

//---------------------------------------------------------------------------------------------------------------------
bool AOUUExampleCharacter::HasAllColorsPossible() const
{
	// [enum.range.use] If you have functions like this that need to iterate over all possible cases of an enum,
	// you should declare the enum ranges statically -> see [enum.range.decl]
	for (const auto Color : TEnumRange<EOUUExampleBodyPartColor>())
	{
		if (HeadColor != Color && TorsoColor != Color)
			return false;
	}
	return true;
}

//---------------------------------------------------------------------------------------------------------------------
void AOUUExampleCharacter::BeginPlay()
{
	// [function.super] Always call the super function, or comment it out with an explicit reason why you omitted the
	// super-call. This is especially true for gameplay framework functions like BeginPlay(), Tick(), etc.
	Super::BeginPlay();

	// [log.temp] Never check-in code that uses LogTemp.
	// UE_LOG(LogTemp, Log, TEXT("AOUUExampleCharacter::BeginPlay"))

	// [log.verbosity] Status logs that are not interesting for the general developer should be checked-in with
	// 'Verbose' verbosity or higher.
	UE_LOG(LogOUUCodingStandard, Verbose, TEXT("AOUUExampleCharacter::BeginPlay - %s"), *GetName());

	// [naming.delegate.func] Functions that are bound to delegates are called 'Handle' + optional object hint +
	// Delegate name without 'On' prefix, e.g. this->OnAwesomenessChanged becomes HandleOwnAwesomenessChanged
	BoundDelegateHandle =
		this->OnAwesomenessChanged.AddUObject(this, &AOUUExampleCharacter::HandleOwnAwesomenessChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void AOUUExampleCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// [delegate.cleanup] Always clean up bound delegates
	this->OnAwesomenessChanged.Remove(BoundDelegateHandle);
	BoundDelegateHandle.Reset();
}

//---------------------------------------------------------------------------------------------------------------------
bool AOUUExampleCharacter::ColorBodyPart(FName BodyPartName, EOUUExampleBodyPartColor BodyPartColor)
{
	// [error.ensure] Prefer to use ensureMsgf and provide some context information over using ensure().
	// ensure(BodyPartColor != EOUUExampleBodyPartColor::Count);
	ensureMsgf(
		BodyPartColor != EOUUExampleBodyPartColor::Count,
		TEXT("%s - Count case must never be used to color body parts"),
		*GetName());

	// [error.check] Use checks with a message if you found some unrecoverable state that must always lead to a crash.
	// This should be avoided in most cases, but sometimes there is no other way out.
	// NOTE: In shipping builds this will ALWAYS lead to a crash.
	checkf(
		BodyPartColor != EOUUExampleBodyPartColor::Count,
		TEXT("%s - Cound case must never be used to color body parts"),
		*GetName());

	// [error.exceptions] Never use exceptions. Unreal does not support C++ exception handling.
	// throw std::invalid_argument("Count case must never be used to color body parts");

	// [markup.engine] If you change something in a pre-existing engine file, make sure you add the following
	// 'STUDIO' markup comments at the start and end of your change. Old engine code may be disabled with #if 0 guard.

	// STUDIO Start username: Description of the change -> Focus on the reasoning.
#if 0
	if (BodyPartName == HeadBodyPartName)
	{
		HeadColor = BodyPartColor;
	}
	else if (BodyPartName == TorsoBodyPartName)
	{
		TorsoColor = BodyPartColor;
	}
#endif
	// STUDIO End

	// [comment.todo] If you leave todo comments, start with #TODO, so we can find them and add a developer that should
	// take care of the todo.
	// #TODO username: Update mesh materials based on enum state
	bWasColorChanged = true;

	return false;
}

//---------------------------------------------------------------------------------------------------------------------
void AOUUExampleCharacter::Server_SendDataToServer_Implementation() {}

//---------------------------------------------------------------------------------------------------------------------
void AOUUExampleCharacter::Client_SendDataToClient_Implementation() {}

//---------------------------------------------------------------------------------------------------------------------
void AOUUExampleCharacter::Multicast_SendDataToEveryone_Implementation() {}

//---------------------------------------------------------------------------------------------------------------------
void AOUUExampleCharacter::HandleOwnAwesomenessChanged(EAwesomenessLevel Awesomeness) const
{
	// [braces.always] Always use braces, even for single line if-statements.
	// only exception: early-returns -> see [earlyreturn]
	if (Awesomeness == EAwesomenessLevel::Awesome)
	{
		UE_LOG(LogOUUCodingStandard, Log, TEXT("Character %s just became AWESOME!"), *GetName());
	}

	// ...for the specific case above you can use UE_CLOG as alternative:
	UE_CLOG(
		Awesomeness == EAwesomenessLevel::Awesome,
		LogOUUCodingStandard,
		Log,
		TEXT("Character %s just became AWESOME!"),
		*GetName());
}

//---------------------------------------------------------------------------------------------------------------------
void AOUUExampleCharacter::OnRep_Score(int32 ReplicatedScore) {}

//---------------------------------------------------------------------------------------------------------------------
// [func.replprops] This function is auto-declared by UHT for any AActor with replicated properties.
// Because we do not have a matching declaration in the header file, it should be implemented at the end of the list of
// member function definitions in the cpp file.
// The output parameter OutLifetimeProps must not be renamed, otherwise the DOREPLIFETIME macros do not work.
void AOUUExampleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AOUUExampleCharacter, Score);
}

//---------------------------------------------------------------------------------------------------------------------
// UOUUExampleBlueprintFunctionLibrary
//---------------------------------------------------------------------------------------------------------------------
int32 UOUUExampleBlueprintFunctionLibrary::GetAwesomenessThreshold()
{
	// Could be called from animation thread in animation blueprints, so any thread.
	return OUU::CodingStandard::Private::CVar_MinAwesomeness.GetValueOnAnyThread();
}
