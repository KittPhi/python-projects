#-----------------------------------------------------------------
# find character in string
string = "012 45*7-9*"

result = string.find('*')

print (result) 
#-----------------------------------------------------------------
# find word in list

list = ['orange', 'apple', 'grape']

for fruit in list:
    if (fruit == 'grape'):
        print ('found ', fruit)
#-----------------------------------------------------------------
# another way to find word in list

# prints true
print ('grape' in list)

# prints false
print ('grape' not in list)
#-----------------------------------------------------------------
# function that takes in array and element to be sought
# Return index and count of how many

list = ['orange', 'apple', 'grape']

def search_element(list, item):

    # (1) Array_length must be an integer and not a string:
    array_length = len(list)
    # (2) for loop must be constructed using range:
    for i in range( array_length ):
        if (list[i] == item):
            print ('found item', list[i])
        elif (list[i] != item):
            print ('not item', list[i])
        else:
            print ('found something else...', list[i])

search_element(list, 'grape')
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------
#-----------------------------------------------------------------