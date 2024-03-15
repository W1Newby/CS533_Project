# CS533_Project
This is the repo containing the code from my driver project.
To run this code, please download both folders onto a linux machine and follow the steps below (this is basically the script I wrote for myself for the video in case I got stage freight or forgetful.)

Commands/steps: 
1.	Verify that my device does not exist/isn’t installed:
a.	Command:
i.	Lsmod
b.	This driver is literally called “driver” look for that and make sure it’s not already there.  
2.	Install my driver:
a.	From my driver folder use the command:
i.	Sudo insmod driver.ko
3.	Repeat step 1, only this time result should be different, i.e. driver is installed:
a.	Command:
i.	Lsmod
4.	Verify my Major number by dmesg:
a.	Command:
i.	Dmesg
b.	Look for an output message stating what the major number is, save this for later, when defining the file to be used.  
5.	Make sure the device we’ll be interacting doesn’t already exist:
a.	It’s called “mydevice” so look int the /dev folder to make sure it’s not there.
b.	Command:
i.	Ls -l /dev
c.	If found, it’s probably fine, if desired remove with this Command:  
i.	Sudo rm /dev/mydevice
6.	Create a device for the driver to interact with:
a.	Command:
i.	Sudo mknod -m 777 /dev/mydevice c [major] [minor (0 in this case)]
b.	Some details about this command and its arguments:
i.	Mknod creates a directory device file and assigns it an i-node.
ii.	-m (mode) in this case we set it to 777 for convenience:
1.	Effectively all 7s permits any user to read, write, and execute the file.  
iii.	Location and name of file:
1.	Here it resides int the /dev folder and its name is “mydevice.”
iv.	Type of device:
1.	“c” this is for a char device, alternatively “b” should be used for a block device, and “p” for a FIFO device.  
v.	Major Number:
1.	From step 4 above.  This is important.  We need a the major number for the device to match the major number for the driver that communicates with it.  
vi.	Minor Number:
1.	Use zero in this case, effectively unused here.  
vii.	 
7.	Look at the device to make sure it’s there:
a.	Command:
i.	Ls -l /dev
b.	Should see “mydevice” on the list here somewhere.
8.	At this time the device (the special file in the /dev folder) and the driver should both be created and available.  Now we can force some text into the file using echo:
a.	Command:
i.	Sudo echo hello world >/dev/mydevice
ii.	
9.	We can also check to see if that device contains the text we forced into it:
a.	Command:
i.	Cat /dev/mydevice
b.	This my run an infinite loop.  Honestly, not sure why it does that.  Use “control c” to stop it.  I’m guessing this has something to do with the fact that this isn’t “really” a .txt file, but we’re using Linux system calls to force it to act like one and that may result in undesirable side effects.  
c.	Additionally, this doesn’t test out our driver’s open or close (maybe not even the read/write) it just forces text into the file.  
10.	Now let’s run the user space program:
a.	Command:
i.	Cd ~/home/test
11.	Run the userspace program:
a.	Command:
i.	./test
b.	This brings up the userspace program, pressing the various numerical keys associated with each action will execute the related function.  
c.	Pressing “1” uses the driver to open the device for reading and writing.  It’s worth trying to press “2” and or “3” (read and write, respectively) before “1” to see what happens when you try to read or write without first opening the file.  This should give some errors.  
d.	Pressing “4” will close the device and exit the user space program.  
12.	Remove the driver:
a.	Command:
i.	Rmmod driver
13.	Prove it:
a.	Lsmod.
14.	Remove the device:
a.	Sudo rm /dev/mydevicels
b.	
![image](https://github.com/W1Newby/CS533_Project/assets/104177092/7fcefcd3-69e8-4e0d-8ad5-7d4defccf5bd)


