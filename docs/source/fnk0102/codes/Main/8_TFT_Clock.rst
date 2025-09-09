##############################################################################
Chapter 8 TFT Clock
##############################################################################

In this section, we will learn how to use the TFT display.

Project 8.1 TFT Clock
**********************************

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
    | |Chapter01_00|                     | |Chapter01_01| |
    +------------------------------------+----------------+
    | Extension Board x1(1.14 inch/3.5 inch)              |
    |                                                     |
    | |Chapter01_11|                                      |
    +-----------------------------------------------------+

.. |Chapter01_00| image:: ../_static/imgs/Main/1_LEDPixel_Test/Chapter01_00.png
.. |Chapter01_01| image:: ../_static/imgs/Main/1_LEDPixel_Test/Chapter01_01.png
.. |Chapter01_11| image:: ../_static/imgs/Main/1_LEDPixel_Test/Chapter01_11.png

Circuit
=================================

Connect Freenove Media Kit for ESP32-S3 to your computer using the USB cable.

.. image:: ../_static/imgs/Main/1_LEDPixel_Test/Chapter01_04.png
    :align: center

Sketch
=================================

Before uploading the code, you need to isntall the TFT_eSPI library and make some configuration.

How to install the library
---------------------------------

Open Arduino IDE, click Sketch -> Include Library -> Add .ZIP Library. In the pop-up window, find the file named **"Freenove_Media_Kit_for_ESP32-S3\\Libraries\\TFT_eSPI.Zip"**, which locates in this directory, and click OPEN.

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_00.png
    :align: center

Select **TFT_eSPI.Zip** and click Open.

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_01.png
    :align: center

Install the **TFT_eSPI_Setups** library with the same approach. Select TFT_eSPI_Setups.zip that locates in this directory, and click OPEN.

.. note::
    
    :combo:`red font-bolder:TFT_eSPI_Setups.Zip and TFT_eSPI.Zip are different and both are needed.`

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_02.png
    :align: center

Select **TFT_eSPI_Setups.Zip** and click Open.

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_03.png
    :align: center

How to configure (Important)
---------------------------------

Open This PC, input **%USERPROFILE%\\Documents\\Arduino\\libraries\\TFT_eSPI_Setups** and press the **Enter** key.

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_04.png
    :align: center

Right click User_Setup_Select.h, click **Edit**.

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_05.png
    :align: center

Uncomment the corresponding macro definition based on the model purchased.

If it is **1.14inch**, configure as below:

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_06.png
    :align: center

If it is **3.5inch**, configure as below:

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_09.png
    :align: center

Save the change and exit the file.

.. warning::
    
    :combo:`red font-bolder:If the following updating message shows up, click LATER. Updating the TFT_eSPI library will reset all related configurations. If you click INSTALL, follow the aforementioned steps to re-add the header file to ensure proper project operation.`

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_07.png
    :align: center

Sketch_08_TFT_Clock
--------------------------------

The following is the program code:

.. literalinclude:: ../../../freenove_Kit/Sketches/Sketch_08_TFT_Clock/Sketch_08_TFT_Clock.ino
    :linenos:
    :language: c
    :dedent:

After uploading the code, the TFT screen will display a real-time clock along with the text "Freenove", as shown in the image below:

.. image:: ../_static/imgs/Main/8_TFT_Clock/Chapter08_08.png
    :align: center

For a more in-depth understanding of how the SPI protocol drives the TFT screen, please refer to `TFT_eSPI <https://github.com/Bodmer/TFT_eSPI>`_.

**If you have any concerns, please feel free to contact us via** support@freenove.com