# Built-In:
# index() list.index( element, start, end )
# append() list.append( item )
# extend() list_1.extend( list_2 )
# insert() list.insert( i, element )
# remove() list.remove( element )
# pop()    list.pop( index ), default last index
# delete() del list, del list[:], del list[2], del list[1:4]
# remove() list.remove( element )
# count()  list.count( element )
# reverse() list.reverse()
# sort()    list.sort()

# element can be { char, int, float, string }


print("dictionaries ::::::::::::::::::::::::::::")

info = {'number': 4320, 'street': 'Calle Sonesta', 'city': 'Las Cruces', 'state': 'NM', 'zip': 88011}
info['number']
info['number'] = 4230
print( info.keys() )
print( info.values() )
print( info.items() )


del info['zip']
print(info)

info.clear()
print(info)

print("conversion :::::::::::::::::::::::::::::::")

# convert between
tu = (1,2,3)
li = [1,2,3]
li = list(tu)
tu = tuple(li)

print("extend() :::::::::::::::::::::::::::::::::")
li = [1,2,3,4,5]
li.append([6,7,8])
li.extend([9,10,11])
print(f'append vs. extend {li}')

print("list() :::::::::::::::::::::::::::::::::::")

# animals list
animals = ['cat', 'dog', 'rabbit']

# list of wild animals
wild_animals = ['tiger', 'fox']

# appending wild_animals list to the animals list
animals.append(wild_animals)

print('Updated animals list: ', animals)

print("mixed list() ::::::::::::::::::::::::::::::")

mixed_list = [{1, 2}, [5, 6, 7]]

# number tuple
number_tuple = (3, 4)

# inserting a tuple to the list
mixed_list.insert(1, number_tuple)

print('Updated List:', mixed_list)

print("index() :::::::::::::::::::::::::::::::::::")
# vowels list
vowels = ['a', 'e', 'i', 'o', 'i', 'u']

# index of 'e' in vowels
index = vowels.index('e')
print('The index of e:', index)

# index of'p' is vowels
# index = vowels.index('p')
# print('The index of p:', index)
# >> ValueError: 'p' is not in list

# alphabets list
alphabets = ['a', 'e', 'i', 'o', 'g', 'l', 'i', 'u']

# index of 'i' in alphabets
index = alphabets.index('i')   # 2
print('The index of i:', index)

# 'i' after the 4th index is searched
index = alphabets.index('i', 4)   # 6
print('The index of i:', index)

# Tuples and strings are immutable
# Lists are mutable
tu = (23, 'abc', 4.56, (2,3), 'def')
li = ['abc', 34, 4.34, 23]

# Tuple is immutable (unchangable)
# tu[0] = 5 # ERROR

# SPEED: The mutability of lists means that they aren’t as FAST as tuples. 

li[0]='def'
print(li)


print(tu[0], " is first item in tubple")

print("insert() ::::::::::::::::::::::::::::::::::")

li = ['a', 'b', 'c', 'e', 'f']
li.insert(3, 'd')
print(f'inserted d List: {li}')


print("pop() :::::::::::::::::::::::::::::::::::::")

# programming languages list
languages = ['Python', 'Java', 'C++', 'Ruby', 'C']

# remove and return the last item
print('When index is not passed:') 
print('Return Value:', languages.pop())
print('Updated List:', languages)

# remove and return the last item
print('\nWhen -1 is passed:') 
print('Return Value:', languages.pop(-1))
print('Updated List:', languages)

# remove and return the third last item
print('\nWhen -3 is passed:') 
print('Return Value:', languages.pop(-3))
print('Updated List:', languages)

print("delete ::::::::::::::::::::::::::::::::::::")

my_var = 15
my_tuple = ('Kitt', 25)
my_dict = {'name': 'Kitt', 'age': 25}
my_list = ['a', 'b', 'c', 'd', 'e', 'f', 'g']

del my_var
del my_tuple
del my_dict
del my_list[1]

# deleting items from 2nd to 4th
del my_list[1:4]

# deleting all elements
del my_list[:]

# -------------
person = { 'name': 'Sam',
  'age': 25,
  'profession': 'Programmer'
}

del person['profession']

# Output: {'name': 'Sam', 'age': 25}
print(person)

print("string ::::::::::::::::::::::::::::::::::::")

st = "Hello World"
second = st[1] # Second character in string.
print(second)

# Loop through string
my_string = "hello world"
my_tuple = 0, 5
print( my_string[1:4] ) # ell

# Loop through string with start and end index
string_name = "GEEKS"
  
# Iterate over index of string
for i in range(0, len( string_name )): 
    if string_name[ i ] == "E":
        print( string_name[ i ]  ) 

# loop thru each char of string
count=0
for i in string_name:
    if i == "E":
        count += 1
print( count )

print("in :::::::::::::::::::::::::::::::::::::::::")

li = [1, 2, 3, 4]
if (5 in li) == False:
    print("False")

string = "abcde"
if ("a" in string) == True: # Need parenthesis ! !
    print("True")

if ("ab" in string) == True:
    print("True")

print(" Print ::::::::::::::::::::::::::::::::::::::")
name = "Kitt Phi"
print( f"Hello {name} " * 3 ) 
print( "Hello {}".format(name) )