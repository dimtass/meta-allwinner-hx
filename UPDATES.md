Update information
----

#### 15.03.2021
- Updated PREEMPT-RT kernel to `5.10.18-rt32`. Warning: The patch is the `5.10.17-rt32` applied on 5.10.18!
- Only verified build method is to use the included Dockerfile
- Added `libmpc-dev` and `libgmp-dev` packages in the Dockerfile. You need to rebuild your docker image.
- Added two new options in `local.conf` which are `BOOTSPLASH` and `WIFI_INJECTION` and they're enabled by default.
  To disable them replace `yes` with `no`.
```
BOOTSPLASH = "yes"
WIFI_INJECTION = "yes"
```

#### 16.01.2021
Poky 3.2.1 version (gatesgarth) currently supports GCC 10.2 which has a
[known bug](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=96377). This bug
is fixed in GCC 10.3. Therefore, be advised that _the Neon acceleration for
AEGIS128 is disabled in the kernel_.

This is the default configuration of the Armbian kernel:
```
CONFIG_CRYPTO_AEGIS128=m
CONFIG_CRYPTO_AEGIS128_SIMD=y
```

And this is the configuration of this kernel.
```
CONFIG_CRYPTO_AEGIS128=m
```

I will revert this change when GCC is updated.

If you need the neon acceleration vectors then use the `dunfell` brach and do not
update to `master` or `gatesgarth`.

#### 11.01.2021
One of the major changes in this update is that the `allwinner-wks-defs.inc` has been moved from:
```
classes/allwinner-wks-defs.inc -> conf/machine/include/allwinner-defs.inc
```

The reason for this change is that from now on the `rootdev` device in the `boot.scr` script is no
longer automatically picked up by u-boot because this creates issues in kernels post to 5.8.x.
For this reason the `SUNXI_STORAGE_DEVICE` in `conf/machine/include/allwinner-defs.inc` is used from
now on as the base boot device. This value is automatically replaced in `recipes-bsp/u-boot/files/allwinnerEnv.txt`,
`recipes-bsp/u-boot/files/fw_env.config` and all relative scripts like `classes/allwinner-create-wks.bbclass`.

> Note: You may have to change the `SUNXI_STORAGE_DEVICE` parameter depending on your board.
(e.g. `mmcblk0` or `mmcblk2`). I can't test with all the boards!

