#include "simos.h"

// opcode definitions
#define OPload 2
#define OPadd 3
#define OPmul 4
#define OPifgo 5
#define OPstore 6
#define OPprint 7
#define OPsleep 8
#define OPexit 1


void initialize_cpu ()
{ // Generally, cpu goes to a fix location to fetch and execute OS
  CPU.interruptV = 0;
  CPU.numCycles = 0;
}

void dump_registers (FILE *outf)
{ 
  fprintf (outf, "Pid=%d, ", CPU.Pid);
  fprintf (outf, "PC=%d, ", CPU.PC);
  fprintf (outf, "IR=(%d,%d), ", CPU.IRopcode, CPU.IRoperand);
  fprintf (outf, "AC="mdOutFormat", ", CPU.AC);
  fprintf (outf, "MBR="mdOutFormat"\n", CPU.MBR);
  fprintf (outf, "          Status=%d, ", CPU.exeStatus);
  fprintf (outf, "IV=%x, ", CPU.interruptV);
  fprintf (outf, "PT=%x, ", CPU.PTptr);
  fprintf (outf, "cycle=%d\n", CPU.numCycles);
}

void set_interrupt (unsigned bit)
{ CPU.interruptV = CPU.interruptV | bit; }

void clear_interrupt (unsigned bit)
{ unsigned negbit = -bit - 1;
  if (cpuDebug) fprintf (bugF, "IV is %x, ", CPU.interruptV);
  CPU.interruptV = CPU.interruptV & negbit;
  if (cpuDebug) fprintf (bugF, "after clear is %x\n", CPU.interruptV);
}

void handle_interrupt ()
{ if (cpuDebug) 
    fprintf (bugF,
            "Interrupt handler: pid = %d; interrupt = %x; exeStatus = %d\n",
            CPU.Pid, CPU.interruptV, CPU.exeStatus); 
  while (CPU.interruptV != 0)
  { if ((CPU.interruptV & ageInterrupt) == ageInterrupt)
    { memory_agescan ();
      clear_interrupt (ageInterrupt);
    }
    if ((CPU.interruptV & pFaultException) == pFaultException)
    { page_fault_handler (); 
      clear_interrupt (pFaultException);
    }
    if ((CPU.interruptV & endIOinterrupt) == endIOinterrupt){
    // *** ADD CODE to handle endIOinterrupt
    endIO_moveto_ready ();
    clear_interrupt (endIOinterrupt);
    }
    if ((CPU.interruptV & tqInterrupt) == tqInterrupt){
        printf("DEBUG: handling the tq %d %d \n",CPU.exeStatus, eRun);
        CPU.exeStatus = eReady;
        clear_interrupt (tqInterrupt);
        // *** ADD CODE to handle tqInterrupt
    }
    if((CPU.interruptV & adcmdInterrupt) == adcmdInterrupt){
        printf("handling the admin interrupt\n");
        CPU.exeStatus = eReady;
        clear_interrupt (adcmdInterrupt);
    }
    
  }
}

void fetch_instruction ()
{ int mret;

  mret = get_instruction (CPU.PC);
  if (mret == mError) CPU.exeStatus = eError;
  else if (mret == mPFault) CPU.exeStatus = ePFault;
  else // from this point on, it is in execute-instruction, not fetch
       // fetch data, but exclude OPexit and OPsleep, which has no data
       // also exclude OPstore, which stores data, not gets data
    if (CPU.IRopcode != OPexit && CPU.IRopcode != OPsleep
        && CPU.IRopcode != OPstore)
    { mret = get_data (CPU.IRoperand);
      if (mret == mError) CPU.exeStatus = eError;
      else if (mret == mPFault) CPU.exeStatus = ePFault;
      else if (CPU.IRopcode == OPifgo)
      { mret = get_instruction (CPU.PC+1);
        if (mret == mError) CPU.exeStatus = eError;
        else if (mret == mPFault) CPU.exeStatus = ePFault;
        else { CPU.PC++; CPU.IRopcode = OPifgo; }
      } // ifgo is the conditional goto instruction
    }   //   It has two words (all other instructions have only one word)
}       //   The memory address for the test variable is in the 1st word
        //      get_data above gets it into MBR
        //   goto addr is in the operand field of the second word
        //     we use get_instruction again to get it 
        //     Also PC++ is to let PC point to the true next instruction
        // ****** if there is page fault, PC will not be incremented

void execute_instruction ()
{ int gotoaddr, mret;
  switch (CPU.IRopcode)
  { case OPload:
  	printf("DEBUG: load \n");
      CPU.AC = CPU.MBR; break;
    case OPadd:
      printf("DEBUG: add \n");
      CPU.AC += CPU.MBR; break;
    case OPmul:
      printf("DEBUG: Multiply \n");
      CPU.AC *= CPU.MBR; break;
    case OPifgo:
      // *** ADD CODE for the instruction
      // check the comments for ifgo in fetch_instruction and then do thi
        printf("DEBUG: ifGo %d %d \n", CPU.IRoperand, CPU.MBR);
        if(CPU.MBR > 0){
	        printf("MBR= %d", CPU.MBR);
      	    CPU.PC = CPU.IRoperand;
      	    CPU.PC--;
	          printf("PC= %d",CPU.PC);
      	  }
        break;
    case OPstore:
        printf("DEBUG: store \n");
        put_data(CPU.IRoperand);
        break;
    case OPprint:
        printf("DEBUG: print \n");
        char *message = (char*)malloc(100 * sizeof(char));
        sprintf(message, "PID= %d IRoperand= %d MBR= %d", CPU.Pid, CPU.IRoperand, CPU.MBR );
        insert_termIO(CPU.Pid, message, regularIO);
	      break;
    case OPsleep:
      // *** ADD CODE for the instruction
      // need to set alarm by clock.c to get interrupted at the waking up time
      printf("DEBUG: sleep \n");
	    add_timer(CPU.IRoperand, CPU.Pid, actReadyInterrupt, 0);
     	CPU.exeStatus = eWait;
	    break;
    case OPexit:
      printf("DEBUG: exit \n");
    	CPU.exeStatus = eEnd;
    	break;
      // *** ADD CODE for the instruction
    default:
      fprintf (infF, "Illegitimate OPcode in process %d\n", CPU.Pid);
      CPU.exeStatus = eError;
  }
  
}

void cpu_execution ()
{ int mret;

  // perform all memory fetches, analyze memory conditions all here
  while (CPU.exeStatus == eRun)
  { fetch_instruction ();
    if (cpuDebug) { fprintf (bugF, "Fetched: "); dump_registers (bugF); }
    if (CPU.exeStatus == eRun)
    { execute_instruction ();
      // if it is eError or eEnd, does not matter
      // if it is page fault, then AC, PC should not be changed
      // because the instruction should be re-executed
      // so only execute if it is eRun
      if (CPU.exeStatus != ePFault) CPU.PC++;
        // the put_data may change exeStatus, need to check again
        // if it is ePFault, then data has not been put in memory
        // => need to set back PC so that instruction will be re-executed
        // no other instruction will cause problem and execution is done
      if (cpuDebug) { fprintf (bugF, "Executed: "); dump_registers (bugF); }
    }

    if (CPU.interruptV != 0) handle_interrupt ();
    usleep (instrTime);   // control the speed of execution
    advance_clock ();
      // since we don't have clock, we use instruction cycle as the clock
      // no matter whether there is a page fault or an error,
      // should handle clock increment and interrupt
  }
}

