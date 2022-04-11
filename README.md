# CPPEuObject
CPPEuObject is the name of the project designed to use a CPP class instead of using the Euphoria Interpreter.

https://github.com/jmsck55/CPPEuObjectVS

Payment:

Give whatever money you would give me to single women, and to single women with children, and to orphans, especially of English origin, who do not have enough financial support.

Then support conservation of the environment, the "preservation, protection, or restoration of the natural environment and of wildlife." (Quoted from Microsoft Bing and Powered by Oxford Languages)

About:

This software library uses code from Rapid Deployment Software's (RDS) 3.1.1 version of The Euphoria Programming Language, or Euphoria v3.1.1 for short.
The webpage can be found at: https://rapideuphoria.com

The code is organized into header and source code files.  Some are written in the "C" programming language, the backend "be_" files, and some are written in the "C++" programming language, the frontend "*.hpp" files.

To use the library, you need to include one of the C++ header files (*.hpp) from within your C++ program.

The C source files (*.c) need to be compiled by a C compiler, but not linked, one after the other, until all the files are compiled.
Then, the C++ source files (*.cpp) need to be compiled by a C++ compiler, but not linked.
Then you need to link all the object files (*.o*) together to produce an executable file (*.exe on Windows).

NOTE: Some of the code is based on code from the 4.0.5 and 4.1 (pre-release) versions of Euphoria's source code (source directory under Euphoria's installed folder).

There are several macros that are defined and change the output files and the availability of certain functions.  For example, under Windows you have to define EWINDOWS in order for it to compile properly.

Updates:

The code is more secure with this update.

The goal is to make the code as secure as possible.

The tool UPX.exe can compress compiled binaries, on Windows and Linux (*.exe, *.dll, *.so), making them more secure, before you distribute them.

https://github.com/upx/upx/releases
