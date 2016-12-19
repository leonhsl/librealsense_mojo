// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICES_EXPORT_H_
#define LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICES_EXPORT_H_

// Define LIBRS_MOJO_SERVICES_EXPORT so that functionality implemented by the
// librs_mojo/services module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(LIBRS_MOJO_SERVICES_IMPLEMENTATION)
#define LIBRS_MOJO_SERVICES_EXPORT __declspec(dllexport)
#else
#define LIBRS_MOJO_SERVICES_EXPORT __declspec(dllimport)
#endif  // defined(LIBRS_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(LIBRS_MOJO_SERVICES_IMPLEMENTATION)
#define LIBRS_MOJO_SERVICES_EXPORT __attribute__((visibility("default")))
#else
#define LIBRS_MOJO_SERVICES_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define LIBRS_MOJO_SERVICES_EXPORT
#endif

#endif  // LIBRS_MOJO_SERVICES_LIBRS_MOJO_SERVICES_EXPORT_H_
