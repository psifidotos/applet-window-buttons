### CHANGELOG

#### Version 0.10.1

* fix glitches with slide-in/out animations
* improve cpu and hard disk usage
* remove all workarounds related to faulty buttons painting
* respect and trust bug fixes in decorations provided with Plasma 5.23.2

#### Version 0.10.0

* support Plasma 5.23
* depend only on Plasma and KDecorations2 >= 5.23
* drop option for plasma panels to disable decoration on maximized windows

#### Version 0.9.0

* support Plasma 5.18 and 5.21
* support aurorae monochrome icons with Latte auto-color functionality
* support aurorae restore button
* fix, do not hide buttons for AlwaysVisible
* fix, do not hide button in Latte edit mode
* fix, improve default based on Plasma values
* fix, preserve empty space for hidden states when needed
* remove no needed workarounds

#### Version 0.8.1

* fix compatibility with Plasma 5.18 (Rodrigo Pedra Brum)
* never hide buttons for AlwaysVisible visibility state

#### Version 0.8.0

* fix which plasma theme colors are used for buttons when Colors:Plasma Theme is chosen
* do not show buttons that the current window has disabled them, for example a window is NOT Closable; Close button becomes hidden in such case
* hide buttons when the last active maximized window is minimized and there are no other shown windows
* improve configuration window texts
* create a new decoration only when it is really needed 
* reduce filesystem trackers for scheme colors files
* use shared decoration between all buttons
* set an object name for the shared decoration in order to be easily identified by KDecoration2 decorations

#### Version 0.7.0

* introduce a new Button Size percentage option and drop thickness margin in order to set the buttons size
* introduce Hidden State in order for the user to choose how buttons should behave when they should not be shown. Currently two states are supported: Slide out and Preserve Empty Space 
* introduce a new Color Scheme based on current Plasma Theme used and make it also default
* live preview for changes in the settings window
* do not crash when the selected decoration does not exist in the systemm, for example it was uninstalled
* update buttons for scheme changes

#### Version 0.6.0

* new visibility mode: At least one window is shown
* option to draw buttons in inactive state (for both c++ and aurorae decorations)
* fix the current buttons decoration discovery, the user can now change between different decorations and the buttons style is updated runtime
* hide when the last maximized window is closed and the visibility mode is ActiveMaximized
* fix multi-screens behavior for Latte panels
* fix typo that broke builds at some systems

#### Version 0.5.0

* provide a relevant background for window buttons in configuration window to fix contrast issues
* support new Latte v0.9 Communicator API
* support new Latte v0.9 Windows Tracking interface when in Latte dock/panel

#### Version 0.4

* add new KeepAbove button support (community contribution)
* merge different versions into a single one. One version for all Plasma versions >= 5.12
* close ComboBoxes after clicking

#### Version 0.3

* support new Latte coloring and Communicator mechanism
* fix spacing issues with Aurorae themes

#### Version 0.2

* Choose Colors to apply a color palette to your window buttons
* Three different Visibility modes, Always Visible/Active Window/Active and Maximized
* Multi-Screen aware, handle active windows only from current screen or from any screen
* Option to activate/deactivate borderless maximized windows and a third System Decide when
the checkbox is partially checked in order to play nice with Latte v0.8
* respect Fitt's Law and include padding and margins when hovering buttons
* fix, update buttons visual settings when the user changes the current decoration settings
* fix, respect all focus out/hover out events
* provide a Plasma 5.12 compatible version

#### Version 0.1

* Current mode, in order to automatically update button appearance according to plasma current window decoration
* Choose the decoration theme you would like to use for the buttons
* Support Minimize/Maximize/Close/OnAllDesktops buttons
* Support icon-based Aurorae themes such as **.svg** and **.svgz** 
* Identify system's available decorations automatically
* Option to show buttons only when there is an active-maximized window in the current screen
* Slide out/in animation for showing/hiding buttons
* Reorder buttons according to user preference
* Various appearance settings in order to adjust margins. This way you can also change the button size
* Automatically discover Aurorae theme settings in order to make buttons look identical to window buttons
