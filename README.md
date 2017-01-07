# RaspberryPi
A C++ library for controlling the RaspberryPi GPIO peripherals.

Inspired by the work of Gert Van de Loo on the Gertboard software and of course Gordan @drogon.net with the excellent WiringPi library.

Why did I write this
--------------------

I wanted something more low-level than the WiringPi and Gertboard software, but still easy to use. I wanted (and still do want) to build a weather station with anometer, rain guage, temperature and pressure using a RaspberryPi, and wanted a library that could sense all the items in the way I envisaged building them.

How to start
------------

  // First create the RaspberryPi...

  RaspberryPi * pi = PiFactory::makePi();

  // Get a pointer to the peripheral you need, e.g.

  GPIO * pGPIO = pi->getGpio();

  //See test.cpp for complete examples.

Notes
-----

Some of the peripherals aren't tested very well, simply because I don't yet have an oscilloscope. I have tested the PWM module using an LED and observed it getting brighter then dimmer by varying the PWM ratio, but have not confirmed the waveform looks OK using an oscilloscope. If you do have an oscilloscope, please test.

I haven't yet tried the SPI interface, again someone with an oscilloscope may be able to help.
