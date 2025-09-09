##############################################################################
Chapter 17 LVGL Sound Recorder
##############################################################################

In previous chapters, we learned about the recording functionality on the Freenove Media Kit for ESP32. In this chapter, we will explore how to integrate it with LVGL.

Project 17.1 LVGL Sound Recorder
****************************************

Capture image data using the camera module and display it on the TFT screen.

Component List 
=================================

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
============================

Sketch_17_LVGL_Sound_Recorder
------------------------------------

The following is the program code:

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_17_Lvgl_Sound_Recorder/Sketch_17_Lvgl_Sound_Recorder.ino
    :linenos:
    :language: c
    :dedent:

Include the required libraries.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_17_Lvgl_Sound_Recorder/Sketch_17_Lvgl_Sound_Recorder.ino
    :linenos:
    :language: c
    :lines: 10-10
    :dedent:

Define SD card, I2S and microphone pins.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_17_Lvgl_Sound_Recorder/Sketch_17_Lvgl_Sound_Recorder.ino
    :linenos:
    :language: c
    :lines: 12-20
    :dedent:

Declare TFT screen object.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_17_Lvgl_Sound_Recorder/Sketch_17_Lvgl_Sound_Recorder.ino
    :linenos:
    :language: c
    :lines: 22-22
    :dedent:

Initialize the audio input, audio output and SD card interface, and set the output volume to level 21.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_17_Lvgl_Sound_Recorder/Sketch_17_Lvgl_Sound_Recorder.ino
    :linenos:
    :language: c
    :lines: 31-34
    :dedent:

Initialize TFT screen 

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_17_Lvgl_Sound_Recorder/Sketch_17_Lvgl_Sound_Recorder.ino
    :linenos:
    :language: c
    :lines: 40-40
    :dedent:

Initialize and load the UI component for sound recording.

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_17_Lvgl_Sound_Recorder/Sketch_17_Lvgl_Sound_Recorder.ino
    :linenos:
    :language: c
    :lines: 50-51
    :dedent:

If you are interesting in the implementation of functions, you can check them out here.

.. image:: ../_static/imgs/Main/17_LVGL_Sound_Recorder/Chapter17_00.png
    :align: center

After uploading the sketch, you will see the following interface.

.. note::
    
    :combo:`red font-bolder:Here, take 1.14 inches as an example; the usage is the same for 3.5 inches.`

.. image:: ../_static/imgs/Main/17_LVGL_Sound_Recorder/Chapter17_01.png
    :align: center

Pressing different directions on the 5-way navigation switch will trigger corresponding functions:

Switches 2 & 3 switch between recording and playback modes.

Button 1 serves as the confirmation key.

Switches 4 & 5 are disabled (no function).

.. note::
    
    :combo:`red font-bolder:In recording mode, long-press Button 1 to start recording. The recording duration depends on how long Button 1 is held down.`

.. image:: ../_static/imgs/Main/17_LVGL_Sound_Recorder/Chapter17_02.png
    :align: center

