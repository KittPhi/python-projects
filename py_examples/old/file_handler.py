# Built-in functions for creating, writing, and reading files
from os import path # checks if it exists

class file_handler:
  def __init__(self, name, path):
    self.name = name
    self.path = path

  def check_file(self):
    if path.exists(self.name) == True:
      print('File exists')
    if path.exists(self.name) == False:
      print('File does not exist')
    if path.isfile(self.name) == True:
      print('Is a file')
    if path.isdir(self.name) == True:
      print('Is a directory')

    self.write_file()

  def write_file(self):
    if path.exists(self.name) == True:
      f = open(self.name, "a+") # append | create file
    if path.exists(self.name) == False:
      f = open(self.name,"w+")  # create | overwrite file
    
    for i in range(3):
        f.write("writing on line number: %d\r" % (i+1))
    print('Finished writing to file.')
    f.close()
  
  def read_file(self):
    f = open(self.name, "r")
    if f.mode == "r":
      print('Reading file...')
      contents = f.read()
      print(contents)

file_1 = file_handler("test.csv", ".")
file_1.check_file()

# Note: "file_1" is the representation of the object outside of the class
# and "self"  is the representation of the object inside  the class.