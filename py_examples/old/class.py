# Example file for working with classes
class myClass():
  def method1(self):
      print("myClass method 1 called")
        
class childClass(myClass):
  def method1(self):
        myClass.method1(self)   # not defined but derived from the parent myClass
        print ("childClass method 1 called")
        
  def method2(self):
        print("childClass method 2 called")     
         
def main():           
  # exercise the class methods
  c2 = childClass()
  c2.method1()
  c2.method2()

if __name__== "__main__":
  main()