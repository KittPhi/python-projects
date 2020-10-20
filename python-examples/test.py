
if x == 3:
    print("x equals 3")
elif x == 2:
    print("x equals 2")
else:
    print("x equals something")
print("This is the outside of if")

x = 3
while x < 10:
    if x > 7:
        x += 2
        continue
    x = x + 1
    print("Still in the loop")
    if x == 8:
        break
print("Outside the loop")

for x in range(10):
    if x > 7:
        x += 2
        continue
    x = x + 1
    print("Still in Loop")
    if x == 8:
        break
print("Outside of Loop")



