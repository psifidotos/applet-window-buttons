# Window Buttons Applet

This is a Plasma 5 applet that shows window buttons in your panels. This plasmoid is coming from [Latte land](https://phabricator.kde.org/source/latte-dock/repository/master/) but it can also support Plasma panels.

<p align="center">
<img src="https://i.imgur.com/4FItfte.gif" width="580"><br/>
<i>slide in/out animation</i>
</p>

<p align="center">
<img src="https://i.imgur.com/70qeMME.png" width="580"><br/>
<i>Breeze decoration</i>
</p>

<p align="center">
<img src="https://i.imgur.com/uEen6P0.png" width="580"><br/>
<i>BreezeEnhanced decoration</i>
</p>

<p align="center">
<img src="https://i.imgur.com/x3Fac7Y.png" width="580"><br/>
<i>Settings window</i>
</p>

# Features

- Current mode, in order to automatically update button appearance according to plasma current window decoration
- Choose the decoration theme you would like to use for the buttons
- Support Minimize/Maximize/Close/OnAllDesktops buttons
- Support icon-based Aurorae themes such as **.svg** and **.svgz** 
- Identify system's available decorations automatically
- Option to show buttons only when there is an active-maximized window in the current screen
- Slide out/in animation for showing/hiding buttons
- Reorder buttons according to user preference
- Various appearance settings in order to adjust margins. This way you can also change the button size
- Automatically discover Aurorae theme settings in order to make buttons look identical to window buttons

# Requires

- Qt >= 5.9
- KF5 >= 5.38
- Plasma >= 5.12
- KDecoration2 >= 5.12

**Qt elements**: Gui Qml Quick

**KF5 elements**: CoreAddons Declarative Plasma PlasmaQuick extra-cmake-modules


# Install

You can execute `sh install.sh` in the root directory as long as you have installed the previous mentioned development packages
