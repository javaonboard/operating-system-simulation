Client Code written in JAVA 

Execution instruction:

1- please got to client folder and try: java -jar Client.jar 
2- you can open the project in Intelji or any other IDE and run the code Client.java
3- you can follow below instruction to generate new executable jar file

Note: This code compiled in Java 13 if you execute and get problem because differnt Java version please follow instruction

#Step to compile and generate executble jar file 

1) javac -cp ./src ./src/*.java -d ./out/

2) jar cvfm Client.jar ./MANIFEST.MF -C ./out/ .

3) java -jar Client.jar



Code Instruction:

after running the code please follow below instructions:
1. program ask for server ip
2. program ask for server port
3. program ask for program name that you want to submit please make sure the program is in same directory as jar file (i already put prog1 there)
4. then you will see the command that comes from server please pass s as command and hit enter to submit 
5. then becuase i did not use admin.exe you can pass x to execute the program
6. you will see the output of simos send to you through socket
7. you can use T to terminate from server, if you got problem after exeution you can test it again by make new connection and try T command.
8. I tried my best to make it work 

Thank you

