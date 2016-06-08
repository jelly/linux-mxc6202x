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

For example for the **sun8i-a33-q8-tablet.dts**.
```
/dts-v1/;
#include "sun8i-a33.dtsi"
#include "sun8i-q8-common.dtsi"

/ {
	model = "Q8 A33 Tablet";
	compatible = "allwinner,q8-a33", "allwinner,sun8i-a33";
}; 

&i2c1 {
    /* ... */
    mxc6202x@15 {
	    compatible = "memsic,mxc6202x";
	    reg = <0x15>;
    };
};
```
