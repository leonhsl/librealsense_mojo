// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "../include/librealsense_wrapper/librs_thunks.h"

#include <assert.h>

#include "librs_thunks_api.h"

namespace librs {
namespace wrapper {

namespace {
LibraryThunks g_thunks = {0};
}

size_t SetThunks(const struct LibraryThunks* thunks) {
  if (thunks->size >= sizeof(g_thunks))
    g_thunks = *thunks;
  return sizeof(g_thunks);
}

void* CreateContextThunk() {
  assert(g_thunks.CreateContext);
  return g_thunks.CreateContext();
}

void DeleteContextThunk(void* context) {
  assert(g_thunks.DeleteContext);
  g_thunks.DeleteContext(context);
}

int ContextGetDeviceCountThunk(void* context) {
  assert(g_thunks.ContextGetDeviceCount);
  return g_thunks.ContextGetDeviceCount(context);
}

void* ContextGetDeviceThunk(void* context, int index) {
  assert(g_thunks.ContextGetDevice);
  return g_thunks.ContextGetDevice(context, index);
}

void DeleteDeviceThunk(void* device) {
  assert(g_thunks.DeleteDevice);
  g_thunks.DeleteDevice(device);
}

const char* DeviceGetNameThunk(void* device) {
  assert(g_thunks.DeviceGetName);
  return g_thunks.DeviceGetName(device);
}

}  // namespace wrapper
}  // namespace librs
