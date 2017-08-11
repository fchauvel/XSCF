XCSF
====

.. image:: https://app.wercker.com/status/87ebe60ed512a2bd4c033ce2ab3d458c/s/master
   :target: https://app.wercker.com/project/byKey/87ebe60ed512a2bd4c033ce2ab3d458c

.. image:: https://img.shields.io/codecov/c/github/fchauvel/XSCF/master.svg
   :target: https://codecov.io/gh/fchauvel/XSCF

.. image:: https://img.shields.io/codacy/grade/39d92ac4cff743819043c34994b30112.svg
   :target: https://www.codacy.com/app/fchauvel/XSCF

This is a simple C++ implementation of accuracy-based learning
classifiers systems, which operate on real-valued domain, inspired by
S. Wilson's approach [Wilson_2002]_.

Building the Binaries
---------------------

To build XCSF from the source code, you only need a C++ compiler, and
Make, a tool to automate compulation. As both are on most
repositories, you may use commands such as (on Debian):

.. code-block:: console

   $ apt-get install g++
   $ apt-get install make

XCSF depends on the CppUTest_ library, which you will need to install
if you want to build the application from the sources.

.. code-block:: console

   $ apt-get install CppUTest

You can now build the binaries by using:

.. code-block:: console

   $ make


References
----------

.. [Wilson_2002] Stewart W. Wilson. Classifiers that Approximate
                 Functions. Natural Computing. 1: 211 |--| 233, 2002.

.. |--| unicode:: U+2013
    :trim:

.. _CppUTest: https://cpputest.github.io/
