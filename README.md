# linux-mxc6202x
MXC6202x mainline accelerometer iio driver

Accelerometer used in a certain q8 allwinner tablet.

Add the following to your dts file to make Linux pick up the driver.

```
&i2c1 {
    /* ... */
    mxc6202x@15 {
        compatible = "memsic,mxc6202x";
        reg = <0x15>;
    };
};
```
