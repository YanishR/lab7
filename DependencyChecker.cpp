#include "DependencyChecker.h"

DependencyChecker::DependencyChecker(int numRegisters)
/* Creates RegisterInfo entries for each of the 32 registers and creates lists for
 * dependencies and instructions.
 */
{
  RegisterInfo r;

  // Create entries for all registers
  for(int i = 0; i < numRegisters; i++){
    myCurrentState.insert(make_pair(i, r));
  }
}

void DependencyChecker::addInstruction(Instruction i)
/* Adds an instruction to the list of instructions and checks to see if that 
 * instruction results in any new data dependencies.  If new data dependencies
 * are created with the addition of this instruction, appropriate entries
 * are added to the list of dependences.
 */
{
  InstType iType = i.getInstType();

  switch(iType){
  case RTYPE:
    // Check that each register is not UNDEFINED
    // Pass in RD for a Write dependence
    // and RT and RS for a Read dependence
    if (i.getRD() != 32)
        checkForWriteDependence(i.getRD());
    if (i.getRT() != 32)
        checkForReadDependence(i.getRT());
    if (i.getRS() != 32)
        checkForReadDependence(i.getRS());
    break;
  case ITYPE:
    // Chec that each register is not UNDEFINED
    // Pass in RT for a Write dependence
    // and RS for a Read dependence
    if (i.getRT() != 32)
        checkForWriteDependence(i.getRT());
    if (i.getRS() != 32)
        checkForReadDependence(i.getRS());
    break;
  case JTYPE:
    break;
  default:
    // do nothing
    break;
  }

  myInstructions.push_back(i);

}

void DependencyChecker::checkForReadDependence(unsigned int reg)
  /* Determines if a read data dependence occurs when reg is read by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
    if ( myCurrentState[reg].accessType ==  WRITE)
    {
        Dependence newDependence;

        newDependence.dependenceType = RAW;
        newDependence.registerNumber = reg;
        newDependence.previousInstructionNumber = myCurrentState[reg].lastInstructionToAccess;
        newDependence.currentInstructionNumber = (int) myInstructions.size();

        myDependences.push_back(newDependence);
    }

    myCurrentState[reg].accessType = READ;
    myCurrentState[reg].lastInstructionToAccess = (int) myInstructions.size();
}

void DependencyChecker::checkForWriteDependence(unsigned int reg)
  /* Determines if a write data dependence occurs when reg is written by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates 
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
   if (myCurrentState[reg].accessType != A_UNDEFINED) 
   {
        Dependence newDependence;

        if (myCurrentState[reg].accessType == READ)
            newDependence.dependenceType = WAR;
        else if (myCurrentState[reg].accessType == WRITE)
            newDependence.dependenceType = WAW;

        newDependence.registerNumber = reg;
        newDependence.previousInstructionNumber = myCurrentState[reg].lastInstructionToAccess;
        newDependence.currentInstructionNumber = (int) myInstructions.size();

        myDependences.push_back(newDependence);
   }
   
   myCurrentState[reg].accessType = WRITE;
   myCurrentState[reg].lastInstructionToAccess = (int) myInstructions.size();
}


void DependencyChecker::printDependences()
  /* Prints out the sequence of instructions followed by the sequence of data
   * dependencies.
   */ 
{
  // First, print all instructions
  list<Instruction>::iterator liter;
  int i = 0;
  cout << "INSTRUCTIONS:" << endl;
  for(liter = myInstructions.begin(); liter != myInstructions.end(); liter++){
    cout << i << ": " << (*liter).getAssembly() << endl;
    i++;
  }

  // Second, print all dependences
  list<Dependence>::iterator diter;
  cout << "DEPENDENCES: \nType Register (FirstInstr#, SecondInstr#) " << endl;
  for(diter = myDependences.begin(); diter != myDependences.end(); diter++){
    switch( (*diter).dependenceType){
    case RAW:
      cout << "RAW \t";
      break;
    case WAR:
      cout << "WAR \t";
      break;
    case WAW:
      cout << "WAW \t";
      break;
    default:
      break;
    }

    cout << "$" << (*diter).registerNumber << " \t";
    cout << "(" << (*diter).previousInstructionNumber << ", ";
    cout << (*diter).currentInstructionNumber << ")" << endl;
  }


}
