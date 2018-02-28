Streamline bare-metal RTOS example for Arm Cortex-A5
====================================================
An example showing how to configure the Streamline bare-metal agent, run it and then analyze it.

Purpose and scope
-----------------
This example demonstrates how to use the Streamline bare-metal agent (barman) within an RTOS-based system.
The example is based on the Keil "RTX" RTOS (version 4), and uses the RTXv4 "Traffic Lights" example as supplied with DS-5.
The code is built for Arm Cortex-A5 only but the interface with barman is mostly architecture agnostic.

This example is intended to be built with [ARM Compiler 5](https://developer.arm.com/products/software-development-tools/compilers/arm-compiler-5).
The executable is intended for running on the Cortex-A5 FVP model (provided with the DS-5 Ultimate Edition) and Versatile Express Cortex-A5 Coretile/FPGA hardware (available separately).

Ready-made launch configurations to connect to the Cortex-A5 FVP model and Versatile Express Cortex-A5 FPGA hardware are provided.
For a good debug view, the compiler's optimization level is set to `-O0` in the makefile.
You can change this to raise the optimization level to `-O2` or `-O3` for higher performance code generation, but at the cost of a worse debug view.

To support Streamline, some small additions and changes have been made to the original RTXv4 "Traffic Lights" example sources.
The changes are marked with "Streamline" to allow them to be easily found.

Importing into DS-5
-------------------
This example may be imported directly into DS-5 using the File -> Import... -> DS-5 -> Examples & Programming Libraries wizard, or by following these steps:

1. From DS-5 select "File->Import...".
2. Choose "General->Existing Projects into Workspace".
3. Set "Select root directory" to this directory and import the project.
4. Ensure "Search for nested projects" (to reveal the lower-level projects) and "Copy projects into workspace" are ticked
5. Click Import to import all three projects.

Configuring barman in Streamline
--------------------------------
1. Open Streamline and select the "Streamline" menu and then "Generate Barman Sources"
2. In the dialog choose the desired settings. For this example you must enable sending task information by changing "Max number of task information records"
   in the "Advanced options" section to 8 or higher. All other defaults are fine.
3. Now choose the cores, Cortex-A5 for this example, and then click "Next"
4. Choose some events. For example, "Clock - Cycles", "Instruction - Executed" and "Exception - Taken", then click "Next"
5. Click "Next" again.
6. Finally set the save location to the root directory of the `barman-RTX_ARM_A5_FVP_RTX_Traffic` project, click "Next" and then click "Finish"

NB: The example comes with preselected configuration which can be loaded using the "Streamline -> Generate barman sources from saved configuration" menu option.
    To use this configuration, load it and step through the wizard without making any additional changes, saving the output to the folder containing this example.

Building the example
--------------------
This example can be built with Arm Compiler 5 using the supplied Eclipse (makefile builder) project, or directly on the command-line with the supplied makefile using the `make` utility.

A prebuilt binary suitable for running on the FVP model or A5 Versatile Express board is provided in `prebuilt/Streamline_bare_metal_RTOS_Cortex-A5.axf`

Building on the command-line
----------------------------
To build the example on the command-line with the supplied `make` utility:

* On Windows, open a "DS-5 Command Prompt" from the Start menu, run the `select_toolchain` utility, and select `ARM Compiler 5 (DS-5 built-in)` from the list
* On Linux, run the `suite_exec` utility with the `--toolchain` option to select the compiler and start a shell configured for the suite environment, for example: `~/DS-5/bin/suite_exec --toolchain "ARM Compiler 5 (DS-5 built-in)" bash`

Then navigate to the `.../Streamline_bare_metal_RTXv4_Cortex_A5/RTOS/RTX/Boards/ARM/VE_A5/RTX_Traffic/` directory, and type: `make`
The usual `make` rules: `clean` and `all` are provided in the makefile.

Running the example in DS-5
---------------------------
1. From the "Run" menu, select "Debug Configurations..."
2. Expand the list of "DS-5 Debugger" configurations in the left-hand panel, and select "Streamline_bare_metal_RTOS_Cortex-A5_FVP".
In the "Connection" tab, notice the platform (Cortex-A5x1 FVP model) and debug activity ("Bare Metal Debug of Cortex-A5x1") is already selected.
In the "OS Awareness" tab, see that "Keil CMSIS-RTOS RTX OS" awareness is already selected too.
3. Click on "Debug" to start debugging.
4. Debugging requires the DS-5 Debug perspective. If the "Confirm Perspective Switch" dialog box opens, click on "Yes" to switch perspective.
DS-5 Debugger will launch the model, download the code/data from the executable (Streamline_bare_metal_RTOS_Cortex-A5) to the target,
load the debug information into the debugger, and run from the entry point, stopping at $Sub$$main() inside system_VE_A9_MP.c.
5. Run the executable by clicking on the green Continue button in the Debug Control view, or by pressing F8 on the keyboard.
A simulated traffic lights display will appear on the FVP model's screen. Watch the example run through its traffic light sequence.
You can request to "cross the road" by clicking on the small "USERSW 1" switch (left hand side) on the LCD.
6. After 20 seconds or so, stop execution by clicking on the yellow Interrupt button in the Debug Control view, or by pressing F9.
7. In the debug console type: `dump memory <FILE_PATH> 0x90000000 +0x300000`.
   `<FILE_PATH>` should be the location of some file where you want to store the capture. The file *must* end with the extension ".bin", ".raw" or ".barman"; eg "barman.bin".
   This will dump the contents of the model's DRAM area between 0x90000000-0x90300000 which is used for the memory buffer for the bare-metal agent.
   Alternatively you may use the usecase script `barman_in_memory_helpers.py` that is generated alongside the barman sources to export just the part of the buffer that contains data.
   For more information on using usecase scripts refer to the DS-5 Debugger documentation.

Analyzing the capture in Streamline
-----------------------------------
1. Click the "Import Capture File(s)" button and find the capture in the root of your workspace.
   Make sure you select "Bare-Metal Agent captures (*.raw; *.barman; *.bin)" in the file type filter selection.
2. Click the "Change analysis options and regenerate the report" button.
3. Click the "Add ELF image..." button and add `Streamline_bare_metal_RTOS_Cortex-A5.axf` from the example project.
4. Now click "Analyze".

Running the example on real hardware
------------------------------------
This example can be run on Versatile Express Cortex-A5 Coretile/FPGA hardware (available separately) via DSTREAM.
To do this, use the ready-made launch configuration "Streamline_bare_metal_RTOS_Cortex-A5_VX".
In the Connections panel, enter the USB: or TCP: IP address or name of your DSTREAM unit in the Debug Hardware Address field, or click on "Browse" to select one from a list.

Using the Screen View
---------------------
This example simulates a set of traffic lights and displays an animation of them on a screen.
The Cortex-A/R FVP models incorporate a model of an LCD display that this example makes use of via an LCD driver.
Real hardware might not have an LCD or driver, but you can use DS-5 Debugger's Screen view instead.
To use DS-5 Debugger Screen view, open the view in the DS-5 Debug perspective with Window->Show View->Screen, then configure it as:

Base Address = 0x80900000
Screen Width = 320
Screen Height = 240
Scan Line Alignment = 1
Pixel Type = BGR_16_565
Pixel Byte Order = Little Endian

The Screen view will refresh when code execution is stopped, or if timed-refresh is enabled.
