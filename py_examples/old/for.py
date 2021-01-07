for x in range(2,7):
		print(x)

#use a for loop over a collection
Months = ["Jan","Feb","Mar","April","May","June"]
for m in Months:
    print(m)
		
# use the break and continue statements
for x in range (10,20):
    if (x == 15): break
    if (x % 2 == 0) : continue # if even continue
    print(x)

#use a for loop over a collection
Months = ["Jan","Feb","Mar","April","May","June"]
for i, m in enumerate (Months):
	print(i,m)

for i in '123':
    print ("guru99",i,)