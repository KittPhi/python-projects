# list Methods
# https://www.digitalocean.com/community/tutorials/how-to-use-list-methods-in-python-3

fish = ['barracuda', 'cod']

# Add to end of list: list.append()
fish.append('flounder')
print(fish)  # ['barracuda', 'cod', 'flounder']

fish.append(5)
print(fish)  # ['barracuda', 'cod', 'flounder', 5]

# add at index. list.index(i,x)   index, item
fish.insert(0, 'anchovy')
print(fish)  # ['anchovy', 'barracuda', 'cod', 'flounder', 5]

# list.extend(list_to_add)
# ---------------------------------------------------
# alphabets list
alphabets = ['a', 'e', 'i', 'o', 'g', 'l', 'i', 'u']

# index of 'i' in alphabets
index = alphabets.index('e')   # 2
print('The index of e:', index)

# 'i' between 0 upto 3 (not including 3) index is searched
index = alphabets.index('i', 0, 3)
print('The index of i:', index)