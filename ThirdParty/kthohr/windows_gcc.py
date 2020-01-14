import os
import os.path
import shutil
from distutils import dir_util

import Globals

class Builder():
  version_ = '3.9.0'
  
  def is_lean(self):
    return True
  
  def start(self):   
    pwd =  os.getcwd()        
        
    '''
    BUILD EVERYTHING FOR ARMADILLO
    '''
    build_armadillo = True
    if build_armadillo:
        print('-- Building Sub-Library Armadillo')
        print('-- Cleaning up old files')
        armaFileName     = 'armadillo-code-9.900.x'

        if os.path.exists(Globals.target_include_path_ + 'armadillo_bits'):
            os.system('rm -rf ' + Globals.target_include_path_ + 'armadillo_bits')
        if os.path.exists(Globals.target_include_path_ + 'armadillo'):
            os.system('rm -rf ' + Globals.target_include_path_ + 'armadillo')
        if os.path.exists(armaFileName):
            os.system('rm -rf ' + armaFileName)

        print('-- Decompressing Armadillo')
        if os.path.exists(armaFileName + '.zip'):
            os.system('unzip ' + armaFileName + '.zip') #1> armadillo_unzip.log 2>&1') 
        else:
            return Globals.PrintError('Unable to Decompress ' + armaFileName + '.zip')

        dir_util.copy_tree(armaFileName + '/include', Globals.target_include_path_)

    
    '''
    BUILD EVERYTHING FOR LAPACK
    '''
    build_lapack = True
    if build_lapack:
        print('-- Building Sub-Library Lapack')
        print('-- Cleaning up old files')
        lapackFileName   = 'lapack-3.9.0'

        libraries = [ 'libblas.a', 'liblapack.a' ]
        for library in libraries:
            os.system('rm -rf ' + Globals.target_debug_lib_path_ + library)
            os.system('rm -rf ' + Globals.target_release_lib_path_ + library)
        if os.path.exists(lapackFileName):
            os.system('rm -rf ' + lapackFileName)

        print('-- Decompressing LaPack')
        if os.path.exists(lapackFileName + '.7z'):
            os.system('7za.exe x -bd -y ' + lapackFileName + '.7z 1> lapack_unzip.log 2>&1') 
        else:
            return Globals.PrintError('Unable to Decompress ' + lapackFileName + '.7z')

        # Build La Pack    
        print('-- Building Lapack - check casal2_build.log for progress')
        os.chdir(lapackFileName)
        os.mkdir('build')
        os.chdir('build')
        print('-- cmake command: cmake ' + Globals.cmake_compiler_ + ' .. ')
        os.system('cmake ' + Globals.cmake_compiler_ + ' .. > casal2_cmake.log 2>&1') 
        print('-- Compiling....')
        os.system('mingw32-make > casal2_build.log 2>&1')

        # Move LaPack Files
        print('-- Moving files across')   
        shutil.copy('lib/liblapack.a', Globals.target_debug_lib_path_)
        shutil.copy('lib/liblapack.a', Globals.target_release_lib_path_)
        shutil.copy('lib/libblas.a', Globals.target_debug_lib_path_)
        shutil.copy('lib/libblas.a', Globals.target_release_lib_path_)

    '''
    BUILD THE KTHOHR LIBRARY
    '''
    build_kthohr = True
    if build_kthohr:
        print('-- Building Kthohr')
        print('-- Cleaning up old files')
        kthohrFileName   = 'kthohr'   
        library = 'libkthohr.a'
 
        if os.path.exists(Globals.target_include_path_ + kthohrFileName):
            shutil.rmtree(Globals.target_include_path_ + kthohrFileName)  
        os.system('rm -rf ' + Globals.target_debug_lib_path_ + library)
        os.system('rm -rf ' + Globals.target_release_lib_path_ + library)
        if os.path.exists(kthohrFileName):
            os.system('rm -rf ' + kthohrFileName)
    
        print('-- Decompressing KThohr-MCMC')
        if os.path.exists(kthohrFileName + '.7z'):
            os.system('7za.exe x -bd -y ' + kthohrFileName + '.7z 1> kthohr_unzip.log 2>&1') 
        else:
            return Globals.PrintError('Unable to Decompress ' + kthohrFileName + '.7z')
   
        # Build KThor
        print('-- Building Kthohr static library')
        os.chdir(pwd)
        shutil.copy('CMakeLists.txt', kthohrFileName + '/CMakeLists.txt')
        os.chdir(kthohrFileName)
        print('-- cmake command: cmake ' + Globals.cmake_compiler_)
        os.system('cmake ' + Globals.cmake_compiler_)# + ' > casal2_cmake.log 2>&1')
        os.system('mingw32-make')# > casal2_build.log 2>&1')
    
        # Move Everything 
        print('-- Moving files across')   
        dir_util.copy_tree('include', Globals.target_include_path_ + kthohrFileName)
        shutil.copy(library, Globals.target_debug_lib_path_)
        shutil.copy(library, Globals.target_release_lib_path_)

    return True

