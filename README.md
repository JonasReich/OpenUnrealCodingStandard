# Open Unreal Coding Standard

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

## Template
This coding standard has some "template fields", i.e. parts that just mention ``AUTHOR``, or ``STUDIO``.
You'll probably also want to replace ``OUU`` as prefix with your own studio prefix (e.g. we used ``Grim`` for _Grimlore Games_). If you search for those specific strings, you'll find all places that need replacement.

In a similar fashion, you'll need to update the copyright notice ``Copyright (c) 2022 Jonas Reich`` to match your own studio's copyright notice.
