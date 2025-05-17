/****************************************************************************
 * apps/examples/hello/hello_main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/boardctl.h>
#include <nuttx/sensors/icm42688.h>

#define IMU_DEVPATH "dev/imu0"
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * hello_main
 ****************************************************************************/
struct icm42688_imu_msg
{
  float temp;
  float acc_x;
  float acc_y;
  float acc_z;
  float gyro_x;
  float gyro_y;
  float gyro_z;
};

int main(int argc, FAR char *argv[])
{
  int fd;
  struct icm42688_imu_msg data;
  uint32_t prev;
  boardctl(BOARDIOC_INIT, 0);

  fd = open(IMU_DEVPATH, O_RDONLY);
  if (fd < 0)
  {
    printf("Device %s open failure. %d\n", IMU_DEVPATH, fd);
    return -1;
  }

  prev = 0;
  for (;;)
  {
    int ret;

    ret = read(fd, &data, sizeof(struct icm42688_imu_msg));
    if (ret != sizeof(struct icm42688_imu_msg))
    {
      fprintf(stderr, "Read failed.\n");
      break;
    }

    /* If sensing time has been changed, show 6 axis data. */

    printf("%.2f / %.2f, %.2f, %.2f / %.2f, %.2f, %.2f\n",
          data.temp,
           data.acc_x, data.acc_y, data.acc_z,
           data.gyro_x, data.gyro_y, data.gyro_z);
    fflush(stdout);
    nxsig_usleep(200000);
  }

  close(fd);
  return 0;
}
