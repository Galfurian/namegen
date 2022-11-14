/// @file namegen.hpp
/// @brief Fantasy name generator.
/// @details
/// Fantasy name generator ANSI C header library
/// This is free and unencumbered software released into the public domain.
///
/// The compile() function creates a name generator based on an input
/// pattern. The letters s, v, V, c, B, C, i, m, M, D, and d represent
/// different types of random replacements. Everything else is emitted
/// literally.
///
///   s - generic syllable
///   v - vowel
///   V - vowel or vowel combination
///   c - consonant
///   B - consonant or consonant combination suitable for beginning a word
///   C - consonant or consonant combination suitable anywhere in a word
///   i - insult
///   m - mushy name
///   M - mushy name ending
///   D - consonant suited for a stupid person's name
///   d - syllable suited for a stupid person's name (begins with a vowel)
///
/// All characters between parenthesis () are emitted literally. For
/// example, the pattern "s(dim)", emits a random generic syllable
/// followed by "dim".
///
/// Characters between angle brackets <> emit patterns from the table
/// above. Imagine the entire pattern is wrapped in one of these.
///
/// In both types of groupings, a vertical bar | denotes a random choice.
/// Empty groups are allowed. For example, "(foo|bar)" emits either "foo"
/// or "bar". The pattern "<c|v|>" emits a constant, vowel, or nothing at
/// all.
///
/// An exclamation point ! means to capitalize the component that follows
/// it. For example, "!(foo)" will emit "Foo" and "v!s" will emit a
/// lowercase vowel followed by a capitalized syllable, like "eRod".
///
/// This library is based on the RinkWorks Fantasy Name Generator.
/// http://www.rinkworks.com/namegen/
///

#pragma once

#include <string>

/// @brief Main namespace.
namespace namegen
{

/// Cannot exceed bits in a long.
#define NAME_MAX_DEPTH 32

/// Return codes.
enum return_code_t {
    SUCCESS, ///< Name successfully generated.
    INVALID, ///< Pattern is invalid.
    TOO_DEEP ///< Pattern exceeds maximum nesting depth.
};

/// Rather than compile the pattern into some internal representation,
/// the name is generated directly from the pattern in a single pass
/// using reservoir sampling. If an alternate option is selected, the
/// output pointer is reset to "undo" the output for the previous group.
/// This means the output buffer may be written beyond the final output
/// length (but never beyond the buffer length).
///
/// The substitution templates are stored in an efficient, packed form
/// that contains no pointers. This is to avoid cluttering up the
/// relocation table, but without any additional run-time overhead.

/// @brief Contains support functions.
namespace detail
{

/// @brief If the provided key is valid, it will set `tokens` with the array of
/// strings, and return the dimension of the array.
/// @param key the key we want to search.
/// @param tokens the output argument, if the key is valid it points to an array
/// of strings, otherwise it is set to NULL.
/// @return the number of tokens in the array.
inline std::size_t get_tokens(int key, const char **&tokens)
{
    if (key == 's') {
        static const char *__tokens[] = {
            "ach", "ack", "ad", "age", "ald", "ale", "an", "ang", "ar", "ard",
            "as", "ash", "at", "ath", "augh", "aw", "ban", "bel", "bur", "cer",
            "cha", "che", "dan", "dar", "del", "den", "dra", "dyn", "ech", "eld",
            "elm", "em", "en", "end", "eng", "enth", "er", "ess", "est", "et",
            "gar", "gha", "hat", "hin", "hon", "ia", "ight", "ild", "im", "ina",
            "ine", "ing", "ir", "is", "iss", "it", "kal", "kel", "kim", "kin",
            "ler", "lor", "lye", "mor", "mos", "nal", "ny", "nys", "old", "om",
            "on", "or", "orm", "os", "ough", "per", "pol", "qua", "que", "rad",
            "rak", "ran", "ray", "ril", "ris", "rod", "roth", "ryn", "sam",
            "say", "ser", "shy", "skel", "sul", "tai", "tan", "tas", "ther",
            "tia", "tin", "ton", "tor", "tur", "um", "und", "unt", "urn", "usk",
            "ust", "ver", "ves", "vor", "war", "wor", "yer"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'v') {
        static const char *__tokens[] = {
            "a", "e", "i", "o", "u", "y"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'V') {
        static const char *__tokens[] = {
            "a", "e", "i", "o", "u", "y", "ae", "ai", "au", "ay", "ea", "ee",
            "ei", "eu", "ey", "ia", "ie", "oe", "oi", "oo", "ou", "ui"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'c') {
        static const char *__tokens[] = {
            "b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r",
            "s", "t", "v", "w", "x", "y", "z"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'B') {
        static const char *__tokens[] = {
            "b", "bl", "br", "c", "ch", "chr", "cl", "cr", "d", "dr", "f", "g",
            "h", "j", "k", "l", "ll", "m", "n", "p", "ph", "qu", "r", "rh", "s",
            "sch", "sh", "sl", "sm", "sn", "st", "str", "sw", "t", "th", "thr",
            "tr", "v", "w", "wh", "y", "z", "zh"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'C') {
        static const char *__tokens[] = {
            "b", "c", "ch", "ck", "d", "f", "g", "gh", "h", "k", "l", "ld", "ll",
            "lt", "m", "n", "nd", "nn", "nt", "p", "ph", "q", "r", "rd", "rr",
            "rt", "s", "sh", "ss", "st", "t", "th", "v", "w", "y", "z"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'i') {
        static const char *__tokens[] = {
            "air", "ankle", "ball", "beef", "bone", "bum", "bumble", "bump",
            "cheese", "clod", "clot", "clown", "corn", "dip", "dolt", "doof",
            "dork", "dumb", "face", "finger", "foot", "fumble", "goof",
            "grumble", "head", "knock", "knocker", "knuckle", "loaf", "lump",
            "lunk", "meat", "muck", "munch", "nit", "numb", "pin", "puff",
            "skull", "snark", "sneeze", "thimble", "twerp", "twit", "wad",
            "wimp", "wipe"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'm') {
        static const char *__tokens[] = {
            "baby", "booble", "bunker", "cuddle", "cuddly", "cutie", "doodle",
            "foofie", "gooble", "honey", "kissie", "lover", "lovey", "moofie",
            "mooglie", "moopie", "moopsie", "nookum", "poochie", "poof",
            "poofie", "pookie", "schmoopie", "schnoogle", "schnookie",
            "schnookum", "smooch", "smoochie", "smoosh", "snoogle", "snoogy",
            "snookie", "snookum", "snuggy", "sweetie", "woogle", "woogy",
            "wookie", "wookum", "wuddle", "wuddly", "wuggy", "wunny"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'M') {
        static const char *__tokens[] = {
            "boo", "bunch", "bunny", "cake", "cakes", "cute", "darling",
            "dumpling", "dumplings", "face", "foof", "goo", "head", "kin",
            "kins", "lips", "love", "mush", "pie", "poo", "pooh", "pook", "pums"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'D') {
        static const char *__tokens[] = {
            "b", "bl", "br", "cl", "d", "f", "fl", "fr", "g", "gh", "gl", "gr",
            "h", "j", "k", "kl", "m", "n", "p", "th", "w"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    if (key == 'd') {
        static const char *__tokens[] = {
            "elch", "idiot", "ob", "og", "ok", "olph", "olt", "omph", "ong",
            "onk", "oo", "oob", "oof", "oog", "ook", "ooz", "org", "ork", "orm",
            "oron", "ub", "uck", "ug", "ulf", "ult", "um", "umb", "ump", "umph",
            "un", "unb", "ung", "unk", "unph", "unt", "uzz"
        };
        tokens = __tokens;
        return sizeof(__tokens) / sizeof(__tokens[0]);
    }
    tokens = NULL;
    return 0;
}

/// @brief Returns a random number.
/// @param seed the seed used to generate the random number, it is modified.
/// @return a random number between 0 and ULONG_MAX.
inline unsigned long get_rand(unsigned long &seed)
{
    seed ^= seed << 13;
    seed ^= (seed & 0xffffffffUL) >> 17;
    seed ^= seed << 5;
    return seed & 0xffffffffUL;
}

/// @brief Returns a random number.
/// @param seed the seed used to generate the random number, it is modified.
/// @param min the lower bound for the random number.
/// @param max the upper bound for the random number.
/// @return a random number between min and max.
template <typename T>
inline T get_rand(unsigned long &seed, T min, T max)
{
    return static_cast<T>(min + (get_rand(seed) % max));
}

/// @brief Capitalizes the given character.
/// @param c the input caracter
/// @param capitalize controls if we should capitalize or not.
/// @return the capitalized character, if capitalize is true.
inline char get_capitalized(int c, bool capitalize)
{
    return static_cast<char>(capitalize ? std::toupper(c) : c);
}

/// @brief Returns the lenght of the input string.
/// @param s the input string.
/// @return the lenght of the input string.
inline size_t get_strlen(const char *s)
{
    size_t len = 0;
    while (*(s++)) ++len;
    return len;
}

/// @brief Copy a random token inside the buffer, based on the key, at the given location.
/// @param buffer the buffer we manipulate.
/// @param loc the location where the substitution should be placed.
/// @param key the key used to determine the substitution.
/// @param seed the seed for random number generation.
/// @param capitalize controls capitalization of the first letter.
inline void insert_token(
    std::string &buffer,
    std::size_t &location,
    int key,
    unsigned long &seed,
    bool capitalize)
{
    const char **tokens;
    std::size_t count = get_tokens(key, tokens);
    if (count <= 0) {
        if (location == buffer.size())
            buffer.resize(buffer.size() + 1);
        buffer[location++] = detail::get_capitalized(key, capitalize);
    } else {
        size_t select     = get_rand<size_t>(seed, 0UL, count);
        const char *token = tokens[select];
        std::size_t size  = get_strlen(token);
        if ((location + size) > buffer.size()) {
            buffer.resize(buffer.size() + size);
        }
        while (*token) {
            buffer[location++] = get_capitalized(*token++, capitalize);
            capitalize         = 0;
        }
    }
}

} // namespace detail

/// @brief Generate a random name based on pattern and a given seed, and saves it into buffer.
/// @param buffer the string where the name is placed.
/// @param pattern the patter used to generate the random name.
/// @param seed the seed used for random number generation.
/// @return The return value is one of the above codes, indicating success or
/// that something went wrong. Truncation occurs when DST was too short. Pattern
/// is validated even when the output has been truncated.
return_code_t generate(std::string &buffer, const std::string &pattern, unsigned long &seed)
{
    // Current nesting depth.
    int depth = 0;
    // Current output pointer.
    std::size_t loc = 0;
    // Capitalize next item.
    bool capitalize = false;

    // Reset pointer (undo generate).
    std::size_t reset[NAME_MAX_DEPTH];
    // Number of groups.
    unsigned long n[NAME_MAX_DEPTH];
    // Actively generating?
    unsigned long silent = 0;
    // Current "mode".
    unsigned long literal = 0;
    // Initial capitalization state.
    unsigned long capstack = 0;

    // Bit for current depth.
    unsigned long bit;

    // Contains the currently parsed character.
    int c;

    n[0]     = 1;
    reset[0] = 0;
    for (std::string::const_iterator it = pattern.begin(); it != pattern.end(); ++it) {
        // Get the character.
        c = *it;
        // Parse the character.
        switch (c) {
        case '<':
            if (++depth == NAME_MAX_DEPTH) {
                buffer.clear();
                return TOO_DEEP;
            }
            bit          = 1UL << depth;
            n[depth]     = 1;
            reset[depth] = loc;
            literal &= ~bit;
            silent &= ~bit;
            silent |= (silent << 1) & bit;
            capstack &= ~bit;
            capstack |= (unsigned long)capitalize << depth;
            break;

        case '(':
            if (++depth == NAME_MAX_DEPTH) {
                buffer.clear();
                return TOO_DEEP;
            }
            bit          = 1UL << depth;
            n[depth]     = 1;
            reset[depth] = loc;
            literal |= bit;
            silent &= ~bit;
            silent |= (silent << 1) & bit;
            capstack &= ~bit;
            capstack |= (unsigned long)capitalize << depth;
            break;

        case '>':
            if (depth == 0) {
                buffer.clear();
                return INVALID;
            }
            bit = 1UL << depth--;
            if (literal & bit) {
                buffer.clear();
                return INVALID;
            }
            break;

        case ')':
            if (depth == 0) {
                buffer.clear();
                return INVALID;
            }
            bit = 1UL << depth--;
            if (!(literal & bit)) {
                buffer.clear();
                return INVALID;
            }
            break;

        case '|':
            bit = 1UL << depth;
            // Stay silent if parent group is silent.
            if (!(silent & (bit >> 1))) {
                if (detail::get_rand(seed) < (0xffffffffUL / ++n[depth])) {
                    // Switch to this option.
                    loc = reset[depth];
                    silent &= ~bit;
                    capitalize = !!(capstack & bit);
                } else {
                    // Skip this option.
                    silent |= bit;
                }
            }
            break;

        case '!':
            capitalize = true;
            break;

        default:
            bit = 1UL << depth;
            if (!(silent & bit)) {
                if (literal & bit) {
                    // Copy value literally.
                    if (loc == buffer.size())
                        buffer.resize(buffer.size() + 1);
                    buffer[loc++] = detail::get_capitalized(c, capitalize);
                } else {
                    // Insert the toke inside the buffer.
                    detail::insert_token(buffer, loc, c, seed, capitalize);
                }
            }
            capitalize = false;
        }
    }
    if (depth) {
        buffer.clear();
        return INVALID;
    }
    buffer[loc] = 0;
    return SUCCESS;
}

} // namespace namegen