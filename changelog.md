# 4swbbevb-power-average-current-mode-control, release v1.0.0

### Release Highlights
This is the initial release demonstrating the basic control loop implementation of a digital average current mode control loop regulating the output voltage of a 4-switch buck boost converter.
This example is part #2 of a series of code examples for the 4-Switch Buck-Boost Converter.

### Features Added\Updated

* User-button short pressed: power controller is being switched on/off
* User-button pressed long (>1s): power controller switches between open loop and closed average current control mode loop
* Green LED on: PWM enabled (power stage is running)
* Green LED off:  PWM disabled (power stage is not running)
* Red LED off: closed loop
* Red LED slow blinking: open loop
* Red LED on: error
* An external potentiometer can be used to adjust the reference
* The Power Board Visualizer GUI connected to the serial interface can be used to visualize and adjust the behaviour

### Supported Hardware:
- Four-Switch Buck-Boost Development Board, Part-No. [EV44M28A](https://www.microchip.com/developmenttools/ProductDetails/PartNO/EV44M28A)
- dsPIC33CK256MP506 Digital Power PIM, Part-No. [MA330048](https://www.microchip.com/MA330048)

### Needed Software:
- [Power Board Visualizer GUI](https://www.microchip.com/SWLibraryWeb/product.aspx?product=POWER_BOARD_VISUALIZER)
- [MPLAB&reg; X IDE v5.45](https://www.microchip.com/mplabx-ide-windows-installer)
- [MPLAB&reg; XC16 Compiler v1.70](https://www.microchip.com/mplabxc16windows)
- [Microchip Code Configurator v4.0.2](https://www.microchip.com/mplab/mplab-code-configurator)
- [Digital Compensator Design Tool](https://www.microchip.com/developmenttools/ProductDetails/DCDT)
