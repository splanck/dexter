# Dexter
Third time is a charm! Time to write a working X86 kernel from the ground up! There are no grand plans for Dexter at the moment except to create a functional small toy kernel and operating system. We are making up the details as we go along. WHAT COULD GO WRONG? :)

# Status
I'm still working on the basics at the moment. Currently we have a simple bootloader, a bootable kernel that performs initialization of interrupts, heap memory management, paging, basic console output, a subset of string manipulation functions from the C standard library, read-only FAT16 filesystem support, and basic process and task management. I am now working on implementing a keyboard driver and building up the standard C library. My next agenda items are a simple shell and creating a more functional userland. You can see an extended todo list on the Dexter Wiki. 

# Documentation
A wiki has been setup to document Dexter which will be populated over the coming months. There isn't a lot of information posted yet. You can find build instructions, project goals, and other information on the wiki. but its usefulness is quite limited at the moment. You can find the wiki at https://www.dexterwiki.com.

# Boot image
The latest bootable image can be found under the images directory in this repo. This image has only been tested with the i386 version of the QEMU emulator, but may also work on other emulators such as Bochs or VirtualBox. You can emulate the image by installing QEMU i386 and executing 'make run' without building the toolchain or the Dexter source.

# Credit
The vast majority of Dexter code thus far is based on the excellent kernel development course from DragonZap. This course provides the comprehensive tutorial component lacking from https://osdev.org. Combining the multithreaded kernel from scratch course's tutorial style with the vast resources from osdev.org has been the perfect set of learning resources for me. The link below will take you straight to the course on the DragonZap website:

https://dragonzap.com/course/developing-a-multithreaded-kernel-from-scratch

Thanks Daniel!
