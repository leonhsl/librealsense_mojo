// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_WRAPPER_LIBRS_WRAPPER_EXPORT_H_
#define LIBRS_MOJO_WRAPPER_LIBRS_WRAPPER_EXPORT_H_

// Define LIBRS_MOJO_WRAPPER_EXPORT so that functionality implemented by the
// librs_mojo/wrapper shared library can be exported to consumers.

#if defined(WIN32)

#if defined(LIBRS_MOJO_WRAPPER_IMPLEMENTATION)
#define LIBRS_MOJO_WRAPPER_EXPORT __declspec(dllexport)
#else
#define LIBRS_MOJO_WRAPPER_EXPORT __declspec(dllimport)
#endif  // defined(LIBRS_MOJO_WRAPPER_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(LIBRS_MOJO_WRAPPER_IMPLEMENTATION)
#define LIBRS_MOJO_WRAPPER_EXPORT __attribute__((visibility("default")))
#else
#define LIBRS_MOJO_WRAPPER_EXPORT
#endif
#endif

#endif  // LIBRS_MOJO_WRAPPER_LIBRS_WRAPPER_EXPORT_H_
