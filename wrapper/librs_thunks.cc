// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "librs_mojo/wrapper/librs_thunks.h"

#include <assert.h>

#include "librs_mojo/wrapper/librs_thunks_api.h"

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

int GetDeviceCountThunk(void* context) {
  assert(g_thunks.GetDeviceCount);
  return g_thunks.GetDeviceCount(context);
}

}  // namespace wrapper
}  // namespace librs
