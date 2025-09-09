##############################################################################
Chapter 15 LVGL Picture
##############################################################################

In this chapter, you will learn how to read image data from the SD card and display it on the TFT screen.

Project 15.1 LVGL Picture
***************************************

Component List 
=====================================

:combo:`red font-bolder:As of the time of writing, the Freenove Media Kit for ESP32-S3 comes in two different models with varying screen sizes. However, this tutorial applies to both versions.`

.. table:: 
    :align: center
    :class: table-line
    :width: 80%

    +------------------------------------+----------------+
    | Freenove Media Kit for ESP32-S3 x1 | USB cable x1   |
    |                                    |                |
    | |Chapter10_07|                     | |Chapter10_08| |
    +------------------------------------+----------------+
    | Extension Board x1(1.14 inch/3.5 inch)              |
    |                                                     |
    | |Chapter01_11|                                      |
    +----------------------+------------------------------+
    | SD card x1           | Card reader x1 (random color)|
    |                      |                              |
    | |Chapter10_09|       | |Chapter04_01|               |
    +----------------------+------------------------------+

.. |Chapter10_07| image:: ../_static/imgs/Main/10_Record_Test/Chapter10_07.png
.. |Chapter10_08| image:: ../_static/imgs/Main/10_Record_Test/Chapter10_08.png
.. |Chapter01_11| image:: ../_static/imgs/Main/1_LEDPixel_Test/Chapter01_11.png
.. |Chapter10_09| image:: ../_static/imgs/Main/10_Record_Test/Chapter10_09.png
.. |Chapter04_01| image:: ../_static/imgs/Main/4_SD_Card_Read_&_Write_Test/Chapter04_01.png

Circuit
======================================

Before connecting the USB cable, insert the SD card into the SD card slot on the back of the ESP32-S3.

.. image:: ../_static/imgs/Main/1_LEDPixel_Test/Chapter01_12.png
    :align: center

Connect Freenove ESP32-S3 to the computer using the USB cable. 

.. image:: ../_static/imgs/Main/1_LEDPixel_Test/Chapter01_04.png
    :align: center

Sketch
===================================

Sketch_15_LVGL_Picture
----------------------------------

The following is the program code:

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_15_Lvgl_Picture/Sketch_15_Lvgl_Picture.ino
    :linenos:
    :language: c
    :dedent:

Include the required libraries.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_15_Lvgl_Picture/Sketch_15_Lvgl_Picture.ino
    :linenos:
    :language: c
    :lines: 10-11
    :dedent:

Define SD card pins.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_15_Lvgl_Picture/Sketch_15_Lvgl_Picture.ino
    :linenos:
    :language: c
    :lines: 13-15
    :dedent:

Declare TFT screen object.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_15_Lvgl_Picture/Sketch_15_Lvgl_Picture.ino
    :linenos:
    :language: c
    :lines: 17-17
    :dedent:

Initialize the SD card.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_15_Lvgl_Picture/Sketch_15_Lvgl_Picture.ino
    :linenos:
    :language: c
    :lines: 24-24
    :dedent:

TFT screen initialization.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_15_Lvgl_Picture/Sketch_15_Lvgl_Picture.ino
    :linenos:
    :language: c
    :lines: 28-28
    :dedent:

Initialize the UI component for image browsing

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_15_Lvgl_Picture/Sketch_15_Lvgl_Picture.ino
    :linenos:
    :language: c
    :lines: 38-39
    :dedent:

If you are interesting in the implementation of functions, you can check them out here.

.. image:: ../_static/imgs/Main/15_LVGL_Picture/Chapter15_00.png
    :align: center

Before uploading the code, you need to use the sample code in :ref:`Chapter 13 <fnk0102/codes/main/13_lvgl_ledpixel:chapter 13 lvgl ledpixel>` to take a picture and store it in the SD card. Make sure there are image files in the picture folder.

.. image:: ../_static/imgs/Main/15_LVGL_Picture/Chapter15_01.png
    :align: center

.. image:: ../_static/imgs/Main/15_LVGL_Picture/Chapter15_02.png
    :align: center

It is necessary to change the settings in Arduino IDE before clicking the Uploading button, as shown below.

.. caution::

    :combo:`red font-bolder:Incorrect settings will result in compilation error or uploading failure. To achieve desired result, please configure exactly the same as below.`

.. image:: ../_static/imgs/Main/15_LVGL_Picture/Chapter15_03.png
    :align: center

After uploading the code, the TFT display will show images from the SD card.

.. note::
    
    :combo:`red font-bolder:Here, take 1.14 inches as an example; the usage is the same for 3.5 inches.`

The 5-way navigation key operates as follows:

Switches 5 (Right) and 4 (Left): Navigate horizontally between selection frames

Button 1 (Center): Confirms selection (Enter key)

Switches 2/3: Invalid in this project

**Refer to the figure below (right) for the button numbering.**

.. image:: ../_static/imgs/Main/15_LVGL_Picture/Chapter15_04.png
    :align: center

**If you have any concerns, please feel free to contact us via** support@freenove.com
