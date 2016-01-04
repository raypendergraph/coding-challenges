from functools import lru_cache

N, M = [int(_) for _ in input().split(" ")]
coins = [int(_) for _ in input().strip().split(" ")][:M]
coins.sort(reverse=True)
last_coin_idx = len(coins) - 1


@lru_cache(maxsize=512)
def count(coin_idx, total):
    # If n is 0 then there is 1 solution (do not include any coin)
    if total == 0:
        return 1

    # If n is less than 0 then no solution exists
    if total < 0:
        return 0

    # If there are no coins and n is greater than 0, then no solution exist
    if coin_idx <= 0 and total >= 1:
        return 0

    # count is sum of solutions (i) including S[m-1] (ii) excluding S[m-1]
    return count(coin_idx - 1, total) + count(coin_idx, total - coins[coin_idx - 1])


def optimized_count(coin_count, total):
    # table[i] will be storing the number of solutions for
    # value i. We need n+1 rows as the table is consturcted
    # in bottom up manner using the base case (n = 0)
    solutions = [0] * (total + 1)

    # Base case (If given value is 0)
    solutions[0] = 1

    # Pick all coins one by one and update the table[] values
    # after the index greater than or equal to the value of the
    # picked coin
    for coin_idx in range(coin_count):
        coin = coins[coin_idx]
        print("############### Coin ", coin)
        for amount in range(coin, total + 1):
            print("amount", amount, " a - c",amount - coin)
            print([str(_[1]) if _[0] == amount else "_" for _ in enumerate(solutions)])
            solutions[amount] += solutions[amount - coin]
            print([str(_[1]) if _[0] == amount else "_" for _ in enumerate(solutions)])

    return solutions[total]

print(coins)
print([str(_) for _ in range(0, N+1)])
print(optimized_count(M, N))
