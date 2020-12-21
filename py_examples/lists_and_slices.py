sea_creatures = ['shark', 'octopus', 'squid', 'shrimp', 'anemone']

print(sea_creatures[-2])  # shrimp

# slice[x:y] x is always inclusive, y is not included.

# slice (from 1 to 3)
print(sea_creatures[1:4])   # ['octopus', 'squid', 'shrimp']

# to include ":" (index 0 to 2)
print(sea_creatures[:3])  # ['shark', 'octopus', 'squid']

# 4th from right to 3rd from right
print(sea_creatures[-4:-2])  # ['octopus', 'squid']

# list[x:y:z], with z referring to stride.
numbers = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]

print(numbers[1:11:2])  # [1, 3, 5, 7, 9]
# 2 tells the program to print out only every other item.

# omit first two (includes entire list), just use stride
print(numbers[::4])  # [0, 4, 8, 12]

# Adding list
sea_creatures = ['shark']
land_creatures = ['monkey']
print(sea_creatures + land_creatures)  # ['shark', 'monkey']

sea_creatures = sea_creatures + ['yeti crab']
print(sea_creatures)  # ['shark', 'yeti crab']

land_creatures = ['monkey']
print(land_creatures * 2)  # ['monkey', 'monkey']

jungle = ['amazon']
for x in range(1, 2):  # will iterate only ONCE!
    jungle += ['forrest']
    print(jungle)  # ['amazon', 'forrest']

jungle = ['amazon']
for x in range(0, 1):  # will iterate only ONCE!
    jungle += ['forrest']
    print(jungle)  # ['amazon', 'forrest']

# deleting an item from list
sea_creatures = ['shark', 'octopus']
del sea_creatures[1]
print(sea_creatures)  # ['shark']

# deleting items from list
sea_creatures = ['shark', 'octopus', 'blobfish', 'shrimp']
del sea_creatures[:3]  # del 0,1,2
print(sea_creatures)    # ['shrimp']
print(sea_creatures[0])  # shrimp

# List with List items
numbers = [[1, 3, 5], [2, 4, 6], 10, [7, 8, 9]]
print(numbers[2])    # 10
print(numbers[0][1])  # 3

# Lists and slicing
# https://www.digitalocean.com/community/tutorials/understanding-lists-in-python-3#:~:text=A%20list%20is%20a%20data,values%20between%20square%20brackets%20%5B%20%5D%20.


