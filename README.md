Casal2
======

[![Build Status](https://dev.azure.com/zaita/Casal2%20-%20Zaita%20Version/_apis/build/status/Zaita.CASAL2%20(Linux)?branchName=master)](https://dev.azure.com/zaita/Casal2%20-%20Zaita%20Version/_build/latest?definitionId=1&branchName=master)
[![CodeFactor](https://www.codefactor.io/repository/github/zaita/casal2/badge)](https://www.codefactor.io/repository/github/zaita/casal2)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

# About This Fork
This fork is designed to be the bleeding edge of structured population dynamics. The code base will be sync'd with the Casal2 mainbase frequently, but will offer extended functionality to improve performance and scientific possibilities.

Some of the key pieces of functionality provided by this fork are:
* Threading - Multi-threaded estimation/minimisation and markov chain monte carlo (MCMC) runs
* Integration with Python3 for user-defined components (processes, selectivities, likelihoods etc)
* Newest C++ standards, start with C++17
* Cleaner code base to allow for easier extensions
* More libraries and code providing extended, new and experimental minimisation and MCMC algorithms

# About Casal2
Casal2 is an open source age structured population dynamics model. 
It can implement a range of a statistical catch-at-age 
population dynamics models, using a discrete time-step 
area-space model that represents a cohort-based 
population age structure. 

This repository is standalone containing all thirdparty 
libraries and source code necessary to compile Casal2. 
This project is covered under the GNU General Public 
License version 2. This program is distributed in the 
hope that it will be useful, but WITHOUT ANY WARRANTY; 
without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.

For more information about Casal2 contact the Casal2 
Development Team at casal2@niwa.co.nz
