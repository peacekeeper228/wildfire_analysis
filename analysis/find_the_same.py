from collections import defaultdict

def produce_slope_key(first_part: str):
    parts = first_part.split(",")
    return f"{parts[0]}_{int(parts[1]) % 2 == 0}"

with open('distribution.csv', mode='r') as file:
    lines = file.readlines()

first = defaultdict(int)
for i in lines:
    first[produce_slope_key(i.split(";")[0])] += int(i.split(";")[1]) 


with open('distribution_2.csv', mode='r') as file:
    lines2 = file.readlines()

second = defaultdict(int)
for i in lines2:
    second[produce_slope_key(i.split(";")[0])] += int(i.split(";")[1]) 

result = {k: first.get(k, 0) + second.get(k, 0) for k in set(first) & set(second)}
print(result)
print(len(first))
print(len(second))
print(sum(first.values()))

print(f"Всего было {sum(first.values()) + sum(second.values())}, экономим {sum(result.values())}")

