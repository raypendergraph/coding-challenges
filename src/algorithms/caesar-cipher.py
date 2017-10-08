#!/bin/python3

from functools import partial

MASK = int(b'01100000', 2)

# Caesar cipher implementation using bit shifting.
# https://www.hackerrank.com/challenges/caesar-cipher-1/problem
def substitude_letter(shift: int, letter: str) -> str:
    if not letter.isalpha():
        return letter

    letter_ordinal = ord(letter)
    baseline = (MASK & letter_ordinal | 1)
    new_letter = (letter_ordinal - baseline + shift) % 26
    return chr(baseline + new_letter)


n = int(input().strip())
s = input().strip()
k = int(input().strip())

shift_function = partial(substitude_letter, k)
print(''.join((shift_function(l) for l in s)))
