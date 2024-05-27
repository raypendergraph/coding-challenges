from collections import Counter

def chase(string, d):
    for i in range(1,len(string)+1):
        d[string[0:i]] +=1

def minion_game(string):
    stuart = Counter()
    kevin = Counter()
    for i, c in enumerate(string):
       if c in ("A", "E", "I", "O", "U"):
          chase(string[i:], kevin)
          continue
       chase(string[i:], stuart)
    sum_stuart = sum(stuart.values())
    sum_kevin = sum(kevin.values())
    if sum_stuart > sum_kevin:
        print(f"Stuart {sum_stuart}")
    elif sum_stuart < sum_kevin:
        print(f"Kevin {sum_kevin}")
    else:
        print("Draw")

if __name__ == '__main__':
    #s = input()
    s = "BANANANAAAS"
    minion_game(s)