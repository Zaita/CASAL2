//============================================================================
// Name        : CStandardHeaderReport.cpp
// Author      : S.Rasmussen
// Date        : 5/01/2009
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Global Headers
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <math.h>
#include <ctime>
#include <sstream>


// Local Headers
#include "StandardHeader.h"
#include "../../Version.h"

// Namespaces
using std::endl;
using std::cout;
using std::cerr;
using std::ostringstream;

namespace iSAM {
namespace Reports {

/**
 * Default Constructor
 */
StandardHeader::StandardHeader() {

  // Variables
  tmeStart   = time(NULL);

  // Linux Vars
#ifndef __MINGW32__
  times(&cpu_start);
#endif
}

/**
 * Destructor
 */
StandardHeader::~StandardHeader() {
}

/**
 * Prepare the header. This involves building
 * all of the information we need so it can be printed.
 */
void StandardHeader::prepare() {

  ostringstream               header;
  header << "iSAM" << endl;

  /**
   * Build the Command line
   */
  header << "Call: ";
  vector<string>& commandLine = pGlobalConfig->getCommandLineParameters();
  for (unsigned i = 0; i < commandLine.size(); ++i)
    header << commandLine[i] << " ";
  header << endl;

    // Build Date
  header << "Date: " << ctime(&tmeStart);

  /**
   * Version information
   */
  header << "Version: " << SOURCE_CONTROL_VERSION << endl;
  header << "Copyright (c) 2012 - " << SOURCE_CONTROL_YEAR << ", NIWA (www.niwa.co.nz)" << endl;

  /**
   * User and Computer Information
   */

#ifdef __MINGW32__
  header << "Environment: machine:" << getenv("COMPUTERNAME") << ", user:" << getenv("USERNAME") << ", os:" << getenv("OS") << ", pid:" << _getpid() << endl;


#else
  header << "-- Username: ";
  char* cUsername = getenv("LOGNAME");
  if (cUsername != NULL)
    header << cUsername << endl;
  else {
    header << "-----" << endl;
  }

    struct utsname names;
  uname(&names);
  header << "-- Machine: " << names.nodename << " (" << names.sysname << " " << names.release << " " << names.machine << ")" << endl;
  header << "-- Process Id: " << getpid() << endl;
#endif

  // Print Header
  cout << header.str() << endl;
}

/**
 * This report does it's printing during the start and finalise methods
 * so we do not need to have any logic within the run method.
 */
void StandardHeader::run() { }

/**
 * Finalise our report. This will calculate how much time it took
 * to do the execution of our application and print this to the
 * console.
 */
void StandardHeader::finalise() {
#ifndef __MINGW32__
  times(&cpu_stop);
  double cpu_time=(static_cast<double>(cpu_stop.tms_utime)+static_cast<double>(cpu_stop.tms_stime))-(static_cast<double>(cpu_start.tms_utime) + static_cast<double>(cpu_start.tms_stime));
  // Turn into seconds
  cpu_time /= static_cast<double>(sysconf(_SC_CLK_TCK));
  // Turn into hours
  cpu_time = cpu_time / 3600.0;
  int P = (int) floor(log10(cpu_time))+4;
  cout << "Total CPU time: " << std::setprecision(P) << cpu_time << (cpu_time==1?" hour":" hours") << ".\n";
#endif

  double elapsed_time = static_cast<double>(time(NULL)-tmeStart);
  if(elapsed_time < 60) {
    int P = (int) floor(log10(elapsed_time))+4;
    cout << "Total elapsed time: " << std::setprecision(P) << fmax(1,elapsed_time) << (elapsed_time<=1?" second":" seconds") << endl;
  } else if((elapsed_time/60.0) < 60) {
    elapsed_time /= 60.0;
    int P = (int) floor(log10(elapsed_time))+4;
    cout << "Total elapsed time: " << std::setprecision(P) << elapsed_time << (elapsed_time==1?" minute":" minutes") << endl;
  } else {
    elapsed_time /= 3600.0;
    int P = (int) floor(log10(elapsed_time))+4;
    cout << "Total elapsed time: " << std::setprecision(P) << elapsed_time << (elapsed_time==1?" hour":" hours") << endl;
  }
  cout << "Completed" << endl;
}

} /* namespace Report */
} /* namespace iSAM */