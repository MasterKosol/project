**Install Ros**

![MasterHead](https://media.licdn.com/dms/image/D5612AQGAh7Y8yCgjOg/article-cover_image-shrink_720_1280/0/1674818999890?e=2147483647&v=beta&t=3oVv2l3vFBiHVwNZdVPxGWqsnFO4CrW3fGixox_7l4c)

**1.Installation**

1.1Configure your Ubuntu repositories

Configure your Ubuntu repositories to allow "restricted," "universe," and "multiverse." You can follow the Ubuntu guide for instructions on doing this. 

**1.2 Setup your sources.list**

Setup your computer to accept software from packages.ros.org. 

```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```
**1.3 Set up your keys**
```
sudo apt install curl
```
```
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
```
**1.4 Installation**

First, make sure your Debian package index is up-to-date: 
```
sudo apt update
```
Now pick how much of ROS you would like to install. 

**Desktop-Full Install: (Recommended) :** Everything in **Desktop** plus 2D/3D simulators and 2D/3D perception packages 
```
sudo apt install ros-noetic-desktop-full
```
**Desktop Install: Everything** in **ROS-Base** plus tools like rqt and rviz
```
sudo apt install ros-noetic-desktop
```
**ROS-Base: (Bare Bones)** ROS packaging, build, and communication libraries. No GUI tools. 
```
sudo apt install ros-noetic-ros-base
```
There are even more packages available in ROS. You can always install a specific package directly. 
```
sudo apt install ros-noetic-PACKAGE
```
e.g.
```
sudo apt install ros-noetic-slam-gmapping
```
To find available packages, see ROS Index or use: 
```
apt search ros-noetic
```
**1.5 Environment setup**

You must source this script in every **bash** terminal you use ROS in. 
```
source /opt/ros/noetic/setup.bash
```
It can be convenient to automatically source this script every time a new shell is launched. These commands will do that for you. 

**Bash !!**

If you have more than one ROS distribution installed, ~/.bashrc must only source the setup.bash for the version you are currently using. 
```
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```
**zsh**
```
echo "source /opt/ros/noetic/setup.zsh" >> ~/.zshrc
source ~/.zshrc
```
**1.6 Dependencies for building packages**

Up to now you have installed what you need to run the core ROS packages. To create and manage your own ROS workspaces, there are various tools and requirements that are distributed separately. For example, rosinstall is a frequently used command-line tool that enables you to easily download many source trees for ROS packages with one command. 

To install this tool and other dependencies for building ROS packages, run: 
```
sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
```
**1.6.1 Initialize rosdep**

Before you can use many ROS tools, you will need to initialize rosdep. rosdep enables you to easily install system dependencies for source you want to compile and is required to run some core components in ROS. If you have not yet installed rosdep, do so as follows. 
```
sudo apt install python3-rosdep
```
With the following, you can initialize rosdep. 
```
sudo rosdep init
rosdep update
```
**Install Arduino IDE**


**https://www.youtube.com/watch?v=Z8aYYSk4i9o&t=261**

**Install rosserial_lib in arduino IDE**

Reboot the computer, open the arduino IDE, you would not see the new ros_lib in the example manual. You need goto sketch>include library>Manager Libraries>Library Manager, search for "rosserial" and install.
![MasterHead](https://raw.githubusercontent.com/ctsuu/Arduino-in-ROS/master/rosserial_arduino_library.png)
Now you will see the rosserial arduino library under examples> examples from custom libraries. Open any of the example, make sure the following code is there:
```
#include <ros.h>
#include <std_msgs/String.h>
```
Verify and upload. That it is.

**Installing the Software**

**Installing Binaries on the ROS workstation**

You can install rosserial for Arduino by running: 
```
sudo apt-get install ros-${ROS_DISTRO}-rosserial-arduino
sudo apt-get install ros-${ROS_DISTRO}-rosserial
```

**Install vscode**

**Install Using the GUI**

Follow the steps below to install vscode using Ubuntu Software Center:

**1. Open Ubuntu Software Center**

Click the **Show Applications** button and open the **Ubuntu Software** Center app.
![MasterHead](https://phoenixnap.com/kb/wp-content/uploads/2022/05/open-ubuntu-software-center.png)
**2. Find Visual Studio Code**

Click the **Search** button, and type vscode in the search bar. Click **code** on the list to open the app page.
![MasterHead](https://phoenixnap.com/kb/wp-content/uploads/2022/05/find-vscode-in-ubuntu-software-center.png)
**3. Install vscode**

Click the **Install** button and, when prompted, type in your administrator password to start the installation.
![MasterHead](https://phoenixnap.com/kb/wp-content/uploads/2022/05/install-vscode-using-the-gui.png)
**How to Start vscode on Ubuntu?**

Click the **Show Applications** button and locate Visual Studio Code in the app list.
![MasterHead](https://phoenixnap.com/kb/wp-content/uploads/2022/05/open-vscode-ubuntu.png)
Click the app to open it and start customizing the appearance, adding extensions for the programming languages you need, etc.
![MasterHead](https://phoenixnap.com/kb/wp-content/uploads/2022/05/get-started-with-vscode-ubuntu.png)
