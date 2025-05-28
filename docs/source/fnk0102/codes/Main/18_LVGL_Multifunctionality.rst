##############################################################################
Chapter 18 LVGL Multifunctionality
##############################################################################

In this chapter, we will combine the functionalities covered in previous sections.

Project 18.1 LVGL Multifunctionality
***********************************************

Component List 
=================================

+-------------------+-------------------+
| SD card x1        | USB cable x1      |
|                   |                   |
| |Chapter07_00|    | |Chapter07_01|    |
+-------------------+-------------------+
| Freenove Media Kit for ESP32-S3 x1    |
|                                       |
| |Chapter07_02|                        |
+---------------------------------------+

.. |Chapter07_00| image:: ../_static/imgs/Main/7_Video_Web_Server/Chapter07_00.png
.. |Chapter07_01| image:: ../_static/imgs/Main/7_Video_Web_Server/Chapter07_01.png
.. |Chapter07_02| image:: ../_static/imgs/Main/7_Video_Web_Server/Chapter07_02.png

Circuit
======================================

Connect Freenove Media Kit for ESP32-S3 to your computer using the USB cable.

.. image:: ../_static/imgs/Main/2_Battery_Voltage_Detection/Chapter02_03.png
    :align: center

Before connecting the USB cable, insert the SD card into the SD card slot on the back of the ESP32-S3.

.. image:: ../_static/imgs/Main/7_Video_Web_Server/Chapter07_03.png
    :align: center

Sketch
============================

Sketch_18_LVGL_Multifunctionality
---------------------------------------

The following is the program code:

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_18_Lvgl_Multifunctionality/Sketch_18_Lvgl_Multifunctionality.ino
    :linenos:
    :language: c
    :dedent:

It is necessary to change the settings in Arduino IDE before clicking the Uploading button, as shown below.

:red:`Caution: Incorrect settings will result in compilation error or uploading failure. To achieve desired result, please configure exactly the same as below.`

.. image:: ../_static/imgs/Main/18_LVGL_Multifunctionality/Chapter18_00.png
    :align: center

After uploading the code, you can see the following interface on the screen.

.. image:: ../_static/imgs/Main/18_LVGL_Multifunctionality/Chapter18_01.png
    :align: center

The 5-way navigation switch triggers different functions based on directional input:

    - Switches 2 & 3 - Cycle through available function
    
    - Button 1 - Confirm selection (enters chosen mode)

Note: Operational logic within each function remains unchanged from previous implementations.

To exit current function and return to main menu:

    - Deselect all components (no selection box is visible) 
    
    - Switch switches 4 or 5 to return to home interface

.. image:: ../_static/imgs/Main/18_LVGL_Multifunctionality/Chapter18_02.png
    :align: center

**If you have any concerns, please feel free to contact us via** support@freenove.com