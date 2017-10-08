import re
from itertools import combinations
from typing import Tuple, Optional

from builtins import str

def build_correct_regex(length: int, string: str, sequence: str) -> Optional[str]:
    if string.startswith(sequence):
        if length % 2 == 0:
            return r'^({})+$'.format(sequence)
        else:
            return r'^({})+{}$'.format(sequence, sequence[0])


def build_regex_for_sequence(string: str, combination: Tuple[str, str]) -> Optional[str]:
        length = len(string)
        value = build_correct_regex(length, string, ''.join(combination))
        if value:
            return value

        return build_correct_regex(length, string, ''.join(reversed(combination)))


def main(string: str):
    unique_characters = set(string)
    two_character_combinations = combinations(unique_characters, 2)
    longest_match = 0
    for combination in two_character_combinations:
        other_characters = ''.join(unique_characters.difference(combination))
        sanitized_string = string
        if other_characters:
            sanitized_string = re.sub(r'[{}]'.format(other_characters),
                                      '',
                                      string)

        regex = build_regex_for_sequence(sanitized_string, combination)
        if not regex:
            continue

        if re.fullmatch(regex, sanitized_string):
            longest_match = max(longest_match, len(sanitized_string))

    print(longest_match)

if __name__ == '__main__':
    main('ab')
