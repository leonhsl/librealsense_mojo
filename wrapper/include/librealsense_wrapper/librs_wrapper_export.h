// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_WRAPPER_LIBRS_WRAPPER_EXPORT_H_
#define LIBRS_MOJO_WRAPPER_LIBRS_WRAPPER_EXPORT_H_

#if defined(LIBRS_MOJO_WRAPPER_IMPLEMENTATION)
#define LIBRS_MOJO_WRAPPER_EXPORT __attribute__((visibility("default")))
#else
#define LIBRS_MOJO_WRAPPER_EXPORT
#endif

#endif  // LIBRS_MOJO_WRAPPER_LIBRS_WRAPPER_EXPORT_H_
