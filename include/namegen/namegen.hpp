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

enum : unsigned char {
    NAMEGEN_MAJOR_VERSION = 1, ///< Major version of the library.
    NAMEGEN_MINOR_VERSION = 0, ///< Minor version of the library.
    NAMEGEN_MICRO_VERSION = 0  ///< Micro version of the library.
};

/// @brief Main namespace.
namespace namegen
{

/// @brief Contains support functions.
namespace detail
{

/// @brief Represents a single character used as a key for token categories.
using key_t        = char;
/// @brief Represents a token string, which is going to replace the key.
using token_t      = std::string;
/// @brief Represents a collection of tokens.
using token_list_t = std::vector<token_t>;

/// @brief Define the option_t struct that encapsulates all the state options.
struct option_t {
    bool capitalize   = false;        ///< Capitalization flag for the next item
    bool emit_literal = false;        ///< We are emitting literal character.
    bool inside_group = false;        ///< Indicates whether we're inside a < > group
    std::size_t seed  = 0UL;          ///< Keeps track of the seed.
    std::string current_option;       ///< Holds the current option being processed in the group.
    std::vector<std::string> options; ///< Collects options inside a group.
};

/// @brief If the provided key is valid, it will set `tokens` with the array of
/// strings, and return the dimension of the array.
/// @param key the key we want to search.
/// @return the number of tokens in the array.
inline auto get_tokens(key_t key) -> const token_list_t &
{
    // A map of key to token lists (using vectors for dynamic storage)
    static const std::unordered_map<key_t, token_list_t> token_map = {
        {
            's',
            {
                "ach", "ack",  "ad",  "age", "ald", "ale", "an",   "ang",  "ar",   "ard",  "as",  "ash", "at",
                "ath", "augh", "aw",  "ban", "bel", "bur", "cer",  "cha",  "che",  "dan",  "dar", "del", "den",
                "dra", "dyn",  "ech", "eld", "elm", "em",  "en",   "end",  "eng",  "enth", "er",  "ess", "est",
                "et",  "gar",  "gha", "hat", "hin", "hon", "ia",   "ight", "ild",  "im",   "ina", "ine", "ing",
                "ir",  "is",   "iss", "it",  "kal", "kel", "kim",  "kin",  "ler",  "lor",  "lye", "mor", "mos",
                "nal", "ny",   "nys", "old", "om",  "on",  "or",   "orm",  "os",   "ough", "per", "pol", "qua",
                "que", "rad",  "rak", "ran", "ray", "ril", "ris",  "rod",  "roth", "ryn",  "sam", "say", "ser",
                "shy", "skel", "sul", "tai", "tan", "tas", "ther", "tia",  "tin",  "ton",  "tor", "tur", "um",
                "und", "unt",  "urn", "usk", "ust", "ver", "ves",  "vor",  "war",  "wor",  "yer",
            },
        },
        {
            'v',
            {
                "a",
                "e",
                "i",
                "o",
                "u",
                "y",
            },
        },
        {
            'V',
            {
                "a",  "e",  "i",  "o",  "u",  "y",  "ae", "ai", "au", "ay", "ea",
                "ee", "ei", "eu", "ey", "ia", "ie", "oe", "oi", "oo", "ou", "ui",
            },
        },
        {
            'c',
            {
                "b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "y", "z",
            },
        },
        {
            'B',
            {
                "b",  "bl",  "br", "c", "ch", "chr", "cl", "cr", "d",  "dr", "f",   "g",  "h",  "j",  "k",
                "l",  "ll",  "m",  "n", "p",  "ph",  "qu", "r",  "rh", "s",  "sch", "sh", "sl", "sm", "sn",
                "st", "str", "sw", "t", "th", "thr", "tr", "v",  "w",  "wh", "y",   "z",  "zh",
            },
        },
        {
            'C',
            {
                "b",  "c", "ch", "ck", "d", "f",  "g",  "gh", "h", "k",  "l",  "ld", "ll", "lt", "m", "n", "nd", "nn",
                "nt", "p", "ph", "q",  "r", "rd", "rr", "rt", "s", "sh", "ss", "st", "t",  "th", "v", "w", "y",  "z",
            },
        },
        {
            'i',
            {
                "air",    "ankle",   "ball",  "beef",    "bone", "bum",   "bumble",  "bump",    "cheese", "clod",
                "clot",   "clown",   "corn",  "dip",     "dolt", "doof",  "dork",    "dumb",    "face",   "finger",
                "foot",   "fumble",  "goof",  "grumble", "head", "knock", "knocker", "knuckle", "loaf",   "lump",
                "lunk",   "meat",    "muck",  "munch",   "nit",  "numb",  "pin",     "puff",    "skull",  "snark",
                "sneeze", "thimble", "twerp", "twit",    "wad",  "wimp",  "wipe",
            },
        },
        {
            'm',
            {
                "baby",     "booble", "bunker",  "cuddle", "cuddly",    "cutie",     "doodle",    "foofie",    "gooble",
                "honey",    "kissie", "lover",   "lovey",  "moofie",    "mooglie",   "moopie",    "moopsie",   "nookum",
                "poochie",  "poof",   "poofie",  "pookie", "schmoopie", "schnoogle", "schnookie", "schnookum", "smooch",
                "smoochie", "smoosh", "snoogle", "snoogy", "snookie",   "snookum",   "snuggy",    "sweetie",   "woogle",
                "woogy",    "wookie", "wookum",  "wuddle", "wuddly",    "wuggy",     "wunny",
            },
        },
        {
            'M',
            {
                "boo",       "bunch", "bunny", "cake", "cakes", "cute", "darling", "dumpling",
                "dumplings", "face",  "foof",  "goo",  "head",  "kin",  "kins",    "lips",
                "love",      "mush",  "pie",   "poo",  "pooh",  "pook", "pums",
            },
        },
        {
            'D',
            {
                "b",  "bl", "br", "cl", "d",  "f", "fl", "fr", "g",  "gh", "gl",
                "gr", "h",  "j",  "k",  "kl", "m", "n",  "p",  "th", "w",
            },
        },
        {
            'd',
            {
                "elch", "idiot", "ob",  "og",  "ok",   "olph", "olt", "omph", "ong", "onk",  "oo",  "oob",
                "oof",  "oog",   "ook", "ooz", "org",  "ork",  "orm", "oron", "ub",  "uck",  "ug",  "ulf",
                "ult",  "um",    "umb", "ump", "umph", "un",   "unb", "ung",  "unk", "unph", "unt", "uzz",
            },
        },
        {
            't',
            {
                "Master of",
                "Ruler of",
                "Teacher of",
                "Betrayer of",
                "Warden of",
                "Protector of",
                "Conqueror of",
                "King of",
                "Queen of",
                "Champion of",
                "Overlord of",
                "Defender of",
                "Seeker of",
                "Harbinger of",
                "Invoker of",
                "Shaper of",
                "Bearer of",
                "Savior of",
                "Keeper of",
                "Lord of",
                "Lady of",
                "Scholar of",
                "Lord Protector of",
                "Bringer of",
                "Emissary of",
                "Voice of",
                "Commander of",
                "Herald of",
                "Foe of",
                "Enlightener of",
                "Guardian of",
                "Scribe of",
                "Disruptor of",
                "Architect of",
                "Wanderer of",
                "Knight of",
                "Vanguard of",
                "Reaper of",
                "Adviser of",
                "Slayer of",
                "Hunter of",
                "Scribe of",
                "Guide of",
                "Throne of",
                "Archmage of",
                "Mystic of",
                "Scribe of",
                "Watcher of",
                "Curse of",
                "Revenge of",
                "Crown of",
                "Breaker of",
                "Lord of the Shadows",
                "Maestro of",
                "Illuminator of",
                "Tamer of",
                "Harvester of",
                "Bringer of the Dawn",
                "Wielder of",
                "Mastermind of",
                "Chronicler of",
                "Mentor of",
            },
        },
        {
            'T',
            {
                "the Endless",       "the Sea",          "the Fiery Pit",  "the Deep",          "the Forsaken",
                "the Fallen",        "the Immortal",     "the Forgotten",  "the Abyss",         "the Eternal Flame",
                "the Storm",         "the Unseen",       "the Boundless",  "the Savage",        "the Unyielding",
                "the Wilds",         "the First",        "the Cursed",     "the Heavens",       "the Shadows",
                "the Eternal Night", "the Darkened",     "the Wanderer",   "the Unknown",       "the Crowned",
                "the Iron Fist",     "the Moon",         "the Ashen",      "the Silent",        "the Wanderer",
                "the Unforgiven",    "the Alchemist",    "the Lost",       "the Eternal Watch", "the Glorious",
                "the Red Hand",      "the Sky",          "the Crucible",   "the Flame",         "the Ancient",
                "the Heralded",      "the Stormbringer", "the Dread",      "the Shattered",     "the Merciless",
                "the Void",          "the Conquered",    "the Broken",     "the Chosen",        "the Unchained",
                "the Hunter",        "the Dying",        "the Radiant",    "the Last",          "the Hidden",
                "the Seeker",        "the Vanquished",   "the Blighted",   "the Outcast",       "the Sacred",
                "the Voidbringer",   "the Vengeful",     "the Unshakable", "the Phoenix",       "the Blessed",
                "the Valiant",       "the Reborn",       "the Reckoning",
            },
        },
    };
    // Check if the key exists in the map
    auto it = token_map.find(key);
    if (it != token_map.end()) {
        return it->second;
    }
    static token_list_t no_tokens = {};
    return no_tokens;
}

/// @brief Returns a random number.
/// @param seed the seed used to generate the random number, it is modified.
/// @param min the lower bound for the random number.
/// @param max the upper bound for the random number.
/// @return a random number between min and max.
template <typename T>
inline auto get_rand(std::size_t &seed, T min, T max) -> T
{
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<T> distribution(min, max);
    seed = generator();
    return distribution(generator);
}

/// @brief Picks a random element from the given container of tokens.
/// @param seed A reference to the seed value used to generate the random index. It is modified during the function call.
/// @param tokens The container (a list or vector) holding the tokens from which to pick a random element.
/// @return A constant reference to the randomly selected token from the container.
inline auto pick_random_element(std::size_t &seed, const detail::token_list_t &tokens) -> const token_t &
{
    // Generate a random index between 0 and tokens.size() - 1
    std::size_t random_index = detail::get_rand(seed, 0UL, tokens.size() - 1);
    // Return the element at the randomly chosen index
    return tokens[random_index];
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
        if (!options.inside_group) {
            options.emit_literal = true;
        }
        break;
    case ')':
        if (!options.inside_group) {
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
        std::size_t random_index    = detail::get_rand(options.seed, 0UL, options.options.size() - 1);
        std::string selected_option = options.options[random_index];
        // Process and append the selected option.
        for (const auto &token : selected_option) {
            if (!detail::process_character(options, buffer, token)) {
                return 0;
            }
        }
        // Clear options after processing the group.
        options.options.clear();
        break;
    }
    case '!':
        options.capitalize = true;
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

/// @brief Generates a random name based on the provided pattern and seed.
/// Replaces tokens in the pattern with randomly selected values and stores
/// the result in the buffer. Capitalization and groupings are handled
/// according to the pattern.
///
/// @param pattern The pattern defining the structure of the name, with tokens
///                and special characters like `!`, `()`, and `<>`.
/// @param seed The seed for random number generation.
/// @return The output string where the generated name will be stored.
auto generate(const std::string &pattern, std::size_t seed) -> std::string
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