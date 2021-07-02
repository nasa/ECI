Structure of the CI directory
-----------------------------
To help aid the automation of the ECI build tests,
the CI directory contains a CMake build system for
preforming all the steps necessary to build the cFS
with a particular app. Since these are build test,
these CMake files can also be used as an example
of how to integrate your ECI app with the cFE.

A description of each component is as follows:

`common\_defs/`: Contains cFS configuration files
shared by all of our tests. These files come directly
from the cFE repository and
haven't been altered for any particular test.

`<project_name>/`: Contains the project specific
configurations required for building that project.
Within each project directory, there is a specialized
CMakeLists.txt which sets up details about the project
then calls `setup_cfe.cmake` to install all of the
generalized files. Also within this directory is
a modified lab\_sch scheduler header and a definitions
directory containing project specific files for the
cFE to use on build.

`setup_cfe.cmake`: A common CMake file to construct
the cFE environment. This will be called by a project's
CMakeLists.txt file.

How to use the prebuild CMake
--------------------------------

To build a project simply navigate into the project
directory and execute `cmake -B build ./`. After a
successful install, you can then run `make install`
in your cfe directory to preform the final build.
