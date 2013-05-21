/*
 * Support for Medifield PNW Camera Imaging ISP subsystem.
 *
 * Copyright (c) 2010 Intel Corporation. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */
#ifndef _MIPI_CSI2_H_
#define _MIPI_CSI2_H_

#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-ctrls.h>

#define CSI2_PAD_SINK		0
#define CSI2_PAD_SOURCE		1
#define CSI2_PADS_NUM		2

#define CSI2_OUTPUT_ISP_SUBDEV	(1 << 0)
#define CSI2_OUTPUT_MEMORY	(1 << 1)

struct atomisp_mipi_csi2_device {
	struct v4l2_subdev subdev;
	struct media_pad pads[CSI2_PADS_NUM];
	struct v4l2_mbus_framefmt formats[CSI2_PADS_NUM];

	struct v4l2_ctrl_handler ctrls;
	struct atomisp_device *isp;

	u32 output; /* output direction */
};

int atomisp_mipi_csi2_init(struct atomisp_device *isp);
void atomisp_mipi_csi2_cleanup(struct atomisp_device *isp);
void atomisp_mipi_csi2_unregister_entities(
					struct atomisp_mipi_csi2_device *csi2);
int atomisp_mipi_csi2_register_entities(struct atomisp_mipi_csi2_device *csi2,
			struct v4l2_device *vdev);

#endif
