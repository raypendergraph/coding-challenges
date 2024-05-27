from collections import Counter
from operator import mul, add
from functools import reduce
from math import factorial

prime_numbers = [
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101
]

def sherlockAndAnagrams(s):
    counter = Counter()
    for i in range(len(s)):
        for j in range(i + 1, len(s) + 1):
            counter[prime_hash(s[i:j])] += 1
    combinations = 0
    for value in (v for v in counter.values() if v >= 2):
        combinations += factorial(value) // (2 * factorial(value - 2))
    return combinations

def prime_hash(s: str):
    if not s:
        return 0
    return reduce(mul, (prime_numbers[ord(c)-97] for c in s))

def main():
    s="kkkk"
    #s="ifailuhkqq"
    combinations = sherlockAndAnagrams(s)    
    print(combinations)

if __name__ == "__main__":
    main()