# NameGen

[![Ubuntu](https://github.com/Galfurian/namegen/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/namegen/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/namegen/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/namegen/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/namegen/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/namegen/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/namegen/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/namegen/actions/workflows/documentation.yml)

`namegen` is a C++ library designed to generate random names based on customizable patterns. It supports a variety of tokens that can be used to create names for characters, places, and other creative uses in fantasy settings. The library provides flexibility for generating names with syllables, vowels, consonants, titles, and more, with support for capitalization and grouping.

## Features

- **Random Name Generation**: Generate random names based on a pattern that includes customizable tokens.
- **Flexible Pattern Matching**: Use predefined tokens or create your own patterns.
- **Title Handling**: Generate randomized titles with random prefixes and suffixes.
- **Capitalization Support**: Apply capitalization to specific parts of the name.
- **Pattern Parsing**: Supports complex patterns with groups and random selections.
- **Lightweight & Header-Only**: Easy to integrate into any C++ project with no external dependencies.

## Getting Started

### Installation

This is a header-only library. To use it, simply clone the repository and
include the header file in your project.

```bash
git clone https://github.com/Galfurian/namegen.git
```

Then include the header file in your code:

```cpp
#include "namegen/namegen.hpp"
```

### Prerequisites

- C++11 or higher compiler.
- Basic knowledge of C++ for pattern customization and token handling.

## Library Contents

The library is organized into the following components:

- **`namegen.hpp`**: Main header file that contains the `namegen::generate()` function.
- **`detail/`**: Internal namespace that holds helper functions for token generation and pattern processing.

## Example Usage

### Basic Name Generation

```cpp
#include "namegen/namegen.hpp"

int main() {
    std::cout << "Name: " << namegen::generate("!ssV'!i", 12345) << std::endl;
    return 0;
}
```

Possible output:

```bash
Name: Ashasei'Grumble
```

### Title Generation

```cpp
#include "namegen.hpp"

int main() {
    std::cout << "Title: " << namegen::generate("tT", 12345) << std::endl;
    return 0;
}
```

Possible output:

```bash
Title: Master of the Forgotten
```

## Token Explanation

The following tokens are used for generating random parts of the name:

- **s**: Generic syllable: Randomly selects a generic syllable from the list.
- **v**: Vowel: Randomly selects a vowel (a, e, i, o, u, y).
- **V**: Vowel or vowel combination: Randomly selects a vowel or a combination of vowels.
- **c**: Consonant: Randomly selects a consonant (b, c, d, f, g, h, etc.).
- **B**: Consonant or consonant combination suitable for the beginning of a word: Randomly selects a consonant or a combination of consonants suitable for word beginnings (e.g., "br", "ch", "fl").
- **C**: Consonant or consonant combination suitable anywhere in a word: Randomly selects a consonant or a combination of consonants that can appear anywhere in a word.
- **i**: Insult: Randomly selects from a list of humorous or derogatory words (e.g., "idiot", "doof").
- **m**: Mushy name: Randomly selects from a list of cute or affectionate names (e.g., "honey", "cutie").
- **M**: Mushy name ending: Randomly selects from a list of affectionate endings (e.g., "boo", "kins").
- **D**: Consonant suited for a "stupid" person's name: Randomly selects consonants that are typically used in silly or comedic names.
- **d**: Syllable suited for a "stupid" person's name (typically begins with a vowel): Randomly selects syllables that are common in names meant to sound funny or quirky.
- **t**: Randomized beginnings of titles: This token selects a title prefix like "Master of", "Ruler of", "Teacher of", "Conqueror of", etc.
- **T**: Randomized endings of titles: This token selects a title suffix like "the Endless", "the Sea", "the Fiery Pit", etc.

## Group Handling

- **Parentheses `()`**: Characters inside parentheses are emitted literally.
  - Example: `"s(dim)"` generates a random syllable followed by the literal "dim".
- **Angle Brackets `<>`**: Characters inside angle brackets are replaced with tokens from the list.
  - Without a vertical bar `|`, it picks one random token from the list (e.g., `"<s>"` generates a random syllable).
  - With a vertical bar `|`, it allows random selection between options (e.g., `"<s|v>"` generates either a syllable or a vowel).
  - Example: `"<s|v>"` will randomly choose between a syllable or a vowel.
  - Example: `"<c|v|>"` will choose a consonant, a vowel, or nothing at all.

## Capitalization

- The exclamation mark `!` means to capitalize the following component.
  - Example: `"!(foo)"` generates "Foo".
  - Example: `"v!s"` generates a lowercase vowel followed by a capitalized syllable (e.g., "eRod").

## License

`namegen` is released under the MIT License.
