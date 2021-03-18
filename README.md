meta-allwinner-hx
----

This meta layer is mainly a mix of `meta-sunxi` and `armbian`. Because of that
this is not a light-weight kernel but it's meant to support as much peripherals
as possible. For this reason, if you need to a light-weight kernel, then you
need to strip out the kernel configuration manually. 

> Note: The master version always points to the latest Yocto version.
If you want to use a specific version then `git checkout` to that
specific version, but be aware that older versions may not be updated.

**Current master branch is based on `gatesgarth`.**

This meta layer supports only boards with the allwinner H2, H3 and H5 cpus.
The boards that are supported are the same ones that supported in
[armbian](https://www.armbian.com/download/).

To view the list of the supported boards run this command see below
in the "How to use the layer".

The way this meta layer works is that it uses the u-boot and kernel patches
from the armbian distro. The patches are located in:
* `u-boot`: recipes-bsp/u-boot/files/patches
* `kernel`: recipes-kernel/linux/linux-stable/patches

Also the patcher is ported from armbian and actually is the same for u-boot
and the kernel and is lcated in `scripts/armbian-patcher.sh`.

> Note: Not all of the above boards are tested, because I don't have them.
I'm only testing with `nanopi-k1-plus` and occasionally `nanopi-neo`, 
`nanopi-neo2` and `nanopi-duo`.

## Important updates notes
In here I list only the latest update. To have a look to also other update
descriptions then have a look in the `UPDATES.md` file.

#### 18.03.2021
Paul Rathgeb has pushed patches that solved the `S = ${B}` issue that was introduced
because of how armbian patches wifi drivers, that assume that build directory is the
same with source. From now on it should be possible to build kernel modules.

## How to use the layer
Create a folder for your project, then create a folder inside and name it
`sources`. You _have_ to use that name.

#### Cloning the needed layers
Then `git clone` this repo inside with `poky` and `meta-openembedded`.

```sh
cd sources
git clone https://gitlab.com/dimtass/meta-allwinner-hx.git
git clone --depth 1 -b gatesgarth https://git.yoctoproject.org/git/poky
git clone --depth 1 -b gatesgarth https://github.com/openembedded/meta-openembedded.git
```

> Note: This layer is compatible with `warrior`, `zeus`, `dunfell` and `gatesgarth`.
But be aware that the compatibility may be broken with older versions than `gatesgarth`.

#### Setting the environment
Then from the `top` directory that includes the sources run this command:
```sh
ln -s sources/meta-allwinner-hx/scripts/setup-environment.sh .
ln -s sources/meta-allwinner-hx/scripts/flash_sd.sh .
ln -s sources/meta-allwinner-hx/scripts/list-machines.sh .
ln -s sources/meta-allwinner-hx/Docker/Dockerfile .
```

Then your top dir contects should look like this:
```sh
flash_sd.sh
list-machines.sh
setup-environment.sh
sources
```

> Note: at this point you need to source the `setup-environment.sh` and set the build
folder. To do this read a bit further down in `Supported DISTRO(s) and MACHINE(s)` on
how to do this, before proceed to the next paragraph.

Now in your `build/conf/local.conf` file you can choose which kernel you want to build.
By default the `linux-stable` 4.19 version is build. In case you want to build the RT
kernel then see next section. Also, by default in this image all the Linux firmware
files are added in the image. If you want to save ~500MB then you can comment out the next
line, but then you need to add the specific firmware for your `MACHINE` with your own
recipe:
```sh
IMAGE_INSTALL += "armbian-firmware"
```

By default this repo is applying all the extra wifi drivers patches of the armbian images.
This is enabled by default in the `local.conf` file and you can disable this
by setting the following variables to `no` instead of `yes`, which is the default.
```py
EXTRAWIFI = "yes"
```

#### Supported machines/boards
To view the list of the supported boards run this command:
```sh
./list-machines.sh
```

Then depending on the board you have, you need to set the `MACHINE` variable to one
from the support list.

#### Supported DISTROs
Since the `zeus` yocto version the mesa is updated to version 19.1.x which supports
the Lima DRM. Also the Armbian patches have support for Lima, therefore you can
build X11/Wayland images with graphic acceleration from Lima.

Currently, this layer supports the following DISTROs:
* `allwinner-distro-tiny`: only console, no GUI, initramfs.
* `allwinner-distro-console`: only console, no GUI.
* `allwinner-distro-wayland`: Supports Wyland with Weston as composer
* `allwinner-distro-x11`: Supports xserver-xorg
* `allwinner-distro-xwayland`: Supports Wayland with Weston as composer and X11.

To build an image you need to select one of the above distros when setting the
environment. By default, the `local.conf` file is set to
```sh
DISTRO ?= "poky"
```

This is will be override when setting the environment with the `setup-environment.sh`
script. But this requires you to select one of the above distros by prepending this:
```sh
DISTRO=allwinner-distro-wayland
```

> Note: there's an example in the next section

#### Supported images
Currently there are a few images in this repo, but I can only verify that the console
image is working properly. The rest of the images are for supporting GUI (Wayland and
X11). This is a list of the images:
* `allwinner-console-image`: Image with only debug console support (no GUI)
* `allwinner-multimedia-image`: This image supports both X11 and Wayland and installs
also `gstreamer1.0` with all plugins 
* `allwineer-testing-image`: An image that installs various testing tools.

#### Control image extra space
Currently the extra free space for the image is set to 4GB. You can control the size
with the `ROOT_EXTRA_SPACE` variable in `meta-allwinner-hx/classes/allwinner-wks-defs.inc`.
If you want to remove all additional space then set it to `0`.

#### Lima DRM support
Wayland seems to be working, but I can verify that there are some issues, like keyboard
is not working properly and that Weston seems to consume the 100% of the CPU every
2-3 secs, which makes GUI unusable.

This kernel supports the Lima DRM. The module is loaded when the kernel boots, but
I wasn't able to verify that works as `kmscube` returns an error when running from
the debug port. I can't tell if this issue is because the command is not running
from the Weston terminal, but this is the error that I get.
```sh
kmscube -d -D /dev/dri/renderD128
  could not open drm device
  failed to initialize legacy DRM
```

#### Supported DISTRO(s) and MACHINE(s)
To set the build environment you need to source the `setup-environment.sh` script
and set the `DISTRO` and `MACHINE` variables. As mentioned above you can use the
`list-machines.sh` script to list the supported machines. Also the supported `DISTRO`s
are listed above.

There are a few examples to build various distros for the `nanopi-k1-plus`:

Build an image with only console support
```sh
DISTRO=allwinner-distro-console MACHINE=nanopi-k1-plus source ./setup-environment.sh build
```

Build an image with Wayland
```sh
DISTRO=allwinner-distro-wayland MACHINE=nanopi-k1-plus source ./setup-environment.sh build
```

Build an image with X11
```sh
DISTRO=allwinner-distro-x11 MACHINE=nanopi-k1-plus source ./setup-environment.sh build
```

After the environment is set you can start building the image:
```sh
bitbake allwinner-multimedia-image
```

> Note: Added a new tiny image distro. This image boot in 1-2 seconds on the nanopi-k1-plus.
This is a initramfs image, therefore you need to create your own `/init` script to mount the
bigger rootfs partition. This is not currenty done and this image is provided as a template
to create fast boot ditros. In order to use it run the following commands:

```sh
DISTRO=allwinner-distro-tiny MACHINE=nanopi-k1-plus source ./setup-environment.sh build
bitbake allwinner-tiny-console-image
```

In this case this will create a `.wic.bz2` image inside your `build/tmp/deploy/images/nanopi-k1-plus`.

## Supported Kernels
The default kernel version for this version is 5.10.y. Also the PREEMPT-RT kernel
is supported, but it might be a slight different version compared to the SMP,
depending the current rt release.

To enable another kernel you need to edit your `build/conf/local.conf` and select
the kernel you want. The available options are:

* orange-pi megous 5.10.y
```
PREFERRED_PROVIDER_virtual/kernel = "linux-megous"
PREFERRED_VERSION_linux-stable = "5.10%"
```

* linux-stable-rt 5.10.y
```
PREFERRED_PROVIDER_virtual/kernel = "linux-megous-rt"
PREFERRED_VERSION_linux-megous-rt = "5.10%"
```

> Note: You can now go back to previous kernel versions using git tags

#### Current versions
* 5.10.18
* 5.10.18-rt32

## Build the SDK
There's a known issue that some bb recipes that are used while the SDK is built
conflict with some packages. In this BSP the packages that are conflict are the
listed in the `SDK_CONFLICT_PACKAGES` variable, which is located in `meta-allwinner-hx/classes/package-groups.inc`.
Therefore, in case you add more packages in the image and the SDK is failing, then
you can add them in the `SDK_CONFLICT_PACKAGES`.

Then, when you setup the environment to build the image using the `meta-allwinner-hx/scripts/setup-environment.sh`
script, you can control if those packages will be added with the `REMOVE_SDK_CONFLICT_PKGS`
variable in the `local.conf`. By default this is set to `0`, but when you build the
SDK you need to set that to `1`.

To bulid the SDK run this command (after the environment is set)
```sh
bitbake -c populate_sdk allwinner-console-image
```

## Overlays
This layer supports overlays for the allwinners boards. In order to use them you need
to edit the `recipes-bsp/u-boot/files/allwinnerEnv.txt` file or even better create
a new layer with your custom cofiguration and override the `allwinnerEnv.txt` file by
pointing to your custom file in your `recipes-bsp/u-boot/u-boot_2018.11.bbappend`
with this line:

```sh
SRC_URI += "file://allwinnerEnv.txt"
```

Of course, you need to create this file and place it in your layer file folder.
In that file you need to edit it and add the overlays you need, for example:

```sh
extra_bootargs=
rootfstype=ext4
verbosity=d
overlays=sun8i-h3-i2c0 sun8i-h3-spi-spidev
param_spidev_spi_bus=0
```

Some overlays (like the `spi-spidev`) get parameters as shown above. For more details
on the allwinner overlays always refer to the decumentation [here](https://docs.armbian.com/Hardware_Allwinner_overlays/)

## WiFi networking
If your board has only a wifi network then you can add the `SSID` and the `PSK` password
in the `build/conf/local.conf` and build the image. You can remove the comment on those
two lines in the `build/conf/local.conf` and the proper values for your network.
```sh
SSID = "YOUR_SSID"
PSK = "YOUR_SSID_PASSWORD"
```

For some reason it seems that the `wpa_supplicant@wlan0.service` service is not installed
and started automatically. Because of that, on the first boot after the SD flash you need
to run the following command manually and after that it works fine forever.
```sh
systemctl enable wpa_supplicant@wlan0
```

## Flashing the image
After the image is build, you can use `bmaptool` to flash the image on your SD card.
To this you first need to install `bmap-tools`.
```sh
sudo apt-get install bmap-tools
```

Then you need to run `lsblk` to find the device path of the SD and only after
you verified the correct device then from your top directory run this:
```sh
sudo ./flash_sd.sh /dev/sdX
```

If you want to do the steps manually then:
```sh
sudo umount /dev/sdX*
sudo bmaptool copy <.wic.bz2_image_path> /dev/sdX
```

Of course you need to change `/dev/sdX` with you actuall SD card dev path.

## Why bmap-tools and wic images?
Well, wic images are a no-brainer. You can create a 50GB image, but this image
probably won't be that large really. Most of the times, the real data bytes in
the image will be from a few hundreds MB, to maybe 1-2 GB. The rest will be
empty space. Therefore, if you build a binary image then this image will be
filled with zeros. You will also have to use `dd` to flash the image to the SD
card. That means that for a few MBs of real data, you'll wait maybe more than
an hour to be written in the SD. Wic creates a map of the data and creates an
image with the real binary data and a bmap file that has the map data. Then,
bmaptool will use this bmap file and create the partitions and only write the
real binary data. This will take a few seconds or minutes, even for images that
are a lot of GBs.

For example the default image size for this repo is 13.8GB but the real data
are ~62MB. Therefore, with a random SD card I have here the flashing takes
~14 secs and you get a 14GB image.

## Using Docker to build the image
For consistency reasons and also to keep your main OS clean of the bloat that
Yocto needs, you can use docker to build this repo. I've provided a Dockerfile
which you can use to build the image and I'm also listing some tips how to use
it properly, in case you have several different docker containers that need to
share the download or sstate-cache folder.

> Important: To build the docker image don't copy the `Dockerfile` from
`meta-allwinner-hx/Dockerfile` to the parent folder (where `sources` folder is).
Always build the image inside `meta-allwinner-hx/Docker/`, because this will save
you from sending the build context.

To build the docker image run this command:

```sh
docker build --build-arg userid=$(id -u) --build-arg groupid=$(id -g) -t allwinner-yocto-image .
```

This will create a new image named `allwinner-yocto-image` and you can run this
to verify that it exists.
```sh
docker images
```

Which returns:
```sh
REPOSITORY              TAG                 IMAGE ID            CREATED             SIZE
allwinner-yocto-image   latest              4e89467d537a        3 minutes ago       917MB
```

Now you can create a container and run (=attach) to it. You need to run this command
in the parent folder where you can see the `sources` folder that contains all the
meta-layers.
```sh
docker run -it --name allwinner-builder -v $(pwd):/docker -w /docker allwinner-yocto-image bash
```

Then you can follow the standard procedure to build images. In case that you exit
the container, then you can just run it again and attach to it like this:
```sh
docker start allwinner-builder
docker attach allwinner-builder
```

#### Sharing download folder between several different builds
In case that you have several different yocto builds, it doens't make sense to
have a download folder for each build, because this means that you need much more
space and most of the files will be duplicated. To avoid this you can create a
`download` folder somewhere in your hard drive which can be shared from all
builds. If you don't use docker, then you just need to create this folder and
then create symlinks to every yocto build.

The problem with docker though, is that those symlinks don't work. Therefore, you
need to virtually mount the `external` folder to the docker container. To do that,
you need to create the container the first time with the correct options.

Let's assume that your shared download folder is this `/opt/yocto-downloads`.

First on the normal OS run this command:
```sh
ln -s /opt/yocto-downloads downloads
```

This is will create a symlink to the shared downloads folder.
Then to mount this folder to the docker container you need to run:
```sh
docker run -it --name allwinner-builder -v $(pwd):/docker -v /opt/yocto-downloads:/docker/downloads -w /docker allwinner-yocto-image bash
```

Then you can build the yocto image inside the container as usual, e.g.:
```sh
yoctouser@dcca27f70336:/docker$ DISTRO=allwinner-distro-console MACHINE=nanopi-k1-plus source ./setup-environment.sh build
yoctouser@dcca27f70336:/docker$ bitbake allwinner-console-image
```

## Notes
You can also build the `core-image-minimal` using this meta layer. But for some
reason when you'll get the login prompt, then the `root` account doesn't work.
This problem seems to be quite common, though.

The `allwinner-*-image` will install a service that forces the `perfomance` governor
for all the cores by default. If you want to disable this, then you can remove
the `allwinner-performance` entry from the `meta-allwinner-hx/recipes-images/images/allwinner-*-image.bb`
image file. Or you can disable the service after you boot with
```sh
systemctl disable allwinner-performance
```

> Once again, I haven't test this with all the boards, as I don't have them, but
I expect that it should work also for the others!

