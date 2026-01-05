# Open Unreal Coding Style

The following rules are __style__ rules, meaning they are concerned with the visual layout of code to make it pretty/consistent and therefore more easily readable.

These rules are __highly subjective__, so they are only included as reference and not expected to be copied over / adopted by anyone elses projects. In my dayjob at Grimlore Games we have different naming / indentation rules than this.

Originally, all of these rules were part of the main coding standard / source code files which sticks to them, which is why they were extracted as code snippets below.

For reference, the following naming rules are still in the main (non style) ruleset, because I think they all have some rational objective justification:

- naming
    - naming.identifiers
    - naming.prefixes
    - naming.func.onrep
    - naming.func.rpc
    - naming.cvar
- documentation
    - basic.doc
    - doc.member.virtual
    - doc.delegate.instance
- ordering
    - namespace.nesting
    - order.overrides
    - order.inline_funcs
    - order.cpp_mirror_h

## General Style

```c++
// [STYLE.copyright] Every code file must start with the following copy right notice
// Copyright (c) 2022 Jonas Reich

// [STYLE.basic.language] Use US English for all names and comments.
//  BAD:    FColor SpezialisierteFarbe;
//  BAD:    FColor SpecialisedColour;
//  GOOD:   FColor SpecializedColor;

// [STYLE.basic.format] Auto-format all of your source-files using the clang-format files and application provided with the
// project. Any rules enforced by clang-format are omitted from this file.
// This includes the everlasting tabs-or-spaces debate over indentation. Each plugin can enforce what they want, stick
// with it and adjust your IDE display settings to make it pretty to your eyes.

// [STYLE.dividers] These divider lines may be used in long header files, but generally their presence is a sign of breaking
// the [basic.files] rule. If a cpp file contains function definitions for multiple classes, use dividers to separate the
// classes and add class headers
//---------------------------------------------------------------------------------------------------------------------
```

## Naming
```c++
// [STYLE.naming.case]
// Identifiers should all use PascalCase_WithUnderscores, with an exception for MACROS, which should be
// ALL_CAPS_WITH_UNDERSCORES

// [STYLE.include.quotes] Use "double quotes" for include paths instead of <angled brackets>.

// [STYLE.naming.typenames.prefixes]
// The type prefixes used by Epic should be used in plugin code even if not enforced by UHT
// i.e. E prefix for enum classes, S for slate widgets, F for structs and non-UObject classes

// [naming.bool] Boolean variables and member fields are prefixed with b as in the Epic conventions.
// Use verb as name with prefixes such as is, has, can or similar.
// Should be in positive form to avoid double negatives or even triple negatives.

// [STYLE.naming.func.param.in] Optionally prefix function input parameters with 'In' to distinguish them from locals and
// members.
FString LexToString(EAwesomenessLevel InAwesomenessLevel);

// [STYLE.naming.func.param.out] Always prefix out-by-ref-parameters with 'Out'.
OUUCODINGSTANDARD_API bool TryLexFromString(EAwesomenessLevel& OutAwesomenessLevel, const FString& String);

// [STYLE.naming.delegate.type] Delegate types have two permissible naming schemes:
// 1. FFooEvent -> Used for generic groups of events that share a common signature, e.g. FFileOperationEvent
// 2. FOnFoo -> Used for single purpose events, matching the delegate instance name, e.g. FOnActorDestroyed
DECLARE_DELEGATE(FFileOperationEvent);
DECLARE_DELEGATE(FOnActorDestroyed);

// [STYLE.naming.delegate.instance] Delegate instances are prefixed with 'On'. The name must be an event in past tense.

// [STYLE.naming.delegate.func] Functions that are bound to delegates are called 'Handle' + optional object hint +
// Delegate name without 'On' prefix, e.g. this->OnAwesomenessChanged becomes HandleOwnAwesomenessChanged
```

## Documentation
```c++
// [STYLE.doc.omit] Omit doc comments that are obvious from the type/member naming.
// Restating the obvious just wastes space and time.

// [STYLE.doc.enum] Commenting enum cases is optional. Decide on case-by-case basis.
// When commenting enum cases, place the comment above the case label, not behind it.
// [STYLE.doc.interface.uclass] The UInterface does not need a type comment, as it's mainly required for the reflection data.
// [STYLE.doc.interface.iclass] The IInterface should receive the regular type-doc treatment.

// [STYLE.doc.function] Function declarations should also be documented with multiline type-doc comments.
// Document parameters and return values with @param and @return(s) notation respectively.
/**
 * Color a body part by name.
 * @param		BodyPartName	Name ID of the body part to be colored.
 * @param		BodyPartColor	Color preset to apply to the body-part.
 * @returns		true if the body part was found and successfully colored.
 */
UFUNCTION(BlueprintCallable)
virtual bool ColorBodyPart(FName BodyPartName, EOUUExampleBodyPartColor BodyPartColor) = 0;
```

## Structs vs Classes
```c++
// [STYLE.struct.functions] Structs may only have constructor, operator and conversion functions.
// If it gets any more complicated than that, you should declare them as class instead.
```

## Namespaces
```c++
// [STYLE.naming.namespaces]
// Use the following namespace structure: OUU::ModuleName or OUU::ModuleName::Private
namespace OUU::CodingStandard
{
	// ...
} // namespace OUU::CodingStandard
// [STYLE.namespace.end] Add a namespace end comment. This is automatically done by clang-format, but it fails to update
// renames, so you should keep an open eye.

```

## Templates
```c++
// [STYLE.naming.template.type] Template types should always be prefixed with T
// [STYLE.naming.template.param] All template parameters should be prefixed with 'In' and exposed to users via using
// declaration
// [STYLE.naming.template.paramtype] Parameter types should be suffixed with 'Type' or 'Types' in case of a parameter pack
template <typename InElementType, typename InAllocatorType, int32 InDefaultSlack>
struct TMyContainer
{
public:
	// [STYLE.naming.template.alias] No type prefix needed for the aliases.
	using ElementType = InElementType;
	using AllocatorType = InAllocatorType;
	static constexpr int32 DefaultSlack = InDefaultSlack;

	static_assert(DefaultSlack >= 8, "A default slack size of 8 or more is required, because xyz");
};

```

## Aliases
```c++
// [STYLE.naming.alias] Adopt the prefix of the aliased type, except for the following exceptions:
// - template type arguments -> see [naming.template.alias]
// - concrete template instantiations -> see [naming.alias.template.instance]
using FCharacterData = OUU::CodingStandard::FNumericAwesomeness;

// [STYLE.naming.alias.template.instance] When aliasing a template instance, use F as prefix, e.g.
using FCharacterMeshPtr = TWeakObjectPtr<USkeletalMeshComponent>;
```

## Ordering
```c++
// [STYLE.order.h] Header files are ordered roughly like this:
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

// [STYLE.order.members] Within any class/struct, members must be sorted by the following criteria in order:
// 1. Access Level -> see [order.access]
// 2. Nested categories (see items below)
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

// [STYLE.order.access] Order and group members by access level: public > protected > private.
// In total a class should therefore have max three access level specifiers.
// Always add an initial access level declaration after GENERATED_BODY().
// Do not rely on implicit access (i.e. struct = public, class = private).

// [STLYE.order.macro.impl] Implementation macros (e.g. log categories, modules) should come before any other implementations

// [STYLE.order.cvar] Console variables should appear towards the top of the file, in the same groups as constants. For the most part, game code will treat them the same way, and it's easier to find the cvar declarations that way.
```
