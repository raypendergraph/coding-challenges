from functools import lru_cache

__author__ = 'ray'

sum = 8
coins = [4, 2,1]
last_coin_idx = len(coins) - 1


def flatten_to_list(*args):
    def always_iter(obj):
        return obj if hasattr(obj, "__iter__") else (obj,)

    return [item for arg in args for item in always_iter(arg)]

def calculate(total, coin_idx):
    combinations = []
    coin = coins[coin_idx]
    fits, remains = divmod(total, coin)

    if coin_idx == last_coin_idx:
        return [coin]* fits if remains == 0 else []
    else:
        if fits == 0:
            combinations = calculate(total, coin_idx + 1)
        elif fits == 1:
            if remains == 0:
                combinations.append([coin])
            else:
                new_total = total - coin
                for subcombination in calculate(new_total, coin_idx + 1):
                    combinations.append(flatten_to_list(coin, subcombination))
        else:
            for subcombination in calculate(coin, coin_idx):
                combinations.append(flatten_to_list(coin, subcombination))

    return combinations

for idx in range(len(coins)):
    print(calculate(sum, idx))

#8
#4 4
#4 1 1 1 1