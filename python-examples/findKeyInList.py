students = [
	{
		"name": "Bart",	
		"age": 8
	},
	{
		"name": "Homer",
		"age": 9
	},
	{
		"name": "Lisa",
		"age": 8
	}
]

to_find = input("Enter the name of the student whose age you want to find: ")

for s in range(len(students)):
	if students[s]["name"] == to_find:
		print("The age of {} is {}.".format(students[s]["name"], students[s]["age"]))
		print(students[s])
