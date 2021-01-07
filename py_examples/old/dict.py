print(":".join("Python"))

# accessing tuples
tuple1 = (99, 'Mike', '1995', 'Five')
print(tuple1[0])
print(tuple1[2])

profile = ("NMSU", "Kruz Phi", 18)    # tuple packing
(school, student, age) = profile      # tuple unpacking
print (school)
print (student)
print (age)

# comparing Tuples
tuple1 = (5, 6)
tuple2 = (1, 4)
if (tuple1>tuple2):print("tuple1 is bigger")
else: print ("tuple2 is bigger")

# Dictionary
Dict = {'Tim': 18,'Charlie':12,'Tiffany':22,'Robert':25}	
print (Dict['Tiffany'])
Dict.update({"Tim":22})
print(Dict['Tim'])
del Dict['Tim']
print(Dict)

# if a key in Boys Dict exist in "Dict"
Dict = {'Tim': 18,'Charlie':12,'Tiffany':22,'Robert':25}
Boys = {'Tim': 18,'Charlie':12,'Robert':25}
Girls = {'Tiffany':22}
for key in Dict.keys():
    if key in Boys.keys():
        print (True)
    else:       
        print (False)

print('*' * 20)
Dict = {'Tim': 18,'Charlie':12,'Tiffany':22,'Robert':25}
Boys = {'Tim': 18,'Charlie':12,'Robert':25}
Girls = {'Tiffany':22}
Students = list(Dict.keys())
Students.sort()
for S in Students:
      print(":".join((S,str(Dict[S]))))

print('*' * 20)
x = 20
y = 20
if ( x is y ): 
	print("x & y  SAME identity")

print('*' * 20)
x=4
y=30
if ( x is not y ):
	print("x & y have DIFFERENT identity")

a="a"
b="smoke"
if (a not in b):
    print('letter not in word')

def square(x):
    print(x*x)
square(4)