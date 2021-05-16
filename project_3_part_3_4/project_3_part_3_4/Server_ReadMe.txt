SiMOS Instruction for project_3 phase 3 and 4

Execution instruction: make -f makefile
		       ./simos.exe <port>
			hit the enter as program asked.
			then run the client 
Changed Files:
	Submit.c:
		1. socket_handling() method:
			a. make connection through socket with given port number
			b. start listening on given port
			c. using select() to control the resource access for mutiple client
			d. hadnle the possible errors
			e. as soon as client connected to the server it jump to the loop for communication
			f. ask user fot command> sending through socket 
			g. if user send s we start to gettingthe data from socket
			h. we wrtie sent data in a file
			i. ask user if wants to execute the the given file
			j. handle execution through pipe and dup and return the output to client
			k. if user willing to leave can enter T system will close the connection

		2. write_file():
			a. crete a empty file
			b. receving data through given socket 
			c. write the comming data in the created file
			e. close the file
		3. execute_sucket():
			as I use simos not admin i had to have simple pipe and dup to capture the stdout and send it to client
			a. using pipe to redirect the stdout to file descriptor in pipe
			b. execute the instruction
			c. get the std out from pipe and return it to caller
		4. start_client_submission () and end_client_submission()
			this two method are responislbe to create thread and handle the client submission through socket and join the trhead and end it.

There are changes in simos.h cpu.c but i did not mention that is obviouse to run the system with new changes.
The changes for project 3 phase 1 and 2 submitted in perviouse submmision.

I did my best to get it up and running. too much time.

Thanks

 

		