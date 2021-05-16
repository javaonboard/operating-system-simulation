Project 2 phase 3:


**************CHANGES*****************
1. AdminUI.c added
	a. it has main method 
	b. it create two fileDescriptor buffer
	c. it create two pipe
	d. handle the pipe error
	e. fork the process
	f. in parnet/Admin fork we close no need file descriptor 
	g. we get admin command
	h. write admin command in pipe
	i. send signal by using kill to child/Sim fork 
	j. in child/Sim fork we listen for parent signal
	G. we initialize the system
	k. we read command from admin/parent pipe
	l. will execute the command
	m. we write the output in parent pipe
	n. in Admin.parent we read from pipe and print out the result
	o. system exeit
2. cpu.c
	a. handling interrupt for admin added
	b. move the current to ready 
	c. clear the interrupt
3. system.c
	a. main mehtod comment out as it had confilict with adminUI
4. admin.c 
	a. x command removed
	b. y command removed
	c. iterative exe will invoke at intilize time
5. simos.h
	a. require abstarct method added
	b. new inttrupt for admin added
6. make file
	a. require change made to genrate admin.exe

**************INSTRUCTION*****************
1. extract the files
2. compile: make -f makefile
3. it ask for command 
4. pass the command and enter
5. it prints the logs
6. executes your command
6. display out put in console.