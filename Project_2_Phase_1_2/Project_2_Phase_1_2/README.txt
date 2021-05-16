**PHASE 1
Changed Files:
	cpu.c: handle_interrupt (), execute_instruction ()  in order to complete the missing code.
	prrcess.c: context_in (int pid), context_out (int pid), execute_process (), in order to complete missing code.
	config.sys: in order to increase the maxProcess value to be compatible for submmission programs.
	makefile: in order to fix the missing paging.c

**PHASE 2 try to calculate 43 and 48 :D (SE4348)
	File name: myProgram
	Description:
		load 20 from memeory at 13
		mul by 2
		store (40) in first availbe free space at (19)
		print result from stored place in memory
		load 40 from 19
		add it by 3
		store 43 in to memory at 20
		load 10 from 16
		mul by 4 from 17
		add by 8 from 18
		store 48 to memory at 21
		print 48 from memory
		exit the program

