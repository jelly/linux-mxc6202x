/*
 *  * 3-axis accelerometer driver for MXC6202X Memsic sensor
 *
 *  Copyright (c) 2016, Jelle van der Waa <jelle@vdwaa.nl>
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/iio/iio.h>
#include <linux/acpi.h>
#include <linux/gpio/consumer.h>
#include <linux/regmap.h>
#include <linux/iio/sysfs.h>
#include <linux/iio/trigger.h>
#include <linux/iio/buffer.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/iio/trigger_consumer.h>

#define MXC6202X_DRV_NAME	"mxc6202x"
#define MXC6202X_REG_CONTROL	0x15
#define MXC6202X_REG_CTRL	0x00
#define MXC6202X_REG_DATA	0x01

enum mxc6202x_axis {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
};

struct mxc6202x_data {
	struct i2c_client *client;
};


static const unsigned long mxc6202x_scan_masks[] = {
	BIT(AXIS_X) | BIT(AXIS_Y) | BIT(AXIS_Z),
	0
};

#define MXC4005_CHANNEL(_axis, _addr) {				\
	.type = IIO_ACCEL,					\
	.modified = 1,						\
	.channel2 = IIO_MOD_##_axis,				\
	.address = _addr,					\
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),		\
	.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE),	\
	.scan_index = AXIS_##_axis,				\
	.scan_type = {						\
		.sign = 's',					\
		.realbits = 12,					\
		.storagebits = 16,				\
		.shift = 4,					\
		.endianness = IIO_BE,				\
	},							\
}

static const struct iio_chan_spec mxc6202x_channels[] = {
	MXC4005_CHANNEL(X, 0x1),
	MXC4005_CHANNEL(Y, 0x1),
	MXC4005_CHANNEL(Z, 0x1),
};


static int mxc6202x_read_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int *val, int *val2, long mask)
{
	//dev_info(&ndio_dev->dev.parent, "mxc6202x reading raw");
	int ret;
	// How is the mask filled?
}

static const struct iio_info mxc6202x_info = {
	.driver_module 		= THIS_MODULE,
	.read_raw 		= mxc6202x_read_raw,
	//.write_raw 		= // Set's scale for example
	//.attrs attributes?
	//
};

static int mxc6202x_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int ret;
	struct iio_dev *indio_dev;
	struct mxc6202x_data *data;
	dev_info(&client->dev, "loading mxc6202x");

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*data));
	if (!indio_dev) {
		dev_err(&client->dev, "iio allocation failed!\n");
		return -ENOMEM;
	}

	i2c_set_clientdata(client, indio_dev);
	indio_dev->dev.parent = &client->dev;
	indio_dev->available_scan_masks = mxc6202x_scan_masks;
	indio_dev->name = MXC6202X_DRV_NAME;
	indio_dev->modes = INDIO_DIRECT_MODE; // ????
	indio_dev->channels = mxc6202x_channels;
	indio_dev->num_channels = 3; // FIXME
	indio_dev->info = &mxc6202x_info;

	ret = iio_device_register(indio_dev);
	if (ret < 0) {
		dev_err(&client->dev,
			"unable to register iio device %d\n", ret);
	}

	return 0;
}

static int mxc6202x_remove(struct i2c_client *client)
{
	dev_info(&client->dev, "mxc6202x remove()");
	return 0;
}


static const struct i2c_device_id mxc6202x_id[] = {
	{ "mxc6202x",	0},
	{ },
};

MODULE_DEVICE_TABLE(i2c, mxc6202x_id);

static struct i2c_driver mxc6202x_driver = {
	.driver = {
		.name = MXC6202X_DRV_NAME
	},
	.probe = mxc6202x_probe,
	.remove = mxc6202x_remove,
	.id_table = mxc6202x_id,
};

module_i2c_driver(mxc6202x_driver);

MODULE_AUTHOR("Jelle van der Waa <jelle@vdwaa.nl>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("MXC6202X 3-axis accelerometer driver");
