#!/bin/python3

from typing import Optional


def find_character(character: str, other: str) -> Optional[int]:
    for i, sample in enumerate(other):
        if sample == character:
            return i
    return None


def search_for_string(string: str, sample: str) -> bool:
    last_character = string[-1]
    all_but_last_character = string[:-1]
    string_state = sample
    for next_character in all_but_last_character:
        found_index = find_character(next_character, string_state)
        if found_index is None:
            return False
        string_state = string_state[found_index + 1:]

    if find_character(last_character, string_state) is None:
        return False
    else:
        return True


q = 1#int(input().strip())
test_string = 'hackerrank'
for _ in range(q):
    #s = input().strip()
    s = 'hackerworld'
    print('YES' if search_for_string(test_string, s) else 'NO')




