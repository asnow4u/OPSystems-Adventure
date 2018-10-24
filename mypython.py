
import string
import random




def main():

    #Establish Variables
    f = open("first.txt", "w+");
    s1 = random.choice(string.ascii_lowercase)

    #Loop 9 times for random char values
    for i in range(9):
        s1 = s1 + random.choice(string.ascii_lowercase)
    
    #Write and Print
    f.write(s1 + '\n')
    print(s1)
    f.close()

    #Establish Variables
    f = open("second.txt", "w+")
    s2 = random.choice(string.ascii_lowercase)

    #Loop 9 times for random char values
    for i in range(9):
        s2 = s2 + random.choice(string.ascii_lowercase)

    #Write and Print
    f.write(s2 + '\n')
    print(s2)
    f.close()


    #Establish Variables
    f = open("third.txt", "w+")
    s3 = random.choice(string.ascii_lowercase)

    #Loop 9 times for random char values
    for i in range(9):
        s3 = s3 + random.choice(string.ascii_lowercase)

    #Write and Print
    f.write(s3 + '\n')
    print(s3)
    f.close()



def number():
    num = random.randint(1, 43)
    print(num)

    num2 = random.randint(1, 43)
    print(num2)

    print(num * num2)




#Call main
main()
number()
