/// @file namegen.hpp
/// @brief Fantasy name generator.
/// @details
/// The `generate()` function creates a name based on an input pattern. The
/// pattern consists of various characters representing different types of
/// random replacements. Everything else in the pattern is emitted literally.
/// The following tokens are supported for random generation:
///
/// @section Token Explanation
///
/// The following tokens are used for generating random parts of the name:
///
///  - **s**: Generic syllable: Randomly selects a generic syllable from the list.
///  - **v**: Vowel: Randomly selects a vowel (a, e, i, o, u, y).
///  - **V**: Vowel or vowel combination: Randomly selects a vowel or a combination
///    of vowels.
///  - **c**: Consonant: Randomly selects a consonant (b, c, d, f, g, h, etc.).
///  - **B**: Consonant or consonant combination suitable for the beginning of a
///    word: Randomly selects a consonant or a combination of consonants suitable
///    for word beginnings (e.g., "br", "ch", "fl").
///  - **C**: Consonant or consonant combination suitable anywhere in a word:
///    Randomly selects a consonant or a combination of consonants that can appear
///    anywhere in a word.
///  - **i**: Insult: Randomly selects from a list of humorous or derogatory words
///    (e.g., "idiot", "doof").
///  - **m**: Mushy name: Randomly selects from a list of cute or affectionate names
///    (e.g., "honey", "cutie").
///  - **M**: Mushy name ending: Randomly selects from a list of affectionate
///    endings (e.g., "boo", "kins").
///  - **D**: Consonant suited for a "stupid" person's name: Randomly selects
///    consonants that are typically used in silly or comedic names.
///  - **d**: Syllable suited for a "stupid" person's name (typically begins with
///    a vowel): Randomly selects syllables that are common in names meant to sound
///    funny or quirky.
///  - **t**: Randomized beginnings of titles: This token selects a title prefix
///    like "Master of", "Ruler of", "Teacher of", "Conqueror of", etc.
///  - **T**: Randomized endings of titles: This token selects a title suffix
///    like "the Endless", "the Sea", "the Fiery Pit", etc.
///
/// @section Group Handling
///
/// Characters between parentheses `()` are emitted literally. For example, the
/// pattern "s(dim)" emits a random generic syllable followed by the literal
/// text "dim".
///
/// Characters between angle brackets `<>` emit patterns from the table above.
/// If there is no vertical bar `|`, the pattern inside the angle brackets is
/// emitted directly as a token from the table (e.g., `"<s>"` will emit a random
/// syllable).
/// If the pattern contains a vertical bar `|`, it allows for random selection
/// between the options separated by the bar. For example, `"<s|v>"` will
/// randomly select either a syllable or a vowel.
///
/// Empty groups are allowed. For example, `"<c|v|>"` will emit either a
/// consonant, a vowel, or nothing at all.
///
/// @section Capitalization
///
/// An exclamation point `!` means to capitalize the component that follows it. For example:
/// - `"!(foo)"` will emit "Foo".
/// - `"v!s"` will emit a lowercase vowel followed by a capitalized syllable, like "eR".
///

#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>

#include <json/json.hpp>

enum : unsigned char {
    NAMEGEN_MAJOR_VERSION = 1, ///< Major version of the library.
    NAMEGEN_MINOR_VERSION = 1, ///< Minor version of the library.
    NAMEGEN_MICRO_VERSION = 0  ///< Micro version of the library.
};

/// @brief Main namespace.
namespace namegen
{

/// @brief The type used to store the seeds.
using seed_t = std::random_device::result_type;

/// @brief Contains support functions.
namespace detail
{

using key_t        = char;                     ///< Type alias for a character key used in token processing.
using token_list_t = std::vector<std::string>; ///< Type alias for a list of tokens (strings).
using token_map_t  = std::unordered_map<key_t, token_list_t>; ///< Type alias for a map of tokens.

/// @brief Global map to store tokens loaded from JSON files.
static token_map_t token_map;

/// @brief Define the option_t struct that encapsulates all the state options.
struct option_t {
    bool capitalize   = false;        ///< Capitalization flag for the next item
    bool emit_literal = false;        ///< We are emitting literal character.
    bool inside_group = false;        ///< Indicates whether we're inside a < > group
    seed_t seed       = 0UL;          ///< Keeps track of the seed.
    std::string current_option;       ///< Holds the current option being processed in the group.
    std::vector<std::string> options; ///< Collects options inside a group.
};

/// @brief Retrieves the tokens corresponding to a key.
/// @param key The key to retrieve the tokens for.
/// @return The list of tokens for the specified key.
inline auto get_tokens(char key) -> const std::vector<std::string> &
{
    if (token_map.find(key) != token_map.end()) {
        return token_map[key];
    }
    static std::vector<std::string> empty_tokens;
    return empty_tokens;
}

/// @brief Returns a random number.
/// @param seed the seed used to generate the random number, it is modified.
/// @param min the lower bound for the random number.
/// @param max the upper bound for the random number.
/// @return a random number between min and max.
template <typename T> inline auto get_rand(seed_t &seed, T min, T max) -> T
{
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<T> distribution(min, max);
    return distribution(generator);
}

/// @brief Picks a random element from the given container of strings.
/// @param seed A reference to the seed value used to generate the random index. It is modified during the function call.
/// @param strings The container (a list or vector) holding the strings from which to pick a random element.
/// @return A constant reference to the randomly selected token from the container.
inline auto pick_random_element(seed_t &seed, const std::vector<std::string> &strings) -> const std::string &
{
    // Generate a random index between 0 and strings.size() - 1.
    std::size_t random_index = detail::get_rand<std::size_t>(seed, 0UL, strings.size() - 1);
    // Return the element at the randomly chosen index.
    return strings[random_index];
}

/// @brief Capitalizes the given character.
/// @param character the input caracter
/// @param capitalize controls if we should capitalize or not.
/// @return the capitalized character, if capitalize is true.
inline auto capitalize_and_clear(key_t character, bool &capitalize) -> key_t
{
    if (capitalize) {
        capitalize = false;
        return static_cast<key_t>(std::toupper(character));
    }
    return character;
}

/// @brief Processes a token based on the provided key and appends it to the buffer.
/// If the `emit_literal` flag is set, the key is added as a literal character.
/// Otherwise, a random token is selected based on the key and added to the buffer.
/// Capitalization is applied if specified in the options.
///
/// @param options The current state options that control token processing.
/// @param buffer The string buffer where the processed token will be appended.
/// @param key The key representing the type of token to process.
/// @return 1 on success, 0 otherwise.
inline auto process_token(option_t &options, std::string &buffer, key_t key) -> int
{
    auto tokens = detail::get_tokens(key);
    if (tokens.empty()) {
        buffer.push_back(detail::capitalize_and_clear(key, options.capitalize));
    } else {
        auto token = detail::pick_random_element(options.seed, tokens);
        if (token.empty()) {
            return 0;
        }
        auto it = token.begin();
        buffer.push_back(detail::capitalize_and_clear(*it++, options.capitalize));
        buffer.insert(buffer.end(), it, token.end());
    }
    return 1;
}

/// @brief Processes a character from the pattern and appends it to the buffer.
/// Handles special characters (e.g., `!`, `()`, `<>`) and processes them
/// accordingly. Regular characters are processed as tokens and added to the buffer.
///
/// @param options The current state options controlling character processing.
/// @param buffer The string buffer where the processed character will be appended.
/// @param character The character to process, which may be part of a token or a special symbol.
/// @return 1 on success, 0 otherwise.
inline auto process_character(option_t &options, std::string &buffer, key_t character) -> int
{
    switch (character) {
    case '(':
        if (options.inside_group) {
            options.current_option.push_back(character);
        } else {
            options.emit_literal = true;
        }
        break;
    case ')':
        if (options.inside_group) {
            options.current_option.push_back(character);
        } else {
            options.emit_literal = false;
        }
        break;
    case '<':
        options.inside_group = true;
        options.options.clear();
        options.current_option.clear();
        break;
    case '|':
        options.options.push_back(options.current_option);
        options.current_option.clear();
        break;
    case '>': {
        options.inside_group = false;
        options.options.push_back(options.current_option);
        options.current_option.clear();
        // Ensure there's at least one option in the group.
        if (options.options.empty()) {
            return 0;
        }
        // Randomly pick an option.
        auto option = detail::pick_random_element(options.seed, options.options);
        // Process and append the selected option.
        for (const auto &token : option) {
            if (!detail::process_character(options, buffer, token)) {
                return 0;
            }
        }
        // Clear options after processing the group.
        options.options.clear();
        break;
    }
    case '!':
        if (options.inside_group) {
            options.current_option.push_back(character);
        } else {
            options.capitalize = true;
        }
        break;
    default:
        if (options.inside_group) {
            options.current_option.push_back(character);
        } else if (options.emit_literal) {
            buffer.push_back(detail::capitalize_and_clear(character, options.capitalize));
        } else {
            detail::process_token(options, buffer, character);
        }
    }
    return 1;
}

} // namespace detail

/// @brief Loads tokens from a JSON file.
/// @param filename The path to the JSON file containing the tokens.
/// @return true if the loading was successful, false otherwise.
inline bool load_tokens_from_json(const std::string &filename)
{
    try {
        auto root = json::parser::parse_file(filename);
        if (!root.is_object()) {
            std::cerr << "Error: JSON root is not an object." << std::endl;
            return false;
        }
        // Clear existing tokens before loading new ones.
        detail::token_map.clear();
        root >> detail::token_map;
        if (detail::token_map.empty()) {
            std::cerr << "Warning: No tokens found in the JSON file." << std::endl;
            return false;
        }
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Error loading tokens: " << e.what() << std::endl;
        return false;
    }
}

/// @brief Sets the token list of a given key in the global token map.
/// @param key The key for which to set the token list.
/// @param list The list of tokens (strings) to associate with the key.
inline void set_token(detail::key_t key, const std::vector<std::string> &list) { namegen::detail::token_map[key] = list; }

/// @brief Sets a given list of key-value pairs in the global token map.
/// @param tokens A map where each key is a character and the value is a list of strings (tokens).
inline void set_tokens(const detail::token_map_t &tokens)
{
    for (const auto &token : tokens) {
        set_token(token.first, token.second);
    }
}

/// @brief Generates a random name based on the provided pattern and seed.
/// Replaces tokens in the pattern with randomly selected values and stores
/// the result in the buffer. Capitalization and groupings are handled
/// according to the pattern.
///
/// @param pattern The pattern defining the structure of the name, with tokens
///                and special characters like `!`, `()`, and `<>`.
/// @param seed The seed for random number generation.
/// @return The output string where the generated name will be stored.
auto generate(const std::string &pattern, namegen::seed_t seed) -> std::string
{
    detail::option_t options;
    options.seed = seed;
    std::string buffer;
    for (const auto &c : pattern) {
        if (!detail::process_character(options, buffer, c)) {
            buffer.clear();
            break;
        }
    }
    return buffer;
}

} // namespace namegen