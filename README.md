# Open Unreal Coding Standard

![Open Unreal Coding Standard logo](./Resources/oucpp_wide.png)

## Intent
The coding standard reflects my personal views and what I try to enforce for personal and commercial projects I work on.
As such, this plugin evolves over time. Both in what rules it contains, what it focuses on and how it is structured / conveyed.

It has served me well as a template for updating the coding standards at Grimlore Games but was also influenced by my work there,
so some of the rules first introduced in 2022 were now struck from the conventions.

- I generally try to avoid injecting purely subjective items that are solely based on personal preferences

## Methodology
The coding standard consists of 'live' source code files that are checked in along with the project and participates in compilation.
They serve as example for developers and are always available in the IDE during development.

Every rule of the standard is marked in comments with ``[tags]`` that can be referenced in chat and code reviews.
For example the following source code sample might receive a code review comment as follows:

```c++
class FFoo
{
	FFoo(){}
};
```

> As per ``[ctor.default]`` the constructor should be declared as ``FFoo() = default;``

This approach is losely inspired by [Splash Damage Coding Standard](https://github.com/splash-damage/coding-standards),
while the actual rules are based on a mix of
- [Epic Games Coding Standard](https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/index.html) (90% match)
- Aesir Interactive C++ Coding Conventions (90% match)
- Grimlore Games C++ Coding Conventions (80% match)

## Code Style
Rules about the visual formatting and naming of code are all prefixed with `STYLE` and contained within [STYLE.md](STYLE.md).

Any other rules in the source code files should be steadfast rules and recommendations that result in good clean code, but not necessarily code formatted in the exact way I format MY code.

This includes rules about capitalization, comment conventions, whitespace indentation, etc.

This separation has two major reasons:
- Very few descrepancies between my personal rules and Grimlore rules both of which have slightly different styles to format/order code files and make them pretty, but 99% shared rules about code structure, feature usage, etc.

There's some rules that fall in a gray area but which I would argue still make the cut:
```c++
// [basic.language] Use US English for all names and comments
```

## Limitations
The methodology described above has a couple major downsides: 
1. Structuring the files is HARD, because they attempt to fulfill two contradicting paradigms:
- The source code order required by the compiler and recommended by me to understand code
- The order in which it would be easiest to comprehend the rules imposed by the standard
In some ideal cases both of those goals overlap, because code legibility for a first time reader is one (but not the only!) goal of the rules.

2. Files in this plugin have far more comments than actual lines of code making them unrepresentative of what I would like actual production code to look like. For that you're better off looking at the OpenUnrealUtilities or OUUTags plugins which mostly follow these rules and are both used in production.

## Template
This coding standard has some "template fields", i.e. parts that just mention ``AUTHOR``, or ``STUDIO``.
You'll probably also want to replace ``OUU`` as prefix with your own studio prefix (e.g. we used ``Grim`` for _Grimlore Games_). If you search for those specific strings, you'll find all places that need replacement.

In a similar fashion, you'll need to update the copyright notice ``Copyright (c) 2022 Jonas Reich`` to match your own studio's copyright notice.

## License
Feel free to copy/remix this into your own thing. The whole coding standard is licensed under the [MIT License](LICENSE.md).
