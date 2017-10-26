==================
(LV2) Qt Orion Pi
==================

This repository is a set of libraries and stand alone applications that support a mobile platform ORION. A documentation of the whole project can be found here [1]_ and [2]_\.

You can also visit a project's website `Orion Project <https://www.facebook.com/orion.pollub/>`_\.

Dependencies
============

* Qt-5.8 or newer [3]_
    Required modules:
        * Qt Network
        * Qt SerialPort
        * Qt Gamepad
        * Qt Widgets
        * Qt Gui
        * Qt UnitTest
        * Qt Core

* protocol-buffers 3.3 [4]_
    Serves as the basis for LV2-LV3 communication


* xboxdrv [5]_
    Allows using gamepads in Linux
    
    
Installation
============

Qt
--
The best way to install Qt is to download all required from the repositories. Make sure all symbolic links are set correctly. There should be no problems if packages are downloaded from the OS repos or installed via the installer provided by the Qt.

Protocol-buffers
----------------
The software requires protocol buffers in version 3.3. At this moment no proto file is generated during installation. Some automation will be implemented soon. Qt Orion Pi uses a statically linked lib.

Follow the standard instructions found in here: [4]_\. Configure the project as follows:

.. code-block:: bash

    # see set_environment_variables.sh for details
    ./configure --prefix=/opt/protobuf

Xboxdrv
-------
Install xboxdrv by following the instructions related to yours Linux flavor.

Qt Orion Pi
-----------
Make sure you have installed all required dependencies. Then follow the instructions:

.. code-block:: bash

    git clone https://github.com/rCorvidae/Qt-OrionPi.git
    mkdir build
    cd build
    source ../Qt-OrionPi/set_environment_variables.sh 
    qmake ../Qt-OrionPi/OrionPI.pro
    make -j4

If the compilation process is successful, the application will be present under *${HOME}/ORION/OrionPi* location.
Anytime the app is to be started it is required to execute environmental variables. The convenient way is to add it as *~/.bashrc* or make use of *ldconfig*/.

License
=======

This software is released under (unknown yet) license. To be selected. Anyway, it will be a license that is compliant with Qt's open-source licensing.


.. rubric:: Reference

.. [#] https://github.com/rCorvidae/OrionDoc
.. [#] http://oriondoc.readthedocs.io/en/latest/
.. [#] https://www.qt.io/
.. [#] https://github.com/google/protobuf
.. [#] https://github.com/xboxdrv/xboxdrv